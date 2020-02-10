#include "room.h"

Room::Room()
{
	roomPos = sf::Vector2i(0, 0);
	entityManager = NULL;
}

Room::Room(sf::Vector2i roomPos, EntityManager* entityManager)
{
	this->roomPos = roomPos;
	this->entityManager = entityManager;
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
	entityManager->Add(new EntityDoor(DoorStates::Closed, Directions::N));
	entityManager->Add(new EntityDoor(DoorStates::Closed, Directions::S));
	entityManager->Add(new EntityDoor(DoorStates::Closed, Directions::E));
	entityManager->Add(new EntityDoor(DoorStates::Closed, Directions::W));
}

void Room::SetupRoom()
{
	CreateWalls();
	CreateDoors();
}