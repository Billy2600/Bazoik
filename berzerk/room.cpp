#include "room.h"
#include "pugixml.hpp"
#include "entity_robot.h"
#include "entity_keese.h"
#include "entity_block.h"
#include "entity_floor.h"

Room::Room()
{
	roomCoord = sf::Vector2i(0, 0);
	entityManager = NULL;
}

Room::Room(const sf::Vector2i roomPos, EntityManager* entityManager)
{
	this->roomCoord = roomPos;
	this->entityManager = entityManager;
}

void Room::SpawnEntity(const std::string type, const sf::Vector2f pos)
{
	if (type == "robot")
	{
		entityManager->Add( new EntityRobot(pos, { false, true, 50, 3000, 5, ERROR_COLOR, 1, false }) );
	}
	else if (type == "keese")
	{
		entityManager->Add(new EntityKeese(pos));
	}
	else if (type == "block")
	{
		entityManager->Add(new EntityBlock(pos));
	}
	else if (type == "block_inverted")
	{
		entityManager->Add(new EntityBlock(pos, BlockTypes::Inverted));
	}
	else if (type == "water")
	{
		entityManager->Add(new EntityBlock(pos, BlockTypes::Water));
	}
	else if (type == "hole")
	{
		entityManager->Add(new EntityBlock(pos, BlockTypes::Hole));
	}
	else if (type == "face_right_light")
	{
		entityManager->Add(new EntityBlock(pos, BlockTypes::FaceRightLight));
	}
	else if (type == "face_right_dark")
	{
		entityManager->Add(new EntityBlock(pos, BlockTypes::FaceRightDark));
	}
	else if (type == "face_left_light")
	{
		entityManager->Add(new EntityBlock(pos, BlockTypes::FaceLeftLight));
	}
	else if (type == "face_left_dark")
	{
		entityManager->Add(new EntityBlock(pos, BlockTypes::FaceLeftDark));
	}
	else if (type == "gravel_1")
	{
		entityManager->Add(new EntityFloor(pos, FloorTypes::GravelOne));
	}
	else if (type == "gravel_2")
	{
		entityManager->Add(new EntityFloor(pos, FloorTypes::GravelTwo));
	}
	else if (type == "gravel_3")
	{
		entityManager->Add(new EntityFloor(pos, FloorTypes::GravelThree));
	}
	else
	{
		log.Write("Invalid entity type specified: " + type);
	}
}

DoorStates Room::GetDoorStateFromString(const std::string strState)
{
	if (strState == "none") return DoorStates::None;
	else if (strState == "closed") return DoorStates::Closed;
	else if (strState == "locked") return DoorStates::Locked;
	else if (strState == "open") return DoorStates::Open;
	else
	{
		ErrorLog log;
		log.Write("Invalid door state chosen: " + strState);
		return DoorStates::None;
	}
}

std::string Room::GetDoorStateStringFromState(const DoorStates state)
{
	switch (state)
	{
		case DoorStates::Closed:
			return "closed";
		case DoorStates::Locked:
			return "locked";
		case DoorStates::Open:
			return "open";
		case DoorStates::None:
		default:
			return "none";
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
			// Don't do anything if this isn't the right room position
			if( (room.attribute("x") != NULL && std::stoi(room.attribute("x").value()) == roomCoord.x)
				&& (room.attribute("y") != NULL && std::stoi(room.attribute("y").value()) == roomCoord.y) )
			{

				if (room.attribute("door_n") != NULL)
					doors.insert(std::make_pair(Directions::N, GetDoorStateFromString(room.attribute("door_n").value())));
				if (room.attribute("door_s") != NULL)
					doors.insert(std::make_pair(Directions::S, GetDoorStateFromString(room.attribute("door_s").value())));
				if (room.attribute("door_e") != NULL)
					doors.insert(std::make_pair(Directions::E, GetDoorStateFromString(room.attribute("door_e").value())));
				if (room.attribute("door_w") != NULL)
					doors.insert(std::make_pair(Directions::W, GetDoorStateFromString(room.attribute("door_w").value())));

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
		log.Write("Error while loading assets/rooms.xml");
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
	entityManager->Add(new EntityDoor(doors[Directions::N], Directions::N));
	entityManager->Add(new EntityDoor(doors[Directions::S], Directions::S));
	entityManager->Add(new EntityDoor(doors[Directions::E], Directions::E));
	entityManager->Add(new EntityDoor(doors[Directions::W], Directions::W));
}

void Room::SetupRoom()
{
	LoadRoomContents();
	CreateWalls();
	CreateDoors();
}