#pragma once

#include <SFML\Graphics.hpp>
#include "..\SharedConstants.h"

class State
{
	public:
		/** Default constructor initialises properties. */
		State();

		/** Base destructor. */
		virtual ~State();

		/** Abstract method intended to act as the main loop of the state.*/
		virtual void Update(sf::Event events, const sf::Input &input) = 0;

		/** Abstract method intended to render all objects of the state. */
		virtual void Draw(sf::RenderWindow &renderWindow) = 0;

		/**
		 * Abstract method intended to load state content.
		 * For instance: positioning objects, loading images, setting parameters.
		 * Returns true if the operation was successful, otherwise returns false.
		 */
		virtual bool Load() = 0;

		/** Returns the target variable. */
		SharedConstants::StateID GetTarget();
		
		/** Returns true if targetID differs from ID, otherwise returns false. */
		bool Switch();

	protected:
		/** The unique ID of the state. */
		SharedConstants::StateID ID;

		/** The unique ID of the desired state to switch to. */
		SharedConstants::StateID targetID;

		/** Determines if the content of the state has been loaded properly. */
		bool loaded;

};