#include "state_options.h"

StateOptions::StateOptions( Game *game )
{
	this->game = game;
	AssetManager *assetManager = &this->game->assetManager;

	awaitingRebind = false;
	error = false;
	title.setFont( assetManager->GetFontRef( "joystix" ) );
	title.setFillColor( sf::Color::Green );
	title.setCharacterSize( 70 );
	title.setPosition( sf::Vector2f( 30, 30 ) );
	title.setString( "Options" );

	rebind = title;
	rebind.setPosition( 30, 100 );
	rebind.setCharacterSize( 30 );
	rebind.setString( "Rebind Keys:" );

	buttons["up"] = GuiButton( sf::Vector2f( 30, 150 ), sf::Vector2f( 500, 50 ), sf::Vector2f( 10, 3 ), "", assetManager->GetFontRef( "joystix" ) );
	buttons["up"].SetColors( sf::Color::Black, sf::Color::Green, sf::Color::Transparent );
	buttons["up"].SetHighlightColors( sf::Color::Black, sf::Color::Red, sf::Color::Transparent );
	buttons["up"].SetHighlight( false );

	buttons["down"] = buttons["up"];
	buttons["down"].SetPos( sf::Vector2f( 30, 200 ) );

	buttons["left"] = buttons["up"];
	buttons["left"].SetPos( sf::Vector2f( 30, 250 ) );

	buttons["right"] = buttons["up"];
	buttons["right"].SetPos( sf::Vector2f( 30, 300 ) );

	buttons["fire"] = buttons["up"];
	buttons["fire"].SetPos( sf::Vector2f( 30, 350 ) );
}

void StateOptions::HandleInput()
{
	sf::Event event;
	sf::Vector2i m = sf::Mouse::getPosition( this->game->window );

	while( game->window.pollEvent( event ) )
	{
		// Close window
		if( event.type == sf::Event::Closed )
			game->window.close();

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
			}
		}

		if( event.type == sf::Event::MouseButtonPressed )
		{
			// Check if mouse was inside any of the gui buttons
			for( auto button : buttons )
			{
				if( button.second.hitbox.contains( sf::Vector2f( (float)m.x, (float)m.y ) ) )
				{
					awaitingRebind = true;
					bind = button.first;
				}
			}
		}

		if( event.type == sf::Event::MouseMoved )
		{
			// Check GUI highlights
			for( auto& button : buttons )
			{
				if( button.second.hitbox.contains( sf::Vector2f( (float)m.x, (float)m.y ) ) )
					button.second.SetHighlight( true );
				else
					button.second.SetHighlight( false );
			}
		}
	}
}

void StateOptions::Update(const float dt)
{
	if( awaitingRebind )
	{
		error = false;
		rebind.setString( "Press key to bind " + bind );
	}
	else
	{
		if(!error)
			rebind.setString( "Rebind keys:" );

		buttons["up"].SetText( "up:    " + game->inputManager.GetKeyName( "up" ) );
		buttons["down"].SetText( "down:  " + game->inputManager.GetKeyName( "down" ) );
		buttons["left"].SetText( "left:  " + game->inputManager.GetKeyName( "left" ) );
		buttons["right"].SetText( "right: " + game->inputManager.GetKeyName( "right" ) );
		buttons["fire"].SetText( "fire:  " + game->inputManager.GetKeyName( "fire" ) );
	}
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