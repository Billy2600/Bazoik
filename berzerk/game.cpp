#include <algorithm>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "game.h"
#include "game_state.h"
#include "state_gameplay.h"
#include "state_titlescreen.h"
#include "state_highscore.h"

Game::Game()
{
	window.create( sf::VideoMode( GAME_WIDTH, GAME_HEIGHT ), GAME_NAME );
	window.setFramerateLimit( 60 );
	popped = false;

	inputManager.LoadFromFile( "keys.xml" );
	inputManager.game = this;
	lives = START_LIVES;

	// Load sprite texture
	assetManager.LoadTexture( "sprites", "assets/sprites.png" );
	// Load animations
	animManager.LoadFromFile( "assets/animations.xml" );
}

void Game::GameLoop()
{
	sf::Clock clock;

	while( window.isOpen() )
	{
		if( popped )
			popped = false;

		// Skip if stack is empty
		if( states.empty() )
		{
			continue;
		}

		// Get input
		states.top()->HandleInput();
		if( popped )
			continue;

		states.top()->Update( clock.restart().asSeconds() );

		// Draw
		Draw();
	}

	// Clear out states
	while( !states.empty() )
	{
		PopState();
	}
}

void Game::ResetState()
{
	if( dynamic_cast<StateGameplay*>( states.top() ) != NULL )
	{
		SwitchState( new StateGameplay( this ) );
	}
}

void Game::SwitchState( GameState* newState )
{
	PopState();
	states.push( newState );
}

unsigned int Game::GetLives() const
{
	return lives;
}

void Game::ResetLives()
{
	lives = START_LIVES;
}

void Game::RemoveLife()
{
	lives--;
}

void Game::AddLife()
{
	lives++;
}

void Game::PopState()
{
	if( states.top() != NULL )
	{
		delete states.top();
		states.pop();
		popped = true;
	}
}

void Game::Draw()
{
	window.clear();
	states.top()->Draw();
	window.display();
}

std::string Game::GetConfigDir()
{
	std::string path;
	char *buf = nullptr;
	size_t sz = 0;

	if( _dupenv_s( &buf, &sz, P_ENV_VAR ) == 0 && buf != nullptr )
	{
		path = std::string( buf );
		free( buf );
	}

#ifdef _WIN32
	std::string createPath = path + P_SEPERATOR + GAME_NAME;
	// Check if directory exists, and if it doens't, create it
	DWORD attrib = GetFileAttributes( createPath.c_str() );
	if( attrib == INVALID_FILE_ATTRIBUTES && attrib & FILE_ATTRIBUTE_DIRECTORY )
	{
		CreateDirectory( createPath.c_str(), NULL );
	}
#endif
	path = path + P_SEPERATOR + GAME_NAME + P_SEPERATOR;
	return path;
}

Game::~Game()
{
	
}