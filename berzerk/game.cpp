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

Game::Game()
{
	if ( !DirectoryExists( "assets" ) )
	{
		ErrorLog log;
		log.Write( "Assets not found!" );
#ifdef _WIN32
		MessageBox( 0, "Game assets not found!", "Error", MB_OK | MB_ICONERROR );
#endif
		return;
	}

	popped = false;

	inputManager.LoadFromFile( "keys.xml" );
	inputManager.game = this;

	if ( inputManager.fullscreen )
	{
		window.create( sf::VideoMode::getDesktopMode(), GAME_NAME, sf::Style::None );
	}
	else
	{
		window.create( sf::VideoMode( GAME_WIDTH * 2, GAME_HEIGHT * 2 ), GAME_NAME, sf::Style::Default );
	}

	sf::View view = window.getDefaultView();
	view.setSize(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT));
	view.setCenter(sf::Vector2f(GAME_WIDTH / 2, GAME_HEIGHT / 2));
	window.setView(view);

	window.setFramerateLimit( 60 );
	sf::Image icon;
	icon.loadFromFile("assets/ICON.png");
	window.setIcon(792, 528, icon.getPixelsPtr());

	// Load sprite texture
	assetManager.LoadTexture( "sprites", "assets/sprites.png" );
	assetManager.LoadTexture( "background", "assets/background.png" );
	// Load animations
	AnimManager animManager;
	animManager.LoadFromFile( "assets/animations.xml" );

	music.setLoop(true); // Music will always loop
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

sf::Int8 Game::GetHitPoints() const
{
	return hitPoints;
}

void Game::ResetHitPoints()
{
	hitPoints = MAX_HP;
}

void Game::SubtractHitPoints(sf::Int8 subtract)
{
	hitPoints -= subtract;
}

void Game::AddHitPoints(sf::Int8 add)
{
	hitPoints += add;
}

void Game::ResetKeys()
{
	keys = 0;
}

sf::Int8 Game::GetKeys() const
{
	return keys;
}

bool Game::GetRoomVisited(int x, int y) const
{
	if (x >= MAX_ROOM_X || y >= MAX_ROOM_Y)
		return false;

	return visitedRooms[x][y];
}

void Game::SetRoomVisited(int x, int y)
{
	if (x >= MAX_ROOM_X || y >= MAX_ROOM_Y)
		return;

	visitedRooms[x][y] = true;
}

void Game::ResetVisitedRooms()
{
	for (int x = 0; x < MAX_ROOM_X; x++)
	{
		for (int y = 0; y < MAX_ROOM_Y; y++)
		{
			visitedRooms[x][y] = false;
		}
	}
}

bool Game::UseKey()
{
	if (keys > 0)
	{
		keys--;
		return true;
	}
	else
	{
		return false;
	}
}

void Game::AddKey()
{
	keys++;
	keysCollected.push_back(currentRoom);
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

bool Game::FileExists( const std::string path ) const
{
#ifdef _WIN32
	DWORD attrib = GetFileAttributes( path.c_str() );
	return ( attrib != INVALID_FILE_ATTRIBUTES && !( attrib & FILE_ATTRIBUTE_DIRECTORY ) );
#else
	struct stat st = { 0 };
	return ( stat( path.c_str(), &st ) == 0 );
#endif
}

bool Game::DirectoryExists( const std::string path ) const
{
#ifdef _WIN32
	DWORD attrib = GetFileAttributes( path.c_str() );
	return ( attrib != INVALID_FILE_ATTRIBUTES && ( attrib & FILE_ATTRIBUTE_DIRECTORY ) );
#else
	struct stat st = { 0 };
	return ( stat( path.c_str(), &st ) == 0 && S_ISDIR( st.st_mode ) );
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
	music.stop();
}