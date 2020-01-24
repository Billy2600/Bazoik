#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "game.h"

class EntityManager;

// Directions
enum Directions { N, NE, E, SE, S, SW, W, NW };

#define DIAG_SPEED 0.4142f // Subtracted from diagonal movements to keep speed consistant

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

	sf::Vector2f GetMoveTowardsVec( const sf::Vector2f target, float speed ) // Get vector to move towards point, will not do actual movement
	{
		sf::Vector2f targetVec = target - sf::Vector2f( hitbox.left, hitbox.top );
		float targetVecMagnitude = sqrtf( targetVec.x * targetVec.x ) + ( targetVec.y * targetVec.y );
		sf::Vector2f normalizedTargetVec = targetVec / targetVecMagnitude;
		sf::Vector2f moveVec;
		float angle = atan2f( normalizedTargetVec.y, normalizedTargetVec.x );
		moveVec.x = cosf( angle ) * speed;
		moveVec.y = sinf( angle ) * speed;

		return moveVec;
	}

	// Angle helper functions
	float GetAngle( const sf::Vector2f posA, const sf::Vector2f posB ) const;
	float RadToDeg( const float angle ) const;
	Directions AngleToDirection( float angle ) const;
};