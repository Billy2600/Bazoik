#pragma once
#include "entity.h"

#define OTTO_BUFFER 50 // Buffer above/below player so it's possible to avoid Otto
#define OTTO_WIDTH 32
#define OTTO_HEIGHT 32
#define OTTO_DELAY 10000 // Delay before Otto will spawn

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
	// Horizonal/vertical directions; numerical as to be used directly in movement
	float hDirection;
	int vDirection; // Should be 1 or -1
	bool dopefish;
	AnimManager animManager;

public:
	EntityOtto( const sf::Vector2f pos, const float minHeight = 0, const float maxHeight = GAME_HEIGHT, const bool dopefish = false, const int vDirection = 1 );

	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	void SetMinMaxHeight( const float maxHeight, const float minHeight );
	virtual void Move( sf::Vector2f move, const float dt );
};