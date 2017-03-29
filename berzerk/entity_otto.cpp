#include "entity_otto.h"

EntityOtto::EntityOtto( sf::Vector2f pos, const float minHeight, const float maxHeight )
{
	shape.setRadius( 20.f );
	shape.setFillColor( sf::Color( 255, 165, 0 ) );
	hitbox.left = pos.x;
	hitbox.top = pos.y;
	SetMinMaxHeight( minHeight, maxHeight );
	hitbox.width = 20;
	hitbox.height = 20;

	float direction = ySpeed;
}

void EntityOtto::Think( const float dt )
{
	if( direction == 0 ) direction = ySpeed; // Make sure this is initialized

	hitbox.left += xSpeed * dt;
	if( ( hitbox.top + hitbox.height ) > maxHeight )
		direction = -ySpeed;
	else if( ( hitbox.top + hitbox.height ) < minHeight )
		direction = ySpeed;

	hitbox.top += direction * dt;

	shape.setPosition( sf::Vector2f(hitbox.left, hitbox.top) );
}

void EntityOtto::Draw() const
{
	this->game->window.draw( shape );
}

void EntityOtto::HandleCollision( Entity *other )
{
	// Fool, nothing can stop Evil Otto!
}

void EntityOtto::SetMinMaxHeight( const float minHeight, const float maxHeight )
{
	this->maxHeight = maxHeight;
	this->minHeight = minHeight;
}