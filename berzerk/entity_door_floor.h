#pragma once
#include "entity.h"

// Entity for drawing the floor under the frame and player, for open doors
class EntityDoorFloor : public Entity
{
private:
	sf::Sprite sprite;
	AnimManager animManager;

public:
	EntityDoorFloor(sf::Vector2f pos, Directions direction, float rotation, Game* game);
	virtual void Draw() const;
	virtual void Think(const float dt);
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision(Entity* other);
};