#include "state_titlescreen.h"
#include "state_gameplay.h"
#include "state_options.h"
#include "state_credits.h"

StateTitleScreen::StateTitleScreen(Game *game)
{
	this->game = game;

	AssetManager *assetManager = &this->game->assetManager;
	assetManager->LoadFont( "joystix", "assets/joystix monospace.ttf" );
	title.setFont( assetManager->GetFontRef( "joystix" ) );
#ifdef OLD_SFML
	title.setColor( sf::Color::Green );
#else
	title.setFillColor( sf::Color::Green );
#endif
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

	buttons["quit"] = buttons["start"];
	buttons["quit"].order = 2;
	buttons["quit"].SetText( "Quit" );
	buttons["quit"].SetPos( sf::Vector2f( 150, 350 ) );

	selectedButton = 0;
	playDemo = false;
	recordDemo = false;
	startLevel = 1;
	currentDemo = 0;
	clkAttractMode.restart();

	if (!txImage.loadFromFile( "assets/TitleScreen001.png" ))
		log.Write( "Could not load title screen image" );
	else
	{
		spImage.setTexture( txImage );
		spImage.setPosition( 0, 0 );
	}
}

void StateTitleScreen::Start()
{
	log.Write( "Title state started" );
	clkAttractMode.restart();
	playDemo = false;
	recordDemo = false;
	game->ResetLives();
	game->score = 0;

	if (game->music.getStatus() != sf::Music::Status::Playing)
	{
		if (!game->music.openFromFile("assets/Title_Music.wav"))
		{
			ErrorLog log;
			log.Write("Could not open music file Title_Music.wav");
		}
		else
		{
			game->music.play();
		}
	}
}

void StateTitleScreen::HandleInput()
{
	sf::Event event;
	sf::Vector2f m = game->window.mapPixelToCoords( sf::Mouse::getPosition( this->game->window ) );

	while( game->window.pollEvent( event ) )
	{
		// Reset timer on all input
		if ( event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyPressed || /*event.type == sf::Event::MouseMoved || */ joystickInput )
			clkAttractMode.restart();

		// Close window
		if( event.type == sf::Event::Closed )
		{
			game->Close();
			return;
		}

		if( event.type == sf::Event::KeyPressed )
		{
			if( event.key.code == sf::Keyboard::Key::Escape )
			{
				game->window.close();
			}
		}

		// Click on/select button
		for( auto button : buttons )
		{
			if( event.type == sf::Event::MouseButtonPressed && button.second.hitbox.contains( m ) 
				|| joystickInput && button.second.order == selectedButton && game->inputManager.TestKeyDown( "fire", event )
				|| button.second.order == selectedButton && ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Return ) )
			{
				// Perform action based on which button this is
				if( button.first == "start" )
				{
					game->music.stop(); // Stop attract mode music so that gameplay music will start
					StartGame();
				}
				else if( button.first == "options" )
				{
					this->game->states.push( new StateOptions( this->game ) );
				}
				else if ( button.first == "quit" )
				{
					game->Close();
					return;
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
		if( game->inputManager.TestKeyDown( "up", event )
			|| ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Up ) )
		{
			if( joystickInput )
			{
				joystickInput = true;
				
				selectedButton--;
				if ( selectedButton < 0 )
					selectedButton = buttons.size() - 1;
			}
			else
				joystickInput = true; // On first press, just turn on joystick functionality
		}

		if( game->inputManager.TestKeyDown( "down", event )
			|| ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Down ) )
		{
			if( joystickInput )
			{
				joystickInput = true;
				
				selectedButton++;
				if ( selectedButton >= (signed)buttons.size() )
					selectedButton = 0;
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
		
#ifdef _DEBUG
		// Change start level
		if ( event.key.code == sf::Keyboard::Key::F1 )
		{
			if ( startLevel > 1 )
				startLevel--;
			title.setString( "Start: " + std::to_string( startLevel ) );
		}
		else if ( event.key.code == sf::Keyboard::Key::F2 )
		{
			startLevel++;
			title.setString( "Start: " + std::to_string( startLevel ) );
		}

		// Record/play demos
		if( event.key.code == sf::Keyboard::Key::F10 )
		{
			playDemo = true;
			recordDemo = false;
			title.setString( "Play Demo" );
		}
		if (event.key.code == sf::Keyboard::Key::F11)
		{
			recordDemo = true;
			playDemo = false;
			title.setString("Record Demo");
		}
#endif
	}
}

void StateTitleScreen::Update(const float dt)
{
	if ( clkAttractMode.getElapsedTime().asMilliseconds() > ATTRACT_MODE_DELAY )
	{
		bool demoCheck = false;
		for ( int i = 0; i < NUM_DEMOS; i++ )
		{
			// No demos will play unless all are accounted for
			demoCheck = game->FileExists( "assets/demo" + std::to_string( i ) + ".xml" );
		}

		if ( demoCheck )
		{
			playDemo = true;
			recordDemo = false;
			StartGame();
			currentDemo++;
			if ( currentDemo >= NUM_DEMOS + 1 ) // One above the limit will show credits instead
				currentDemo = 0;
		}
	}
}

void StateTitleScreen::Draw() const
{
	game->window.draw( title );

	game->window.draw( spImage );

	for( auto button : buttons )
	{
		game->window.draw( button.second );
	}
}

void StateTitleScreen::StartGame()
{
	log.Write( "Switching to gameplay state" );
	game->ResetLives();
	game->level = startLevel;
	if( currentDemo >= NUM_DEMOS )
		this->game->states.push( new StateCredits( this->game) );
	else
		this->game->states.push( new StateGameplay( this->game, recordDemo, playDemo, "assets/demo" + std::to_string(currentDemo) + ".xml" ) );
}