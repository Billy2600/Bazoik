#pragma once
#include "entity.h"

// Movement stuff for Otto
#define OTTO_PERIOD 6
#define OTTO_AMPLITUDE 3

class EntityOtto : public Entity
{
private:
	sf::CircleShape shape;

public:
	EntityOtto( const sf::Vector2f pos );

	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );

};