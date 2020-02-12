#pragma once
#include "entity.h"

class EntityEnemy : public Entity
{
protected:
	sf::Vector2f playerPos; // All enemies might want to know where the player is

public:
	void SetPlayerPos(const sf::Vector2f playerPos)
	{
		this->playerPos = playerPos;
	}
};

