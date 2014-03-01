#include "CreditsState.h"

CreditsState::CreditsState()
	: MenuState(SharedConstants::CREDITS_STATE)
{
}

bool CreditsState::MenuLoad()
{
	if(overlayImage.LoadFromFile(CREDITS_OVERLAY_IMAGE))
	{
		overlaySprite.SetImage(overlayImage);
	}
	else
	{
		return false;
	}

	return true;
}

CreditsState::~CreditsState()
{
}

void CreditsState::MenuUpdate(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
}

void CreditsState::MenuDraw(sf::RenderWindow* renderWindow)
{
}

void CreditsState::SetView(sf::RenderWindow* renderWindow)
{
}

void CreditsState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
}
