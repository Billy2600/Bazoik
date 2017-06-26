#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include "asset_manager.h"
#include "input_manager.h"
#include "anim_manager.h"

#define GAME_NAME "Bazoik"
#define GAME_WIDTH 848
#define GAME_HEIGHT 480
#define START_LIVES 3
#define EXTRA_LIFE_SCORE 1000 // Extra life is awarded every x points
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
	unsigned int lives;

public:
	bool popped; // Flag for when state was popped
	sf::RenderWindow window;
	std::stack<GameState*> states;
	AssetManager assetManager;
	unsigned int score;
	InputManager inputManager;
	AnimManager animManager;
	int unsigned level; // Current level, certain behaviors will change based on this
	bool errorThrown; // Flag for thrown error
	sf::Text errorText;

	Game();
	void GameLoop(); // Runs every update
	void PopState(); // Pop state off stack
	void ResetState(); // Reset current state
	void SwitchState(GameState* newState); // Switch current state (will delete old one)
	unsigned int GetLives() const;
	void ResetLives();
	void RemoveLife(); // Remove one life
	void AddLife(); // Add one life
	void Draw(); // Runs every frame
	std::string GetConfigDir(); // Get directory for saving/loading files; includes trailing slash
	bool FileExists( std::string path ) const;
	void Close(); // Close game
	void ThrowError( const std::string error );
	~Game();
};