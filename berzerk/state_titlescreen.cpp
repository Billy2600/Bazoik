#include "state_titlescreen.h"
#include "state_gameplay.h"
#include "state_options.h"

StateTitleScreen::StateTitleScreen(Game *game)
{
	this->game = game;

	AssetManager *assetManager = &this->game->assetManager;
	assetManager->LoadFont( "joystix", "assets/joystix monospace.ttf" );
	title.setFont( assetManager->GetFontRef( "joystix" ) );
	title.setFillColor( sf::Color::Green );
	title.setCharacterSize( 90 );
	title.setPosition( sf::Vector2f(150,30) );
	title.setString( GAME_NAME );

	buttons["start"] = GuiButton( sf::Vector2f( 150, 200 ), sf::Vector2f( 270, 50 ), sf::Vector2f(10,3), "Start Game", assetManager->GetFontRef("joystix"), 0 );
	buttons["start"].SetColors( sf::Color::Black, sf::Color::Green, sf::Color::Green );
	buttons["start"].SetHighlightColors( sf::Color::Black, sf::Color::Red, sf::Color::Red );
	buttons["start"].SetHighlight( false );

	buttons["options"] = buttons["start"];
	buttons["options"].order = 1;
	buttons["options"].SetText( "Options" );
	buttons["options"].SetPos( sf::Vector2f( 150, 275 ) );

	selectedButton = 0;
}

void StateTitleScreen::LoadSounds()
{
	game->assetManager.LoadSound( "chicken_hi", "assets/voice/Bazoik_Chicken_Hi.wav" );
	game->assetManager.LoadSound( "chicken_low", "assets/voice/Bazoik_Chicken_Low.wav" );
	game->assetManager.LoadSound( "chicken_mid", "assets/voice/Bazoik_Chicken_Mid.wav" );
	game->assetManager.LoadSound( "humanoid_hi", "assets/voice/Bazoik_get_the_Humanoid_Hi.wav" );
	game->assetManager.LoadSound( "humanoid_low", "assets/voice/Bazoik_get_the_Humanoid_Low.wav" );
	game->assetManager.LoadSound( "humanoid_mid", "assets/voice/Bazoik_get_the_Humanoid_Mid.wav" );
	game->assetManager.LoadSound( "intruder_hi", "assets/voice/Bazoik_get_the_Intruder_Hi.wav" );
	game->assetManager.LoadSound( "intruder_low", "assets/voice/Bazoik_get_the_Intruder_Low.wav" );
	game->assetManager.LoadSound( "intruder_mid", "assets/voice/Bazoik_get_the_Intruder_Mid.wav" );
	game->assetManager.LoadSound( "got_chicken_hi", "assets/voice/Bazoik_Got_the_Chicken_Hi.wav" );
	game->assetManager.LoadSound( "got_chicken_low", "assets/voice/Bazoik_Got_the_Chicken_Low.wav" );
	game->assetManager.LoadSound( "got_chicken_mid", "assets/voice/Bazoik_Got_the_Chicken_Mid.wav" );
	game->assetManager.LoadSound( "got_humanoid_hi", "assets/voice/Bazoik_Got_the_Humanoid_Hi.wav" );
	game->assetManager.LoadSound( "got_humanoid_low", "assets/voice/Bazoik_Got_the_Humanoid_Low.wav" );
	game->assetManager.LoadSound( "got_humanoid_mid", "assets/voice/Bazoik_Got_the_Humanoid_Mid.wav" );
	game->assetManager.LoadSound( "got_intruder_hi", "assets/voice/Bazoik_Got_the_Intruder_Hi.wav" );
	game->assetManager.LoadSound( "got_intruder_low", "assets/voice/Bazoik_Got_the_Intruder_Low.wav" );
	game->assetManager.LoadSound( "got_intruder_mid", "assets/voice/Bazoik_Got_the_Intruder_Mid.wav" );
	game->assetManager.LoadSound( "intruder_alert_hi", "assets/voice/Bazoik_Intruder_alert_Hi.wav" );
	game->assetManager.LoadSound( "intruder_alert_low", "assets/voice/Bazoik_Intruder_alert_low.wav" );
	game->assetManager.LoadSound( "intruder_alert_mid", "assets/voice/Bazoik_Intruder_alert_Mid.wav" );
	game->assetManager.LoadSound( "must_not_escape_hi", "assets/voice/Bazoik_The_Humanoid_Must_Not_Escape_Hi.wav" );
	game->assetManager.LoadSound( "must_not_escape_low", "assets/voice/Bazoik_The_Humanoid_Must_Not_Escape_Low.wav" );
	game->assetManager.LoadSound( "must_not_escape_mid", "assets/voice/Bazoik_The_Humanoid_Must_Not_Escape_Mid.wav" );
}

void StateTitleScreen::HandleInput()
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
			if( event.key.code == sf::Keyboard::Key::Return )
			{
				StartGame();
			}
			if( event.key.code == sf::Keyboard::Key::Escape )
			{
				game->window.close();
			}
		}

		// Click on/select button
		if( event.type == sf::Event::MouseButtonPressed || game->inputManager.TestKeyDown( "fire", event ) )
		{
			// Check if mouse was inside any of the gui buttons
			for( auto button : buttons )
			{
				if( button.second.hitbox.contains( sf::Vector2f( (float)m.x, (float)m.y ) ) || ( joystickInput && button.second.order == selectedButton ) )
				{
					// Perform action based on which button this is
					if( button.first == "start" )
					{
						StartGame();
					}
					else if( button.first == "options" )
					{
						this->game->states.push( new StateOptions( this->game ) );
					}
				}
			}
		}

		if( event.type == sf::Event::MouseMoved )
		{
			joystickInput = false;
			// Check GUI highlights
			for( auto& button : buttons )
			{
				if( button.second.hitbox.contains( sf::Vector2f( (float)m.x, (float)m.y ) ) )
					button.second.SetHighlight( true );
				else
				{
					if( !joystickInput || button.second.order != selectedButton )
						button.second.SetHighlight( false );
				}
			}
		}

		// Interact with buttons with joystick
		if( game->inputManager.TestKeyDown( "up", event ) )
		{
			if( joystickInput )
			{
				joystickInput = true;
				selectedButton = 0;
			}
			else
				joystickInput = true; // On first press, just turn on joystick functionality
		}

		if( game->inputManager.TestKeyDown( "down", event ) )
		{
			if( joystickInput )
			{
				joystickInput = true;
				selectedButton = 1;
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

void StateTitleScreen::Update(const float dt)
{
	LoadSounds();
	game->ResetLives();
	game->level = 1;
}

void StateTitleScreen::Draw() const
{
	game->window.draw( title );

	for( auto button : buttons )
	{
		game->window.draw( button.second );
	}
}

void StateTitleScreen::StartGame()
{
	this->game->states.push( new StateGameplay( this->game ) );
}