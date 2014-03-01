#include "HelpState.h"

HelpState::HelpState()
	: MenuState(SharedConstants::HELP_STATE)
{
}

bool HelpState::MenuLoad()
{
	if(overlayImage.LoadFromFile(HELP_OVERLAY_IMAGE))
	{
		overlaySprite.SetImage(overlayImage);
	}
	else
	{
		return false;
	}

	return true;
}

HelpState::~HelpState()
{
}

void HelpState::MenuUpdate(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
}

void HelpState::MenuDraw(sf::RenderWindow* renderWindow)
{
}

void HelpState::SetView(sf::RenderWindow* renderWindow)
{
}

void HelpState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
}
