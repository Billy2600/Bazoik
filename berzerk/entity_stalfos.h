#pragma once
#include "entity_enemy.h"

class EntityStalfos : public EntityEnemy
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
	EntityStalfos(const sf::Vector2f pos);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void HandleCollision(Entity* other);
};

