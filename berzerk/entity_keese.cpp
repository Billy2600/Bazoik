#include "entity_keese.h"
#include "entity_sword.h"

EntityKeese::EntityKeese(const sf::Vector2f pos)
{
	hitbox.left = pos.x;
	hitbox.top = pos.y;
	hitbox.width = 32;
	hitbox.height = 20;
	moving = true;
	moveTimer.restart();

#ifdef _DEBUG
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
	shape.setSize(sf::Vector2f(hitbox.width, hitbox.height));
#endif
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
	if (moveTimer.getElapsedTime().asSeconds() >= KEESE_MOVE_TIME_LIMIT)
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
}

void EntityKeese::Draw() const
{
	game->window.draw(sprite);
#ifdef _DEBUG
	game->window.draw(shape);
#endif
}

void EntityKeese::HandleCollision(Entity* other)
{
	if (dynamic_cast<EntitySword*>(other) != NULL)
	{
		deleteMe = true;
	}
}

void EntityKeese::Move(sf::Vector2f move, const float dt)
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;

	sprite.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
#ifdef _DEBUG
	shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
#endif
}