#include "BulletHandler.h"

#define fullUpdateUpperBound 200
#define partialUpdateUpperBound 600

BulletHandler::BulletHandler(PlayerNetworkData* playerNetworkData)
{


	this->playerNetworkData = playerNetworkData;
	//set the waiting for vector to be the length of the number of players, the structure is a 2d array with the bullets that need to be received being stored for each player
	std::vector<sf::Uint32> emptyVector;
	bulletSendRedundancyCount = 3;
	bulletIDCounter = 0;
	bulletDimensions.x = 38;
	bulletDimensions.y = 12;
	bulletCenterXOffset = 28;
	bulletCenterYOffset = 6;
}


BulletHandler::~BulletHandler(void)
{
	playerNetworkData = NULL;
}

void BulletHandler::CheckCollision(std::vector<sf::Rect<float>> &platforms, std::vector<Player> &players, sf::Rect<float> levelBounds, sf::Uint32 stateIterator)
{
	//Loop through the bullets
	for(size_t i = 0; i < projectileList.size(); i++)
	{
		bool playerHasCollided = false;
		//Deals with player collisions
		for(size_t k = 0; k < players.size(); k++)
		{
			if(playerNetworkData->playersActive[k])
			{
				if(players[k].GetBounds().Intersects(projectileList[i]->GetBounds()))
				{
					ProjectileDeathPacket deathPacket;
					deathPacket.PackData(sharedConstants.GAME_STATE, projectileList[i]->GetBulletID(), projectileList[i]->GetPosition(), stateIterator, deathPacket);

					if(projectileList[i] != NULL)
					{
						deathsToSend.push_back(deathPacket);
					}

					playerHasCollided = true;
					players[k].SetHealth(players[k].GetHealth() - 10);
					//Check if the player is dead so the score can be incremented
					if(players[k].GetHealth() <= 0)
					{
						players[projectileList[i]->GetBulletOwner()].SetScore(players[projectileList[i]->GetBulletOwner()].GetScore() + 1);
					}

					//THIS ERASE IS SUPER INNEFICIANT AND SHOULD PROBABLY BE DONE WITH A SET TO NULL WITH SMARTER BULLET SPAWNING
					projectileList.erase(projectileList.begin() + i);
					break;
				}
			}
		}

		//This is done so you dont get a vector exception if you kill the bullet with a player above. Should probly be seperate like, checkLevelCollision, CheckPlayerCollision, etc
		if(playerHasCollided == true)
		{
			continue;
		}


		//Deals with destroying bullets if they goe outside the level
		if(levelBounds.Intersects(projectileList[i]->GetBounds()) == false)
		{
			ProjectileDeathPacket deathPacket;
			deathPacket.PackData(sharedConstants.GAME_STATE, projectileList[i]->GetBulletID(), projectileList[i]->GetPosition(), stateIterator, deathPacket);

				if(projectileList[i] != NULL)
				{
					deathsToSend.push_back(deathPacket);
				}

				//THIS ERASE IS SUPER INNEFICIANT AND SHOULD PROBABLY BE DONE WITH A SET TO NULL WITH SMARTER BULLET SPAWNING
				projectileList.erase(projectileList.begin() + i);
				continue;
		}

		//Loop through the platforms
		for(size_t j = 0; j < platforms.size(); j++)
		{
			if(platforms[j].Intersects(projectileList[i]->GetBounds()))
			{
				ProjectileDeathPacket deathPacket;
				deathPacket.PackData(sharedConstants.GAME_STATE, projectileList[i]->GetBulletID(), projectileList[i]->GetPosition(), stateIterator, deathPacket);

				if(projectileList[i] != NULL)
				{
					deathsToSend.push_back(deathPacket);
				}

				// THIS ERASE IS SUPER INNEFICIANT AND SHOULD PROBABLY BE DONE WITH A SET TO NULL WITH SMARTER BULLET SPAWNING
				projectileList.erase(projectileList.begin() + i);
				break;
			}
		}
	}
}

void BulletHandler::SpawnBullet(int bulletOwner,sf::Vector2f spawnPosition, sf::Vector2f velocity, sf::Uint32 stateIterator)
{

	// Create the bullet on the server
	projectileList.push_back(new Projectile(spawnPosition.x , spawnPosition.y ,velocity, bulletDimensions, bulletIDCounter,bulletOwner));

	// Pack a packet to be put into the bullets to send vector
	ProjectilePacket bulletPacket;
	bulletPacket.PackData(sharedConstants.GAME_STATE,bulletIDCounter,spawnPosition,velocity,stateIterator,bulletPacket);

	// Push this packet into the bullets to send vector
	bulletsToSend.push_back(bulletPacket);

	bulletIDCounter++;
}

void BulletHandler::SendPendingBullets(ServerTransmitter &serverTransmitter)
{
	if(bulletsToSend.size() > 0)
	{
		// Send any bullets waiting in the bulletsToSend Buffer, and send them lots of times as defined by bulletSendRedundancyCount
		for(int i = 0; i < bulletSendRedundancyCount; i++)
		{
			for(size_t j = 0; j < playerNetworkData->playerIPs.size(); j++)
			{
				for(size_t k = 0; k < bulletsToSend.size(); k++)
				{
					if(playerNetworkData->playerIPs[j] != NULL_IP)
					{
						serverTransmitter.SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[j], bulletsToSend[k]);
					}
				}
			}
		}
		// Clear the buffer
		bulletsToSend.clear();	
	}
}

void BulletHandler::SendPendingDeathPackets(ServerTransmitter &serverTransmitter)
{
	if(deathsToSend.size() > 0)
	{
		// Send any deaths waiting in the DeathsToSend Buffer, and send them lots of times as defined by bulletSendRedundancyCount
		for(int i = 0; i < bulletSendRedundancyCount; i++)
		{
			for(size_t j = 0; j < playerNetworkData->playerIPs.size(); j++)
			{
				for(size_t k = 0; k < deathsToSend.size(); k++)
				{
					if(playerNetworkData->playerIPs[j] != NULL_IP)
					{
						serverTransmitter.SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[j], deathsToSend[k]);
					}
				}
			}
		}

		// Clear the buffer
		deathsToSend.clear();	
	}
}

void BulletHandler::Update(ServerTransmitter &serverTransmitter,sf::Uint32 stateIterator, sf::Rect<float> levelBounds, std::vector<Player> &players,std::vector<sf::Rect<float>> &platforms)
{
	SendPendingBullets(serverTransmitter);
	SendPendingDeathPackets(serverTransmitter);

	// Update bullet positions
	for(size_t i = 0; i < projectileList.size(); i++)
	{
		projectileList[i]->Move(projectileList[i]->GetVelocity().x,projectileList[i]->GetVelocity().y);
	}

	CheckCollision(platforms,players,levelBounds,stateIterator);
}
