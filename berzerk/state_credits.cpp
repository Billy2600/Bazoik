#include "state_credits.h"
#include "state_titlescreen.h"

StateCredits::StateCredits( Game *game )
{
	this->game = game;
	clock.restart();

	AssetManager *assetManager = &this->game->assetManager;
	title.setFont( assetManager->GetFontRef( "joystix" ) );
#ifdef OLD_SFML
	title.setColor( sf::Color::Green );
#else
	title.setFillColor( sf::Color::Green );
#endif
	title.setCharacterSize( 90 );
	title.setPosition( sf::Vector2f( 3, 3 ) );
	title.setString( "CREDITS" );

	txCredits = title;
	txCredits.setCharacterSize( 30 );
	txCredits.setPosition( sf::Vector2f( 3, 90 ) );
	txCredits.setString( "\nProgramming . Billy McPherson\n\nArt . . . . . Aaron Martinez\n\nVoice . . . . Argus Swift" );
}

void StateCredits::Start()
{

}

void StateCredits::HandleInput()
{
	
}

void StateCredits::Update( const float dt )
{
	if ( clock.getElapsedTime().asMilliseconds() > ATTRACT_MODE_DELAY )
	{
		game->PopState(); // Return to title screen
	}
}

void StateCredits::Draw() const
{
	game->window.draw( title );
	game->window.draw( txCredits );
}