#pragma once
#include "entity.h"

enum DoorStates
{
	None,
	Closed,
	Locked,
	Open
};

class EntityDoor : public Entity
{
public:
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif
private:
	EntityDoor();
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision(Entity* other);
};