#pragma once
#include "entity.h"

#define SWORD_HEIGHT 11
#define SWORD_WIDTH 3

class EntitySword : public Entity
{
private:
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif

	void SetDimensionsBasedOnDirection(Directions direction);

public:
	Entity* owner;

	EntitySword(Directions direction, Entity* owner);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void HandleCollision(Entity* other);
	virtual void Move(sf::Vector2f move, const float dt);
};