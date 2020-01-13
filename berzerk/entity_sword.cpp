#include "entity_sword.h"

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


EntitySword::EntitySword(Directions direction, Entity* owner)
{
	this->owner = owner;
	hitbox.left = owner->hitbox.left;
	hitbox.top = owner->hitbox.top;
	SetDimensionsBasedOnDirection(direction);

#ifdef _DEBUG
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setSize(sf::Vector2f(hitbox.width, hitbox.height));
#endif
}

void EntitySword::Think(const float dt)
{
	hitbox.left = owner->hitbox.left;
	hitbox.top = owner->hitbox.top;
#ifdef _DEBUG
	shape.setPosition(hitbox.left, hitbox.top);
#endif 

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