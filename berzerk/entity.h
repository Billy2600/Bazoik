#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "game.h"

class EntityManager;

// Directions
enum Directions { N, NE, E, SE, S, SW, W, NW };

#define DIAG_SPEED 0.4142f // Subtracted from diagonal movements to keep speed consistant
//#define SPRITE_SCALE 3.f // How much to scale up sprites from original assets

class Entity
{
public:
	bool deleteMe = false; // Delete flag
	Game *game;
	sf::FloatRect hitbox; // Hit box for size and positioning
	EntityManager *entityManager; // So we can instantiate bullets etc.

	virtual void Think( const float dt ) = 0; // Think every step
	virtual void Draw() const = 0; // Draw every frame
	virtual void HandleCollision( Entity *other ) = 0; // Called upon collision
	virtual void Move( sf::Vector2f move, const float dt ) // Add vector to produce movement
	{
		hitbox.left += move.x * dt;
		hitbox.top += move.y * dt;
	}

	// Angle helper functions
	float GetAngle( const sf::Vector2f posA, const sf::Vector2f posB ) const;
	float RadToDeg( const float angle ) const;
	Directions AngleToDirection( float angle ) const;
};