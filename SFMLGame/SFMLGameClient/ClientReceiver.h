#pragma once

#include <SFML\Network.hpp>
#include "ClientRouter.h"

class ClientReceiver
{
public:
	ClientReceiver();
	~ClientReceiver();
	void ReceiveUDP(unsigned short port, ClientRouter &networkRouter, bool blocking);

};

