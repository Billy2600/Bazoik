#pragma once
#include "entity.h"

// Movement stuff for Otto
#define OTTO_PERIOD 6
#define OTTO_AMPLITUDE 3

class EntityOtto : public Entity
{
private:
	sf::Sprite sprite;
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif
	const float xSpeed = 50;
	const float ySpeed = 200;
	float maxHeight;
	float minHeight;
	float direction; // Current direction (up/down)
	sf::Vector2f playerPos;

public:
	EntityOtto( const sf::Vector2f pos, const float minHeight = 0, const float maxHeight = GAME_HEIGHT );

	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	void SetMinMaxHeight( const float maxHeight, const float minHeight );

};