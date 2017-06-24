#include "pause_menu.h"
#include "state_options.h"

PauseMenu::PauseMenu()
{
	open = false;
	quit = false;
	selectedButton = 0;
}

void PauseMenu::SetGame( Game *game )
{
	this->game = game;

	AssetManager *assetManager = &this->game->assetManager;
	title.setFont( assetManager->GetFontRef( "joystix" ) );
#ifdef OLD_SFML
	title.setColor( sf::Color::Green );
#else
	title.setFillColor( sf::Color::Green );
#endif
	title.setCharacterSize( 60 );
	title.setPosition( sf::Vector2f( 250, 30 ) );
	title.setString( "Pause" );

	buttons["resume"] = GuiButton( sf::Vector2f( 250, 150 ), sf::Vector2f( 270, 50 ), sf::Vector2f( 10, 3 ), "Resume", assetManager->GetFontRef( "joystix" ), 0 );
	buttons["resume"].SetColors( sf::Color::Black, sf::Color::Green, sf::Color::Green );
	buttons["resume"].SetHighlightColors( sf::Color::Black, sf::Color::Red, sf::Color::Red );
	buttons["resume"].SetHighlight( false );

	buttons["options"] = buttons["resume"];
	buttons["options"].order = 1;
	buttons["options"].SetText( "Options" );
	buttons["options"].SetPos( sf::Vector2f( 250, 215 ) );

	buttons["quit"] = buttons["resume"];
	buttons["quit"].order = 1;
	buttons["quit"].SetText( "Exit" );
	buttons["quit"].SetPos( sf::Vector2f( 250, 280 ) );

	overlay.setPosition( 0, 0 );
	overlay.setSize( sf::Vector2f(game->window.getSize().x, game->window.getSize().y ) );
#ifdef OLD_SFML
	overlay.setColor( sf::Color( 0, 0, 0, 150 ) );
#else
	overlay.setFillColor( sf::Color( 0, 0, 0, 150 ) );
#endif
}

void PauseMenu::HandleInput()
{
	sf::Event event;
	sf::Vector2f m = game->window.mapPixelToCoords( sf::Mouse::getPosition( this->game->window ) );

	while ( game->window.pollEvent( event ) )
	{
		// Close window
		if ( event.type == sf::Event::Closed )
		{
			game->Close();
			return;
		}

		if ( event.type == sf::Event::KeyPressed )
		{
			if ( event.key.code == sf::Keyboard::Key::Escape )
			{
				open = false;
			}
		}

		// Click on/select button
		for ( auto button : buttons )
		{
			if ( event.type == sf::Event::MouseButtonPressed && button.second.hitbox.contains( m )
				|| joystickInput && button.second.order == selectedButton && game->inputManager.TestKeyDown( "fire", event )
				|| button.second.order == selectedButton && ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Return ) )
			{
				// Perform action based on which button this is
				if ( button.first == "resume" )
				{
					open = false;
				}
				else if ( button.first == "options" )
				{
					this->game->states.push( new StateOptions( this->game ) );
				}
				else if ( button.first == "quit" )
				{
					quit = true;
					return;
				}
			}
		}

		if ( event.type == sf::Event::MouseMoved )
		{
			joystickInput = false;
			// Check GUI highlights
			for ( auto& button : buttons )
			{
				if ( button.second.hitbox.contains( sf::Vector2f( (float)m.x, (float)m.y ) ) )
					button.second.SetHighlight( true );
				else
				{
					if ( !joystickInput || button.second.order != selectedButton )
						button.second.SetHighlight( false );
				}
			}
		}

		// Interact with buttons with joystick
		if ( game->inputManager.TestKeyDown( "up", event )
			|| ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Up ) )
		{
			if ( joystickInput )
			{
				joystickInput = true;

				selectedButton--;
				if ( selectedButton < 0 )
					selectedButton = buttons.size() - 1;
			}
			else
				joystickInput = true; // On first press, just turn on joystick functionality
		}

		if ( game->inputManager.TestKeyDown( "down", event )
			|| ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Down ) )
		{
			if ( joystickInput )
			{
				joystickInput = true;

				selectedButton++;
				if ( selectedButton >= buttons.size() )
					selectedButton = 0;
			}
			else
				joystickInput = true;
		}

		// Highlight buttons
		if ( joystickInput )
		{
			for ( auto& button : buttons )
			{
				if ( button.second.order == selectedButton )
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

void PauseMenu::Draw() const
{
	game->window.draw( overlay );
	game->window.draw( title );

	for ( auto button : buttons )
	{
		game->window.draw( button.second );
	}
}

PauseMenu::~PauseMenu()
{

}
