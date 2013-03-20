#pragma once

#include <iostream>
#include <SFML\Graphics.hpp>
#include "Projectile.h"
#include "PacketTypes.h"
#include "PlayerNetworkData.h"
#include "ServerTransmitter.h"
#include "SharedConstants.h"
#include <algorithm>
#include <utility>
#include "Player.h"
#include <math.h>

class BulletHandler
{
public:
	BulletHandler(PlayerNetworkData* playerNetworkData);
	~BulletHandler(void);

	PlayerNetworkData* playerNetworkData;

	void SpawnBullet(sf::Vector2f spawnPosition, sf::Vector2f velocity, sf::Uint32 stateIterator);
	void Update(ServerTransmitter &serverTransmitter,sf::Uint32 stateIterator, sf::Rect<float> levelBounds, std::vector<Player> &players, std::vector<sf::Rect<float>> &platforms);
	void SendPendingBullets(ServerTransmitter &serverTransmitter);
	void SendPendingDeathPackets(ServerTransmitter &serverTransmitter);
	void CheckCollision(std::vector<sf::Rect<float>> &platforms, std::vector<Player> &players, sf::Rect<float> levelBounds, sf::Uint32 stateIterator);

private:

	sf::Vector2f bulletDimensions;
	float bulletCenterXOffset;
	float bulletCenterYOffset;
	int bulletSendRedundancyCount;

	//Bullet list
	std::vector<Projectile*> projectileList;
	//Bullet Id counter
	sf::Uint32 bulletIDCounter;
	//Bullets that need to be sent
	std::vector<ProjectilePacket> bulletsToSend;
	//Death messages that need to be send
	std::vector<ProjectileDeathPacket> deathsToSend;
};

