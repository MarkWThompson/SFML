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
	for(int i = 0; i < projectileList.size(); i++)
	{
		bool playerHasCollided = false;
		//Deals with player collisions
		for(int k = 0; k < players.size(); k++)
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

					//THIS ERASE IS SUPER INNEFICIANT AND SHOULD PROBABLY BE DONE WITH A SET TO NULL WITH SMARTER BULLET SPAWNING
					projectileList.erase(projectileList.begin() + i);

					playerHasCollided = true;
					players[k].SetHealth(players[k].GetHealth() - 10);
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
		for(int j = 0; j < platforms.size(); j++)
		{
			if(platforms[j].Intersects(projectileList[i]->GetBounds()))
			{
				ProjectileDeathPacket deathPacket;
				deathPacket.PackData(sharedConstants.GAME_STATE, projectileList[i]->GetBulletID(), projectileList[i]->GetPosition(), stateIterator, deathPacket);

				if(projectileList[i] != NULL)
				{
					deathsToSend.push_back(deathPacket);
				}

				//THIS ERASE IS SUPER INNEFICIANT AND SHOULD PROBABLY BE DONE WITH A SET TO NULL WITH SMARTER BULLET SPAWNING
				projectileList.erase(projectileList.begin() + i);
				break;
			}
		}
	}
}

void BulletHandler::SpawnBullet(sf::Vector2f spawnPosition, sf::Vector2f velocity, sf::Uint32 stateIterator)
{

	//Create the bullet on the server
	projectileList.push_back(new Projectile(spawnPosition.x , spawnPosition.y ,velocity, bulletDimensions, bulletIDCounter,playerNetworkData->MAX_PLAYERS));

	//pack a packet to be put into the bullets to send vector
	ProjectilePacket bulletPacket;
	bulletPacket.PackData(sharedConstants.GAME_STATE,bulletIDCounter,spawnPosition,velocity,stateIterator,bulletPacket);

	//push this packet into the bullets to send vector
	bulletsToSend.push_back(bulletPacket);

	bulletIDCounter++;
}

void BulletHandler::SendPendingBullets(ServerTransmitter &serverTransmitter)
{
	if(bulletsToSend.size() > 0)
	{
		//send any bullets waiting in the bulletsToSend Buffer, and send them lots of times as defined by bulletSendRedundancyCount
		for(int i = 0; i < bulletSendRedundancyCount; i++)
		{
			for(int j = 0; j < playerNetworkData->playerIPs.size(); j++)
			{
				for(int k = 0; k < bulletsToSend.size(); k++)
				{
					if(playerNetworkData->playerIPs[j] != NULL_IP)
					{
						serverTransmitter.SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[j], bulletsToSend[k]);
					}
				}
			}
		}
		//clear the buffer
		bulletsToSend.clear();	
	}
}

void BulletHandler::SendPendingDeathPackets(ServerTransmitter &serverTransmitter)
{
	if(deathsToSend.size() > 0)
	{
		//send any deaths waiting in the DeathsToSend Buffer, and send them lots of times as defined by bulletSendRedundancyCount
		for(int i = 0; i < bulletSendRedundancyCount; i++)
		{
			for(int j = 0; j < playerNetworkData->playerIPs.size(); j++)
			{
				for(int k = 0; k < deathsToSend.size(); k++)
				{
					if(playerNetworkData->playerIPs[j] != NULL_IP)
					{
						serverTransmitter.SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[j], deathsToSend[k]);
					}
				}
			}
		}
		//clear the buffer
		deathsToSend.clear();	
	}
}

void BulletHandler::Update(ServerTransmitter &serverTransmitter,sf::Uint32 stateIterator, sf::Rect<float> levelBounds, std::vector<Player> &players,std::vector<sf::Rect<float>> &platforms)
{
	SendPendingBullets(serverTransmitter);
	SendPendingDeathPackets(serverTransmitter);

	//update bullet positions
	for(int i = 0; i < projectileList.size(); i++)
	{
		projectileList[i]->Move(projectileList[i]->GetVelocity().x,projectileList[i]->GetVelocity().y);
	}

	CheckCollision(platforms,players,levelBounds,stateIterator);
}

/*
// DUMB AND DEAD CODE, JUST IN CASE I NEED IT 

void BulletHandler::SmartUpdateBullets(ServerTransmitter &serverTransmitter,sf::Uint32 stateIterator)
{
	int fullNetworkUpdateTimeDelay = 0.3f; //update once every 0.2 seconds on the full update
	int partialNetworkUpdateTimeDelay = 2.0; //update once every second on the partial update

	for(int i = 0; i < playerNetworkData->playerIPs.size(); i++)
	{
		if(playerNetworkData->playerIPs[i] != NULL_IP)
		{
			//cycle through the bullets, and check what update bound they are in. then as according to their lastNetworkSend timer, appropriately add them to the bulletsToSend vector
			for(int j = 0; j < projectileList.size(); j++)
			{
				if(projectileList[j] != NULL)
				{
					sf::Vector2f playerPos = playerNetworkData->players[i].GetPosition();
					sf::Vector2f projectilePos = projectileList[j]->GetPosition();

					//YOYO Here you get the distance difference between the player and the bullet, and then do shit
					float distance = ((playerPos.x - projectilePos.x) * (playerPos.x - projectilePos.x)) + ((playerPos.y - projectilePos.y) * (playerPos.y - projectilePos.y));
					distance = sqrt(distance);

					if(distance < fullUpdateUpperBound)
					{
						//full update code
						if(projectileList[j]->lastNetworkSends[i].GetElapsedTime() > fullNetworkUpdateTimeDelay)
						{
							projectileList[j]->lastNetworkSends[i].Reset();
							//pack a packet to be put into the bullets to send vector
							ProjectilePacket bulletPacket;
							bulletPacket.PackData(sharedConstants.GAME_STATE,projectileList[j]->GetPosition(),projectileList[j]->GetVelocity(),projectileList[j]->GetBulletID(),stateIterator,bulletPacket);

							//Send the bullet to the relevent player!
							serverTransmitter.SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[i], bulletPacket);
						}
					}
					else if((distance > fullUpdateUpperBound) && (distance < partialUpdateUpperBound))
					{
						//partial update code
						if(projectileList[j]->lastNetworkSends[i].GetElapsedTime() > partialNetworkUpdateTimeDelay)
						{
							projectileList[j]->lastNetworkSends[i].Reset();
							//pack a packet to be put into the bullets to send vector
							ProjectilePacket bulletPacket;
							bulletPacket.PackData(sharedConstants.GAME_STATE,projectileList[j]->GetPosition(),projectileList[j]->GetVelocity(),projectileList[j]->GetBulletID(),stateIterator,bulletPacket);

							//Send the bullet to the relevent player!
							serverTransmitter.SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[i], bulletPacket);
						}
					}
					//else no update, so no code needed
				}
			}
		}
	}
}
*/