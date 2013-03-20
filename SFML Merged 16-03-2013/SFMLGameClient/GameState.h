#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientTransmitter.h"
#include "State.h"
#include "Assets.h"
#include "PacketTypes.h"
#include "SharedConstants.h"
#include "ServerNetworkData.h"
#include "PlayerSprite.h"
#include <string>
#include "Level.h"
#include "BulletSprite.h"
#include "ScoreBoard.h"

class GameState : public State
{
public:
	/** Constructor initialiser. */
	GameState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData);

	/** Default destructor. */
	~GameState();

	/** 
	 * Loads GameState content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool Load();

	/** Main loop. */
	void Update(sf::Event events, bool eventFired, const sf::Input &input);

	/** Renders all of the applicable state content. */
	void Draw(sf::RenderWindow &renderWindow);

	/** Receives a packet a processes its contents based upon its type. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	/** Checks if the connection to the server has been lost. */
	void CheckTimeout();

	void UnpackPlayerPositionsPacket(sf::Packet &receivedPacket, std::vector<bool> &playersActive, std::vector<PlayerSprite> &playerSprites, sf::Uint32 &stateIterator);
	void UnpackProjectilePacket(sf::Packet &receivedPacket);
	void UnpackProjectileDeathPacket(sf::Packet &receivedPacket);

	/** Determines if the client is experiencing packet loss from server. */
	bool connectionProblem;

	//Checks if an object is on the screen
	bool IsOnScreen(sf::RenderWindow &renderWindow, sf::Sprite &sprite);

	/** Used to transmit packets from this state to the server. */
	ClientTransmitter* clientTransmitter;
	ServerNetworkData* serverNetworkData;

	// Player containers
	sf::Image playerImage;
	std::vector<PlayerSprite> playerSprites;
	std::vector<bool> playersActive;

	//bullets
	int tempCount;
	sf::Image projectileImage;
	std::vector<BulletSprite*> projectileList;
	sf::Uint32 workingBulletID;

	// Connection interrupted
	sf::Image interuptImage;
	sf::Sprite interuptSprite;

	Level level;

	/** The loop counter for the whole fixed update loop, server is authoratative. */
	sf::Uint32 stateIterator;

	/** The clock updates the above stateIterator at the fixed update loop, but can be overwritten by the server, which is authorotative. */
	sf::Clock timeStepClock;

	//The scoreboard
	ScoreBoard scoreBoard;
	bool showScoreBoard;
	bool shouldGetScores;
	sf::Clock scoreBoardRefreshClock;
	float scoreBoardRefreshTime;
};
