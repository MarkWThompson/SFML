#pragma once
#include "ServerRouter.h"

class ServerReceiver
{
public:
	ServerReceiver();
	~ServerReceiver();
	void ReceiveUDP(unsigned short port, ServerRouter &networkRouter, bool blocking);
};

