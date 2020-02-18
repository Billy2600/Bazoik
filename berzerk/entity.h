#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "game.h"

class EntityManager;

// Directions
enum class Directions { N, NE, E, SE, S, SW, W, NW };

#define DIAG_SPEED 0.4142f // Subtracted from diagonal movements to keep speed consistant

class Entity
{
public:
	bool deleteMe = false; // Delete flag
	Game *game;
	sf::FloatRect hitbox; // Hit box for size and positioning
	EntityManager *entityManager; // So we can instantiate bullets etc.
	sf::Int8 drawPriority = 0; // Lower priority will be drawn first, then higher priority items on top of them

	virtual void Think( const float dt ) = 0; // Think every step
	virtual void Draw() const = 0; // Draw every frame
	virtual void HandleCollision( Entity *other ) = 0; // Called upon collision
	virtual void Move(sf::Vector2f move, const float dt); // Add vector to produce movement
	sf::Vector2f GetMoveTowardsVec(const sf::Vector2f target, float speed); // Get vector to move towards point, will not do actual 

	// Angle helper functions
	float GetAngle( const sf::Vector2f posA, const sf::Vector2f posB ) const;
	float RadToDeg( const float angle ) const;
	Directions AngleToDirection( float angle ) const;
};

// Allows sorting by draw priority
// Necessary since entities are stored by pointer
struct DrawPriority
{
	bool operator() (const Entity* entityA, const Entity* entityB) const;
};