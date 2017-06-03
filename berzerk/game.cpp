#include <algorithm>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "game.h"
#include "game_state.h"
#include "state_gameplay.h"
#include "state_titlescreen.h"
#include "state_highscore.h"

Game::Game()
{
	popped = false;

	inputManager.LoadFromFile( "keys.xml" );
	inputManager.game = this;
	lives = START_LIVES;

	if ( inputManager.fullscreen )
	{
		window.create( sf::VideoMode::getDesktopMode(), GAME_NAME, sf::Style::None );
		sf::View view = window.getDefaultView();
		view.setSize( sf::Vector2f( GAME_WIDTH, GAME_HEIGHT ) );
		view.setCenter( sf::Vector2f( GAME_WIDTH / 2, GAME_HEIGHT / 2 ) );
		window.setView( view );
	}
	else
	{
		window.create( sf::VideoMode( GAME_WIDTH, GAME_HEIGHT ), GAME_NAME, sf::Style::Default );
	}

	window.setFramerateLimit( 60 );

	// Load sprite texture
	assetManager.LoadTexture( "sprites", "assets/Bazoik_Character_Sprite_Sheet.png" );
	// Load animations
	animManager.LoadFromFile( "assets/animations.xml" );
}

void Game::GameLoop()
{
	sf::Clock clock;

	while( window.isOpen() )
	{
		if ( popped )
		{
			states.top()->Start();
			popped = false;
		}

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
	states.top()->Start();
}

void Game::SwitchState( GameState* newState )
{
	PopState();
	states.push( newState );
	states.top()->Start();
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

#ifdef _WIN32
	if( _dupenv_s( &buf, &sz, P_ENV_VAR ) == 0 && buf != nullptr )
	{
		path = std::string( buf );
		free( buf );
	}
#else
	if( secure_getenv( P_ENV_VAR ) != NULL )
	{
		path = std::string( secure_getenv( P_ENV_VAR) );
	}
#endif	

	// Check if directory exists, and if it doens't, create it
	std::string createPath = path + P_SEPERATOR + GAME_NAME;
#ifdef _WIN32
	DWORD attrib = GetFileAttributes( createPath.c_str() );
	if( attrib == INVALID_FILE_ATTRIBUTES && attrib & FILE_ATTRIBUTE_DIRECTORY )
	{
		CreateDirectory( createPath.c_str(), NULL );
	}
#else
	struct stat st = { 0 };

	if( stat( createPath.c_str(), &st ) == -1 )
	{
		mkdir( createPath.c_str(), 0700 );
	}
#endif
	path = path + P_SEPERATOR + GAME_NAME + P_SEPERATOR;
	return path;
}

bool Game::FileExists( std::string path ) const
{
#ifdef _WIN32
	DWORD attrib = GetFileAttributes( path.c_str() );
	return ( attrib != INVALID_FILE_ATTRIBUTES && !( attrib & FILE_ATTRIBUTE_DIRECTORY ) );
#else
	struct stat st = { 0 };
	return ( stat( path.c_str(), &st ) == 0 );
#endif
}

void Game::Close()
{
	while ( states.size() > 0 )
	{
		PopState();
	}
	window.close();
}

Game::~Game()
{
	
}