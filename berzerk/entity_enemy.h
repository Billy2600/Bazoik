#pragma once
#include "entity.h"
#include "entity_player.h"

class EntityEnemy : public Entity
{
protected:
	sf::Vector2f playerPos; // All enemies might want to know where the player is

public:
	bool seePlayer;

	void SetPlayerPos(const sf::Vector2f playerPos);

	void DamagePlayer(EntityPlayer* player, const sf::Int8 damage);
};

