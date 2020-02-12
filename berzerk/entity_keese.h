#pragma once
#include <SFML/Graphics.hpp>
#include "entity_enemy.h"

#define KEESE_MOVE_TIME_LIMIT 5 // In seconds

class EntityKeese : public EntityEnemy
{
private:
	AnimManager animManager;
	sf::Sprite sprite;
	bool moving;
	sf::Clock moveTimer;

	void LoadSprite();

public:
	EntityKeese(const sf::Vector2f pos);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void HandleCollision(Entity* other);
};

