#include "room.h"
#include "pugixml.hpp"
#include "entity_robot.h"

Room::Room()
{
	roomPos = sf::Vector2i(0, 0);
	entityManager = NULL;
	log = NULL;
}

Room::Room(const sf::Vector2i roomPos, EntityManager* entityManager, ErrorLog* log)
{
	this->roomPos = roomPos;
	this->entityManager = entityManager;
	this->log = log;
}

void Room::SpawnEntity(const std::string type, const sf::Vector2f pos)
{
	if (type == "robot")
	{
		entityManager->Add( new EntityRobot(pos, { false, true, 50, 3000, 5, ERROR_COLOR, 1, false }) );
	}
	else
	{
		log->Write("Invalid entity type specified: " + type);
	}
}

void Room::SetDoorState(const char direction, const std::string strState)
{
	if (strState == "none") doors.insert(std::make_pair(direction, DoorStates::None));
	else if (strState == "closed") doors.insert(std::make_pair(direction, DoorStates::Closed));
	else if (strState == "locked") doors.insert(std::make_pair(direction, DoorStates::Locked));
	else if (strState == "open") doors.insert(std::make_pair(direction, DoorStates::Open));
	else
	{
		doors.insert(std::make_pair(direction, DoorStates::None));
		log->Write("Invalid door state chosen: " + strState);
	}
}

void Room::LoadRoomContents()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("assets/rooms.xml");
	if (!result) // Error check
	{
		return;
	}

	try
	{
		pugi::xml_node roomNodes = doc.child("rooms");
		for (pugi::xml_node room : roomNodes.children("room"))
		{
			if (room.attribute("door_n") != NULL)
				SetDoorState('n', room.attribute("door_n").value());
			if (room.attribute("door_s") != NULL)
				SetDoorState('s', room.attribute("door_s").value());
			if (room.attribute("door_e") != NULL)
				SetDoorState('e', room.attribute("door_e").value());
			if (room.attribute("door_w") != NULL)
				SetDoorState('w', room.attribute("door_w").value());

			// Don't do anything if this isn't the right room position
			if( (room.attribute("x") != NULL && std::stoi(room.attribute("x").value()) == roomPos.x)
				&& (room.attribute("y") != NULL && std::stoi(room.attribute("y").value()) == roomPos.y) )
			{
				for (pugi::xml_node entity : room.children("entity"))
				{
					sf::Vector2f pos = sf::Vector2f(0, 0);
					if (entity.attribute("x") != NULL && entity.attribute("y") != NULL)
					{
						pos.x = std::stof( entity.attribute("x").value() );
						pos.y = std::stof( entity.attribute("y").value() );
					}
					if (entity.attribute("type") != NULL)
					{
						SpawnEntity(entity.attribute("type").value(), pos);
					}
				}

				// Early out, no two rooms should have the same position
				return;
			}
		}
	}
	catch (int e)
	{
		log->Write("Error while loading assets/rooms.xml");
	}
}

void Room::CreateWalls()
{
	const sf::Vector2f topLeft(48, 48);
	// Left
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x - WALL_THICKNESS, topLeft.y), sf::Vector2f(WALL_THICKNESS, WALL_HEIGHT), sf::Color::Red));
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x - WALL_THICKNESS, topLeft.y + WALL_HEIGHT + DOOR_WIDTH), sf::Vector2f(WALL_THICKNESS, WALL_HEIGHT), sf::Color::Red));
	// Top
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x, topLeft.y - WALL_THICKNESS), sf::Vector2f(WALL_WIDTH, WALL_THICKNESS), sf::Color::Red));
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x + WALL_WIDTH + DOOR_WIDTH, topLeft.y - WALL_THICKNESS), sf::Vector2f(WALL_WIDTH, WALL_THICKNESS), sf::Color::Red));
	// Right
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x + (WALL_WIDTH * 2) + DOOR_WIDTH, topLeft.y), sf::Vector2f(WALL_THICKNESS, WALL_HEIGHT), sf::Color::Red));
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x + (WALL_WIDTH * 2) + DOOR_WIDTH, topLeft.y + WALL_HEIGHT + DOOR_WIDTH), sf::Vector2f(WALL_THICKNESS, WALL_HEIGHT), sf::Color::Red));
	// Bottom
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x, (topLeft.y + WALL_HEIGHT * 2) + DOOR_WIDTH), sf::Vector2f(WALL_WIDTH, WALL_THICKNESS), sf::Color::Red));
	entityManager->Add(new EntityWall(sf::Vector2f(topLeft.x + WALL_WIDTH + DOOR_WIDTH, (topLeft.y + WALL_HEIGHT * 2) + DOOR_WIDTH), sf::Vector2f(WALL_WIDTH, WALL_THICKNESS), sf::Color::Red));
}

void Room::CreateDoors()
{
	entityManager->Add(new EntityDoor(doors['n'], Directions::N));
	entityManager->Add(new EntityDoor(doors['s'], Directions::S));
	entityManager->Add(new EntityDoor(doors['e'], Directions::E));
	entityManager->Add(new EntityDoor(doors['w'], Directions::W));
}

void Room::SetupRoom()
{
	LoadRoomContents();
	CreateWalls();
	CreateDoors();
}