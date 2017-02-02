#include "entity_wall.h"

EntityWall::EntityWall()
{

}

EntityWall::EntityWall( sf::Vector2f pos, sf::Vector2f size )
{
	SetBox( pos, size );
}

void EntityWall::SetBox( sf::Vector2f pos, sf::Vector2f size )
{
	hitbox.left = pos.x;
	hitbox.top = pos.y;
	hitbox.width = size.x;
	hitbox.height = size.y;

	shape.setFillColor( sf::Color::Blue );
	shape.setPosition( pos );
	shape.setSize( size );
}

void EntityWall::Think( const float dt )
{

}

void EntityWall::Draw() const
{
	this->game->window.draw( shape );
}

void EntityWall::HandleCollision( Entity *other )
{

}

void EntityWall::Move( sf::Vector2f move, const float dt )
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
	shape.move( move );
}