#pragma once
#include "entity.h"

class EntityKey : public Entity
{
private:
	sf::Sprite sprite;
	AnimManager animManager;

public:
	EntityKey(sf::Vector2f pos);
	virtual void Think(const float dt) = 0;
	virtual void Draw() const = 0;
	virtual void HandleCollision(Entity* other) = 0;
	virtual void Move(sf::Vector2f move, const float dt);
};