#pragma once
#include <SFML/Graphics.hpp>
#include "entity_manager.h"
#include "entity_door.h"
#include "error_log.h"

class Room
{
private:
	sf::Vector2i roomPos; // Room position in level grid
	EntityManager* entityManager;
	ErrorLog* log;

	void SpawnEntity(std::string type, sf::Vector2f pos);

public:
	Room();
	Room(sf::Vector2i roomPos, EntityManager* entityManager, ErrorLog* log);
	void LoadRoomContents(); // Loads from XML file and creates entities
	void CreateWalls();
	void CreateDoors();
	void SetupRoom();
};

