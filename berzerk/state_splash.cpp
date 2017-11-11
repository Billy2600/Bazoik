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
	game->assetManager.LoadSound( "robot_die", "assets/robot_die.wav" );
	game->assetManager.LoadSound( "death", "assets/death.wav" );
	game->assetManager.LoadSound( "shoot", "assets/shoot.wav" );
}