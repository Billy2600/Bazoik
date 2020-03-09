#pragma once
#include "entity.h"

enum class FloorTypes
{
	GravelOne,
	GravelTwo,
	GravelThree
};

class EntityFloor : public Entity
{
private:
	sf::Sprite sprite;
	AnimManager animManager;
	FloorTypes type;

public:
	// This entity doesn't to much, so we're going to impliment the members here (no equivalent .cpp file)
	EntityFloor(const sf::Vector2f pos, const FloorTypes type)
	{
		this->type = type;
		sprite.setPosition(pos);
		// We don't want to cause any collisions, also why there's no debug shape
		hitbox = sf::FloatRect(-9000.f, -9000.f, 0.f, 0.f);
	}

	virtual void Think(const float dt)
	{
		if (sprite.getTexture() == NULL)
		{
			sprite.setTexture(game->assetManager.GetTextureRef("sprites"));
			
			switch (type)
			{
			case FloorTypes::GravelOne:
				sprite.setTextureRect(animManager.Animate("gravel_1"));
				break;
			case FloorTypes::GravelTwo:
				sprite.setTextureRect(animManager.Animate("gravel_2"));
				break;
			case FloorTypes::GravelThree:
				sprite.setTextureRect(animManager.Animate("gravel_3"));
				break;
			}
		}
	}
	virtual void Draw() const
	{
		game->window.draw(sprite);
	}

	// Still need to move floors on screen transition
	virtual void Move(sf::Vector2f move, const float dt)
	{
		sprite.move(move.x * dt, move.y * dt);
	}

	virtual void HandleCollision(Entity* other) {}
};