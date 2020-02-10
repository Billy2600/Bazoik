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
	std::map<char, DoorStates> doors; // The states in which to spawn doors, with single char indentifier (n, s, e, or w)

	void SpawnEntity(const std::string type, const sf::Vector2f pos);
	void SetDoorState(const char direction, const std::string strState);

public:
	Room();
	Room(const sf::Vector2i roomPos, EntityManager* entityManager, ErrorLog* log);
	void LoadRoomContents(); // Loads from XML file and creates entities
	void CreateWalls();
	void CreateDoors();
	void SetupRoom();
};

