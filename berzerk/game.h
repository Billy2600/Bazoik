#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include "asset_manager.h"
#include "input_manager.h"
#include "anim_manager.h"

#define GAME_NAME "Zelda Clone"
#define GAME_WIDTH 480	
#define GAME_HEIGHT 320
#define MAX_HP 8
#define MAX_ROOM_X 7
#define MAX_ROOM_Y 6
// Account for OS differences
#ifdef _WIN32
#define P_ENV_VAR "USERPROFILE"
#define P_SEPERATOR "\\"
#else
#define P_ENV_VAR "HOME"
#define P_SEPERATOR "/"
#endif
#define ERROR_COLOR sf::Color( 255, 20, 147 ) // Look for this color to know something went wrong

// Directions
enum class Directions { N, NE, E, SE, S, SW, W, NW };

class GameState;

class Game
{
private:
	sf::Int8 keys;
	sf::Int8 hitPoints; // 1 heart = 2 hp. Here to prevent permission issues if it was part of the player entity
	bool mercyInvicibility; // This is here so we can check for it beforeremoving hit points.

public:
	bool popped; // Flag for when state was popped
	sf::RenderWindow window;
	std::stack<GameState*> states;
	AssetManager assetManager;
	InputManager inputManager;
	sf::Vector2i currentRoom;
	bool visitedRooms[MAX_ROOM_X][MAX_ROOM_Y]; // Which rooms have we visited so far?
	sf::Music music;

	std::vector<sf::Vector2i> keysCollected;
	std::vector< std::pair<sf::Vector2i, Directions> > openedDoors;

	Game();
	void GameLoop(); // Runs every update
	void PopState(); // Pop state off stack
	void ResetState(); // Reset current state
	void SwitchState(GameState* newState); // Switch current state (will delete old one)
	sf::Int8 GetHitPoints() const;
	void ResetHitPoints();
	void SubtractHitPoints(const sf::Int8 subtract);
	void AddHitPoints(const sf::Int8 add);
	void SetMercyInvicibility(const bool newValue);
	bool GetMercyInvicibility() const;
	void ResetKeys();
	bool UseKey(); // Will return if you have any keys to use
	void AddKey();
	sf::Int8 GetKeys() const;
	bool GetRoomVisited(int x, int y) const;
	void SetRoomVisited(int x, int y);
	void ResetVisitedRooms();
	void ResetDoors();
	void Draw(); // Runs every frame
	static std::string GetConfigDir(); // Get directory for saving/loading files; includes trailing slash
	bool FileExists( const std::string path ) const;
	bool DirectoryExists( const std::string path ) const;
	void Close(); // Close game
	~Game();
};