#pragma once
#include "entity.h"
#include "anim_manager.h"
#include "entity_door_floor.h"

#define DOOR_WIDTH 64
#define DOOR_HEIGHT 40

enum class DoorStates
{
	None,
	Closed,
	Locked,
	Open
};

class EntityDoor : public Entity
{
private:
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif
	DoorStates state; // What state is this door in?
	Directions direction; // What direction should door be facing
	AnimManager animManager;
	sf::Sprite sprite;
	sf::RectangleShape wallPiece; // Cover part of wall, so player won't appear over it
	sf::Vector2f position; // Need to store position seperately, as sprite and hitbox can be in different places
	bool doorOpened; // Door opened flag
	bool doorAlreadyOpenCheck; // So we can run DoorAlreadyOpened() only once

	void SetPositionRotationBasedOnDirection(const Directions direction);
	void LoadSprite();
	void OpenDoor();
	bool DoorAlreadyOpened();

public:
	EntityDoor(DoorStates initialState, Directions initialDirection);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision(Entity* other);
	DoorStates GetState() const;
};