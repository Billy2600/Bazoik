#include "entity_enemy.h"

void EntityEnemy::SetPlayerPos(const sf::Vector2f playerPos)
{
	this->playerPos = playerPos;
}

void EntityEnemy::DamagePlayer(EntityPlayer* player, const sf::Int8 damage)
{
	if (!game->GetMercyInvicibility())
	{
		game->SubtractHitPoints(damage);
		player->StartMercyInvincibility();
	}
}