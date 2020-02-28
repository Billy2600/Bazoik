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
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif

	void LoadSprite();

public:
	EntityKeese(const sf::Vector2f pos);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void HandleCollision(Entity* other);
	virtual void Move(sf::Vector2f move, const float dt);
};

