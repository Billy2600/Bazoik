#include "entity_sword.h"

EntitySword::EntitySword(Directions direction, Entity* owner)
{
	clock.restart();

	this->owner = owner;
	lastDir = direction;
	SetPositionBasedOnDirection(direction);
	SetDimensionsBasedOnDirection(direction);

#ifdef _DEBUG
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setSize(sf::Vector2f(hitbox.width, hitbox.height));
#endif
}

void EntitySword::SetDimensionsBasedOnDirection(Directions direction)
{
	switch (direction)
	{
	case Directions::N:
	case Directions::S:
	case Directions::NW:
	case Directions::SW:
		hitbox.width = SWORD_WIDTH;
		hitbox.height = SWORD_HEIGHT;
		break;
	case Directions::W:
	case Directions::E:
	case Directions::NE:
	case Directions::SE:
		hitbox.width = SWORD_HEIGHT;
		hitbox.height = SWORD_WIDTH;
		break;
	}
}

void EntitySword::SetPositionBasedOnDirection(Directions direction)
{
	switch (direction)
	{
	case Directions::NE:
	case Directions::SE:
	case Directions::N:
		hitbox.left = owner->hitbox.left + (owner->hitbox.width / 2);
		hitbox.top = owner->hitbox.top - hitbox.height;
		break;
	case Directions::S:
		hitbox.left = owner->hitbox.left + (owner->hitbox.width / 2);
		hitbox.top = owner->hitbox.top + owner->hitbox.height;
		break;
	case Directions::NW:
	case Directions::SW:
	case Directions::W:
		hitbox.left = owner->hitbox.left - hitbox.width;
		hitbox.top = owner->hitbox.top + (owner->hitbox.height / 2);
		break;
	case Directions::E:
		hitbox.left = owner->hitbox.left + owner->hitbox.width;
		hitbox.top = owner->hitbox.top + (owner->hitbox.height / 2);
		break;
	}
}

void EntitySword::Think(const float dt)
{
	SetPositionBasedOnDirection(lastDir);
#ifdef _DEBUG
	shape.setPosition(hitbox.left, hitbox.top);
#endif 

	// Finish sword "swing"
	if (clock.getElapsedTime().asMilliseconds() >= SWORD_SWING_DELAY)
	{
		deleteMe = true;
	}
}

void EntitySword::Draw() const
{
#ifdef _DEBUG
	game->window.draw(shape);
#endif
}

void EntitySword::HandleCollision(Entity* other)
{

}

void EntitySword::Move(sf::Vector2f move, const float dt)
{
	hitbox.left = owner->hitbox.left;
	hitbox.top = owner->hitbox.top;
}