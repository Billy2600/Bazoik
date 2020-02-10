#pragma once
#include <SFML/Graphics.hpp>
#include "entity_manager.h"
#include "entity_door.h"

class Room
{
private:
	sf::Vector2i roomPos; // Room position in level grid
	EntityManager* entityManager;

public:
	Room();
	Room(sf::Vector2i roomPos, EntityManager* entityManager);
	void CreateWalls();
	void CreateDoors();
	void SetupRoom();
};

