#include "entity_door_floor.h"

EntityDoorFloor::EntityDoorFloor(sf::Vector2f pos, Game* game)
{
	this->game = game;

	hitbox.left = -999; // Don't want any collision
	hitbox.top = -999;
	drawPriority = 1;
	sprite.setPosition(pos);
	sprite.setTextureRect(sf::IntRect(403, 59, 32, 32)); // TODO: Load this from animManager
}

void EntityDoorFloor::Draw() const
{
	game->window.draw(sprite);
}

void EntityDoorFloor::Think(const float dt)
{
	if (sprite.getTexture() == NULL)
	{
		sprite.setTexture(game->assetManager.GetTextureRef("sprites"));
	}
}

void EntityDoorFloor::Move(sf::Vector2f move, const float dt)
{
	sprite.setPosition(sprite.getPosition() + sf::Vector2f(move.x * dt, move.y * dt));
}