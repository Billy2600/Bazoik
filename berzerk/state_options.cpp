#include "state_options.h"

StateOptions::StateOptions( Game *game )
{
	this->game = game;
	AssetManager *assetManager = &this->game->assetManager;

	awaitingRebind = false;
	error = false;
	title.setFont( assetManager->GetFontRef( "joystix" ) );
#ifdef OLD_SFML
	title.setColor( sf::Color::Green );
#else
	title.setFillColor( sf::Color::Green );
#endif
	title.setCharacterSize( 70 );
	title.setPosition( sf::Vector2f( 30, 30 ) );
	title.setString( "Options" );

	rebind = title;
	rebind.setPosition( 30, 100 );
	rebind.setCharacterSize( 30 );
	rebind.setString( "Rebind Keys:" );

	const float xPos = 30; // They all have the same x position
	float yPos = 150.f; // Y position for the first item, will increment as we go down the buttons
	const float yIncrement = 25.f;
	int order = 1; // Same as above, but for the button order

	buttons["up"] = GuiButton( sf::Vector2f( 30, yPos += yIncrement ), sf::Vector2f( 250, 15 ), sf::Vector2f( 5, 3 ), "", assetManager->GetFontRef( "joystix" ), 0 );
	buttons["up"].SetColors( sf::Color::Black, sf::Color::Green, sf::Color::Transparent );
	buttons["up"].SetHighlightColors( sf::Color::Black, sf::Color::Red, sf::Color::Transparent );
	buttons["up"].SetHighlight( false );
	buttons["up"].SetCharacterSize(15);

	buttons["down"] = buttons["up"];
	buttons["down"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["down"].order = order++;

	buttons["left"] = buttons["up"];
	buttons["left"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["left"].order = order++;

	buttons["right"] = buttons["up"];
	buttons["right"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["right"].order = order++;

	buttons["fireUp"] = buttons["up"];
	buttons["fireUp"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["fireUp"].order = order++;

	buttons["fireLeft"] = buttons["up"];
	buttons["fireLeft"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["fireLeft"].order = order++;

	buttons["fireRight"] = buttons["up"];
	buttons["fireRight"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["fireRight"].order = order++;

	buttons["fireDown"] = buttons["up"];
	buttons["fireDown"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["fireDown"].order = order++;

	buttons["pause"] = buttons["up"];
	buttons["pause"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["pause"].order = order++;

	buttons["accept"] = buttons["up"];
	buttons["accept"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["accept"].order = order++;

	buttons["fullscreen"] = buttons["up"];
	buttons["fullscreen"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["fullscreen"].order = order++;

	buttons["return"] = buttons["up"];
	buttons["return"].SetPos( sf::Vector2f( xPos, yPos += yIncrement ) );
	buttons["return"].order = order++;
	buttons["return"].SetText( "Save & Return" );

	selectedButton = 0;
	joystickInput = false;
}

void StateOptions::Start()
{
	log.Write( "Options state started" );
}

void StateOptions::HandleInput()
{
	sf::Event event;
	sf::Vector2f m = game->window.mapPixelToCoords( sf::Mouse::getPosition( this->game->window ) );

	while( game->window.pollEvent( event ) )
	{
		// Close window
		if ( event.type == sf::Event::Closed )
		{
			game->Close();
			return;
		}

		if( event.type == sf::Event::KeyPressed )
		{
			if( event.key.code == sf::Keyboard::Key::Escape )
			{
				Return();
				break;
			}

			// Set bind, if we were waiting for one
			if( awaitingRebind && event.key.code != sf::Keyboard::Key::Return )
			{
				InputManager::Keys newKey;
				newKey.inputType = InputManager::InputType::Keyboard;
				newKey.keyCode = event.key.code;
				SetBind( newKey );
				return;
			}
		}

		if( event.type == sf::Event::JoystickButtonPressed )
		{
			if( awaitingRebind )
			{
				InputManager::Keys newKey;
				newKey.inputType = InputManager::InputType::Joystick;
				newKey.joystickButton = event.joystickButton.button;
				SetBind( newKey );
				return;
			}
		}

		if( event.type == sf::Event::JoystickMoved )
		{
			if( awaitingRebind )
			{
				InputManager::Keys newKey;
				newKey.inputType = InputManager::InputType::Axis;
				newKey.axis = event.joystickMove.axis;
				newKey.axisPos = event.joystickMove.position;
				SetBind( newKey );
				return;
			}
		}

		if( event.type == sf::Event::MouseButtonPressed || game->inputManager.TestKeyDown( "accept", event ) || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Return )
		{
			// Check if mouse was inside any of the gui buttons
			for( auto button : buttons )
			{
				if( button.second.hitbox.contains( m ) || ( joystickInput && button.second.order == selectedButton )
					|| button.second.order == selectedButton  && event.key.code == sf::Keyboard::Key::Return )
				{
					awaitingRebind = true;
					bind = button.first;
				}
			}
		}

		if( event.type == sf::Event::MouseMoved )
		{
			joystickInput = false;
			// Check GUI highlights
			for( auto& button : buttons )
			{
				if( button.second.hitbox.contains( m ) )
					button.second.SetHighlight( true );
				else
				{
					if( !joystickInput || button.second.order != selectedButton )
						button.second.SetHighlight( false );
				}
			}
		}

		// Interact with buttons with joystick
		if( !awaitingRebind )
		{
			if( game->inputManager.TestKeyDown( "up", event ) 
				|| ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Up ) )
			{
				if( joystickInput )
				{
					selectedButton--;
					if( selectedButton < 0 )
						selectedButton = 0;
				}
				else
					joystickInput = true;
			}

			if( game->inputManager.TestKeyDown( "down", event ) 
				|| ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Down ) )
			{
				if( joystickInput )
				{
					joystickInput = true;
					selectedButton++;
					if( (unsigned int) selectedButton >= buttons.size() )
						selectedButton = buttons.size() - 1;
				}
				else
					joystickInput = true;
			}

			// Highlight buttons
			if( joystickInput )
			{
				for( auto& button : buttons )
				{
					if( button.second.order == selectedButton )
					{
						button.second.SetHighlight( true );
					}
					else
					{
						button.second.SetHighlight( false );
					}
				}
			}
		}
	}
}

void StateOptions::Update(const float dt)
{
	if( awaitingRebind )
	{
		// Just toggle fullscreen selector
		if ( bind == "fullscreen" )
		{
			game->inputManager.fullscreen = !game->inputManager.fullscreen;
			awaitingRebind = false;
			error = true;
			std::string type = "windowed";
			if ( game->inputManager.fullscreen ) type = "fullscreen";
			rebind.setString( "Relaunch game for " + type );
			return;
		}
		// Return button
		if ( bind == "return" )
		{
			Return();
			return;
		}

		error = false;
		rebind.setString( "Press key to bind " + bind );
	}
	else
	{
		if ( !error )
			rebind.setString( "Rebind keys:" );

		buttons["up"].SetText( "up:    " + game->inputManager.GetKeyName( "up" ) );
		buttons["down"].SetText( "down:  " + game->inputManager.GetKeyName( "down" ) );
		buttons["left"].SetText( "left:  " + game->inputManager.GetKeyName( "left" ) );
		buttons["right"].SetText( "right: " + game->inputManager.GetKeyName( "right" ) );
		buttons["fireUp"].SetText( "fire up:  " + game->inputManager.GetKeyName( "fireUp" ) );
		buttons["fireLeft"].SetText( "fire left:  " + game->inputManager.GetKeyName( "fireLeft" ) );
		buttons["fireRight"].SetText( "fire right:  " + game->inputManager.GetKeyName( "fireRight" ) );
		buttons["fireDown"].SetText( "fire down:  " + game->inputManager.GetKeyName( "fireDown" ) );
		buttons["pause"].SetText( "pause: " + game->inputManager.GetKeyName( "pause" ) );
		buttons["accept"].SetText( "accept:  " + game->inputManager.GetKeyName( "accept" ) );
	}

	if ( game->inputManager.fullscreen )
		buttons["fullscreen"].SetText( "Fullscreen: Yes" );
	else
		buttons["fullscreen"].SetText( "Fullscreen: No" );
}

void StateOptions::Draw() const
{
	game->window.draw( title );
	game->window.draw( rebind );

	for( auto button : buttons )
	{
		game->window.draw( button.second );
	}
}

void StateOptions::Return()
{
	log.Write( "Quitting options state" );
	game->inputManager.SaveToFile( "keys.xml" );
	game->PopState();
}

void StateOptions::SetBind( InputManager::Keys newKey )
{
	if( game->inputManager.CheckExistingBind( newKey ) )
	{
		rebind.setString( "You have already bound that key" );
		error = true;
	}
	else
	{
		game->inputManager.SetBind( bind, newKey );
	}
	awaitingRebind = false;
}