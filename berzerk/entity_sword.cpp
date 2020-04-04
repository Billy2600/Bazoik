#include "entity_sword.h"

EntitySword::EntitySword(Directions direction, Entity* owner)
{
	clock.restart();

	this->owner = owner;
	lastDir = direction;
	SetDimensionsBasedOnDirection(direction);
	SetPositionBasedOnDirection(direction);
	SetSpriteRotationAndPositionBasedOnDirection(direction);

#ifdef _DEBUG
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setSize(sf::Vector2f(hitbox.width, hitbox.height));
#endif
}

void EntitySword::LoadSprite()
{
	// Load texture if we need to
	if (sprite.getTexture() == NULL)
	{
		sprite.setTexture(game->assetManager.GetTextureRef("sprites"));
		sprite.setTextureRect(animManager.Animate("sword"));
	}
}

void EntitySword::SetDimensionsBasedOnDirection(const Directions direction)
{
	switch (direction)
	{
	// Above/below the player
	case Directions::NE:
	case Directions::NW:
	case Directions::N:
	case Directions::S:
		hitbox.width = SWORD_WIDTH;
		hitbox.height = SWORD_HEIGHT;
		break;
	// Left/right of the player
	case Directions::SE:
	case Directions::SW:
	case Directions::W:
	case Directions::E:
		hitbox.width = SWORD_HEIGHT;
		hitbox.height = SWORD_WIDTH;
		break;
	}
}

void EntitySword::SetPositionBasedOnDirection(const Directions direction)
{
	switch (direction)
	{
	// Above the player
	case Directions::NE:
	case Directions::NW:
	case Directions::N:
		hitbox.left = owner->hitbox.left + 6;
		hitbox.top = (owner->hitbox.top - owner->hitbox.height) - 16; // Normally hitbox height would be divided by two, but hitbox is already half-height of sprite
		break;
	// Below the player
	case Directions::S:
		hitbox.left = owner->hitbox.left + 16;
		hitbox.top = (owner->hitbox.top - owner->hitbox.height) + 30;
		break;
	// Left of the player
	case Directions::SW:
	case Directions::W:
		hitbox.left = owner->hitbox.left - hitbox.width + 2;
		hitbox.top = (owner->hitbox.top - owner->hitbox.height) + 14; 
		break;
	// Right of the player
	case Directions::SE:
	case Directions::E:
		hitbox.left = owner->hitbox.left + 25;
		hitbox.top = (owner->hitbox.top - owner->hitbox.height) + 14;
		break;
	}
}

void EntitySword::SetSpriteRotationAndPositionBasedOnDirection(const Directions direction)
{
	switch (direction)
	{
	// Above the player
	case Directions::NE:
	case Directions::NW:
	case Directions::N:
		sprite.setRotation(0.f);
		sprite.setPosition(hitbox.left - SWORD_HILT_EXTRA_PIXELS, hitbox.top);
		break;
	// Below the player
	case Directions::S:
		sprite.setRotation(180.f);
		sprite.setPosition(hitbox.left + (SWORD_WIDTH + SWORD_HILT_EXTRA_PIXELS), hitbox.top + (owner->hitbox.height * 2));
		break;
	// Left of the player
	case Directions::SW:
	case Directions::W:
		sprite.setRotation(270.f);
		sprite.setPosition(hitbox.left, hitbox.top + (SWORD_WIDTH + SWORD_HILT_EXTRA_PIXELS));
		break;
	// Right of the player
	case Directions::SE:
	case Directions::E:
		sprite.setRotation(90.f);
		sprite.setPosition(hitbox.left + owner->hitbox.width, hitbox.top - SWORD_HILT_EXTRA_PIXELS);
		break;
	}
}

void EntitySword::Think(const float dt)
{
	LoadSprite(); // Will only load once
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
	game->window.draw(sprite);
}

void EntitySword::HandleCollision(Entity* other)
{

}

void EntitySword::Move(sf::Vector2f move, const float dt)
{
	hitbox.left = owner->hitbox.left;
	hitbox.top = owner->hitbox.top;
}