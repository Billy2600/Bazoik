#pragma once
#include "entity.h"

#define WALL_THICKNESS 10 // Thickness of walls
#define WALL_HEIGHT 80 // Height of left/right walls
#define WALL_WIDTH 160 // Width of top/bottom walls

class EntityWall : public Entity
{
private:
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif

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