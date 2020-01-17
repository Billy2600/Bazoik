#include "entity_wall.h"

EntityWall::EntityWall()
{

}

EntityWall::EntityWall( sf::Vector2f pos, sf::Vector2f size, sf::Color color )
{
	SetBox( pos, size );
	SetColor( color );
}

void EntityWall::SetBox( sf::Vector2f pos, sf::Vector2f size )
{
	hitbox.left = pos.x;
	hitbox.top = pos.y;
	hitbox.width = size.x;
	hitbox.height = size.y;

#ifdef _DEBUG
	shape.setFillColor( sf::Color::Transparent );
	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1.f );
	shape.setPosition( pos );
	shape.setSize( size );
#endif
}

void EntityWall::SetColor( const sf::Color color )
{
#ifdef _DEBUG
	shape.setOutlineColor(color);
#endif
}

void EntityWall::Think( const float dt )
{
	
}

void EntityWall::Draw() const
{
#ifdef _DEBUG
	this->game->window.draw( shape );
#endif
}

void EntityWall::HandleCollision( Entity *other )
{

}

void EntityWall::Move( sf::Vector2f move, const float dt )
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
	shape.move( move * dt );
}