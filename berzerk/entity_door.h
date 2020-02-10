#pragma once
#include "entity.h"
#include "anim_manager.h"

#define DOOR_WIDTH 64
#define DOOR_HEIGHT 40

enum DoorStates
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

	void SetPositionRotationBasedOnDirection(const Directions direction);
	void LoadSprite();

public:
	EntityDoor(DoorStates initialState, Directions initialDirection);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision(Entity* other);
	DoorStates GetState() const;
};