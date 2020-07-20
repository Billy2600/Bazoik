#pragma once
#include <SFML/Graphics.hpp>
#include "entity_manager.h"
#include "entity_door.h"
#include "error_log.h"

#define START_ROOM sf::Vector2i(3,5)

class Room
{
private:
	sf::Vector2i roomCoord; // Room position in level grid
	EntityManager* entityManager;
	ErrorLog log;
	std::map<Directions, DoorStates> doors; // The states in which to spawn doors, with single char indentifier (n, s, e, or w)
	bool textRoom;

	void SpawnEntity(const std::string type, const sf::Vector2f pos);

public:
	Room();
	Room(const sf::Vector2i roomPos, EntityManager* entityManager);
	void LoadRoomContents(); // Loads from XML file and creates entities
	void CreateWalls();
	void CreateDoors();
	void SetupRoom();
	static DoorStates GetDoorStateFromString(const std::string strState);
	static std::string GetDoorStateStringFromState(const DoorStates state);
	bool GetTextRoom() const;
};

