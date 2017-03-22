#include "entity_otto.h"
#include <cmath>

EntityOtto::EntityOtto( sf::Vector2f pos )
{
	shape.setRadius( 20.f );
	shape.setFillColor( sf::Color( 255, 165, 0 ) );
	hitbox.width = 20;
	hitbox.height = 20;
}

void EntityOtto::Think( const float dt )
{
	hitbox.left = abs( std::fmod( hitbox.top++, GAME_HEIGHT ) - ( GAME_HEIGHT / 2 ) );
	shape.setPosition( sf::Vector2f( hitbox.top, hitbox.left ) );
}

void EntityOtto::Draw() const
{
	this->game->window.draw( shape );
}

void EntityOtto::HandleCollision( Entity *other )
{

}