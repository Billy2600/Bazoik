#include "state_splash.h"
#include "state_titlescreen.h"

StateSplash::StateSplash( Game *game )
{
	this->game = game;
	graphicShown = false;

	ErrorLog log;
	log.DeleteOld();
}

void StateSplash::Start()
{
	log.Write( "Splash state started" );
}

void StateSplash::HandleInput()
{
	// Wish I could let you skip this splash screen, but we gotta load shit
	sf::Event event;
	while ( game->window.pollEvent( event ) )
	{
		if ( event.type == sf::Event::Closed )
		{
			game->Close();
			return;
		}
	}
}

void StateSplash::Update( const float dt )
{
	if( graphic.getTexture() == NULL )
	{
		log.Write( "Loading splash image" );
		tex.loadFromFile( "assets/mmdclogo.png" );
		graphic.setTexture( tex );
	}

	if( graphicShown ) // Don't jump until we've shown the graphic
	{
		LoadSounds();
		game->SwitchState( new StateTitleScreen( game ) );
	}

	graphicShown = true;
}

void StateSplash::Draw() const
{
	game->window.draw( graphic );
}

void StateSplash::LoadSounds()
{
	log.Write( "Loading sounds" );
	game->assetManager.LoadSound( "robot_die", "assets/robot_die.wav" );
	game->assetManager.LoadSound( "death", "assets/death.wav" );
	game->assetManager.LoadSound( "shoot", "assets/shoot.wav" );
}