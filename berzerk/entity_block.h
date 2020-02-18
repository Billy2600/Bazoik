#pragma once
#include "entity.h"

enum class BlockTypes
{
	Regular,
	Inverted,
	FaceRightLight,
	FaceRightDark,
	FaceLeftLight,
	FaceLeftDark,
	Hole,
	Water
};

class EntityBlock : public Entity
{
private:
	BlockTypes type;
	bool pushable;
	Directions pushableDirection;
	sf::Sprite sprite;
	AnimManager animManager;
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif

	void SetupSprite();

public:
	EntityBlock(const sf::Vector2f pos, const BlockTypes type = BlockTypes::Regular, const bool pushable = false, const Directions pushableDirection = Directions::N);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision(Entity* other);
};

