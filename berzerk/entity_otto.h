#pragma once
#include "entity.h"

#define OTTO_BUFFER 50 // Buffer above/below player so it's possible to avoid Otto

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

public:
	EntityOtto( const sf::Vector2f pos, const float minHeight = 0, const float maxHeight = GAME_HEIGHT );

	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	void SetMinMaxHeight( const float maxHeight, const float minHeight );

};