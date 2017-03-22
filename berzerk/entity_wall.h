#pragma once
#include "entity.h"

class EntityWall : public Entity
{
private:
	sf::RectangleShape shape;

public:
	EntityWall();
	EntityWall( const sf::Vector2f pos, const sf::Vector2f size, sf::Color color = sf::Color::Blue );
	void SetBox( const sf::Vector2f pos, const sf::Vector2f size );
	void SetColor( const sf::Color color );
	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision( Entity *other );
	void SetColor(int i = 0) { shape.setFillColor( sf::Color(i,0,0) ); }
};