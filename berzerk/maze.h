#pragma once
#include <random>
#include <stack>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "game.h"
#include "entity_manager.h"
#include "entity_player.h"
#include "entity_robot.h"

#define MAZE_WIDTH 5 // Total maze width
#define MAZE_HEIGHT 3 // Total maze height
#define WALL_THICKNESS 10 // Thickness of walls

class Maze
{
private:
	// Map tile
	struct Tile
	{
		// Determines which sides have walls
		bool top;
		bool bottom;
		bool left;
		bool right;
		// Has this tile been visited or not
		// Should default to false
		bool visited;
	};

	const int tileWidth = GAME_WIDTH / MAZE_WIDTH;
	const int tileHeight = GAME_HEIGHT / MAZE_HEIGHT;

	Tile map[MAZE_WIDTH][MAZE_HEIGHT]; // Array of maze tiles
	std::stack<sf::Vector2u> stTiles = std::stack<sf::Vector2u>(); // Stack of tile coordinates, for backtracking
	unsigned int numVisited; // Number of tiles visited, halts loop when equal to size of array

	// Random number generator engine
	std::mt19937 rngEngine = std::mt19937((unsigned int)time(0)); 

	unsigned int x, y; // X and Y iterators
	bool done; // Level generation complete flag

	// Make sure the X/Y positions aren't outside map array
	// Will return true if it is in array
	bool CheckBounds(sf::Vector2u pos) const;
	// Mark tile visited
	void MarkVisited(sf::Vector2u pos);

public:
	// Constructor
	Maze();
	// Is level done generating?
	bool IsDone() const;
	// Clear map
	// Will reset map to beginning stage
	void ClearMap();
	// Create randomized Maze
	// Runs every frame
	void Generate();
	// Create gaps in outside walls so player can walk out of screen
	void CreateGaps();
	// Create level walls
	// Will automatically add wall entities, but positions and boundries will also be returend
	std::vector<sf::IntRect> CreateWalls(EntityManager &entityManager) const;
	// Load walls from external source (usually a demo)
	void LoadWalls( const std::vector<sf::IntRect> walls, EntityManager &entityManager );
	// Block last exit
	void BlockExit( EntityManager &entityManager, Directions lastMove ) const;
	// Spawn enemies
	// Will automatically add robot entities, but positions will also be returend
	std::vector<sf::Vector2f> SpawnEnemies( EntityManager &entityManager, const Directions lastMove, const RobotStats stats ); // Uses lastmove to avoid spawning enemies near player
	// Load enemies from external source (usually a demo)
	void LoadEnemies( const std::vector<sf::Vector2f> positions, EntityManager &entityManager, const RobotStats stats );
	// Get position player should start in
	sf::Vector2f GetPlayerStart( const Directions lastMove, EntityPlayer &player ) const;
};

