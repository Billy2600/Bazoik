#include "entity_keese.h"
#include "entity_sword.h"

EntityKeese::EntityKeese(const sf::Vector2f pos)
{
	hitbox.left = pos.x;
	hitbox.top = pos.y;
	hitbox.width = 20;
	hitbox.height = 20;
	moving = true;
	moveTimer.restart();
}

void EntityKeese::LoadSprite()
{
	if (sprite.getTexture() == NULL)
	{
		sprite.setTexture(game->assetManager.GetTextureRef("sprites"));
	}
}

void EntityKeese::Think(const float dt)
{
	LoadSprite();

	// Move for specified time, or pause for specified time
	auto elapsed = moveTimer.getElapsedTime().asSeconds();
	if (elapsed >= KEESE_MOVE_TIME_LIMIT)
	{
		moving = !moving;
		moveTimer.restart();
	}

	if (moving)
	{
		Move(GetMoveTowardsVec(playerPos, 15.f), dt);
		sprite.setTextureRect(animManager.Animate("keese_fly"));
	}
	else
	{
		sprite.setTextureRect(animManager.Animate("keese_still"));
	}

	sprite.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
}

void EntityKeese::Draw() const
{
	game->window.draw(sprite);
}

void EntityKeese::HandleCollision(Entity* other)
{
	if (dynamic_cast<EntitySword*>(other) != NULL)
	{
		deleteMe = true;
	}
}