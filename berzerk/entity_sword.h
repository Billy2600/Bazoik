#pragma once
#include "entity.h"

#define SWORD_HEIGHT 11
#define SWORD_WIDTH 3
#define SWORD_SWING_DELAY 80 // In milliseconds

class EntitySword : public Entity
{
private:
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif
	sf::Sprite sprite;
	AnimManager animManager;
	Directions lastDir;
	sf::Clock clock;

	void LoadSprite();
	void SetDimensionsBasedOnDirection(Directions direction);
	void SetPositionBasedOnDirection(Directions direction);

public:
	Entity* owner;

	EntitySword(Directions direction, Entity* owner);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void HandleCollision(Entity* other);
	virtual void Move(sf::Vector2f move, const float dt);
};