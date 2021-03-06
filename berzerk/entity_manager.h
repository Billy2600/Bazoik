#pragma once
#include <vector>
#include "game.h"
#include "entity_wall.h"
#include "entity_bullet.h"
#include "entity_otto.h"
#include "quadree.h"
#include "liang-barsky.h"

class EntityManager
{
private:
	std::vector<Entity*> entities;
	bool added; // Entity added flag
	Entity *player; // Keep track of player
	std::vector<EntityWall*> walls; // Keep track of walls
	AnimManager animManager;

public:
	Game *game;

	EntityManager();
	void Add( Entity* entity ); // Add new entity
	void CheckCollisions(); // Check all collisions
	void CheckLineOfSight(); // Perform line of sight checks
	void CheckDelete(); // Check if any need to be deleted and delete them
	void Think(const float dt); // All entities will think
	void Draw() const; // All entities will draw
	void MoveAllEntities( sf::Vector2f move, const float dt );
	int GetRobotCount() const; // How many robots are alive?
	~EntityManager();
};