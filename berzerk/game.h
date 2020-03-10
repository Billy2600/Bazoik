#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include "asset_manager.h"
#include "input_manager.h"
#include "anim_manager.h"

#define GAME_NAME "Bazoik"
#define GAME_WIDTH 480	
#define GAME_HEIGHT 320
// Account for OS differences
#ifdef _WIN32
#define P_ENV_VAR "USERPROFILE"
#define P_SEPERATOR "\\"
#else
#define P_ENV_VAR "HOME"
#define P_SEPERATOR "/"
#endif
#define ERROR_COLOR sf::Color( 255, 20, 147 ) // Look for this color to know something went wrong

class GameState;

class Game
{
private:
	sf::Int8 keys;

public:
	bool popped; // Flag for when state was popped
	sf::RenderWindow window;
	std::stack<GameState*> states;
	AssetManager assetManager;
	unsigned int score;
	InputManager inputManager;
	sf::Vector2i currentRoom;
	sf::Music music;

	Game();
	void GameLoop(); // Runs every update
	void PopState(); // Pop state off stack
	void ResetState(); // Reset current state
	void SwitchState(GameState* newState); // Switch current state (will delete old one)
	void ResetKeys();
	bool UseKey(); // Will return if you have any keys to use
	void AddKey();
	void Draw(); // Runs every frame
	static std::string GetConfigDir(); // Get directory for saving/loading files; includes trailing slash
	bool FileExists( const std::string path ) const;
	bool DirectoryExists( const std::string path ) const;
	void Close(); // Close game
	~Game();
};