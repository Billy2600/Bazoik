#pragma once
#include "entity.h"

// Entity for drawing the floor under the frame and player, for open doors
class EntityDoorFloor : public Entity
{
private:
	sf::Sprite sprite;

public:
	EntityDoorFloor(sf::Vector2f pos, Game* game);
	virtual void Draw() const;
	virtual void Think(const float dt);

	// Don't really need these, but compiler will throw a fit if we don't add them
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision(Entity* other) {}
};