#include "entity.h"

void Entity::Move(sf::Vector2f move, const float dt)
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
}

sf::Vector2f Entity::GetMoveTowardsVec(const sf::Vector2f target, float speed) // Get vector to move towards point, will not do actual movement
{
	sf::Vector2f targetVec = target - sf::Vector2f(hitbox.left, hitbox.top);
	float targetVecMagnitude = sqrtf(targetVec.x * targetVec.x) + (targetVec.y * targetVec.y);
	sf::Vector2f normalizedTargetVec = targetVec / targetVecMagnitude;
	sf::Vector2f moveVec;
	float angle = atan2f(normalizedTargetVec.y, normalizedTargetVec.x);
	moveVec.x = cosf(angle) * speed;
	moveVec.y = sinf(angle) * speed;

	return moveVec;
}

float Entity::GetAngle( const sf::Vector2f posA, const sf::Vector2f posB ) const
{
	float dot = posA.x * posB.x + posA.y * posB.y;
	float det = posA.x * posB.y - posA.y * posB.x;
	return atan2( det, dot );
}

float Entity::RadToDeg( const float angle ) const
{
	float degAngle = angle / 3.14159265f * 180.f;
	/*if( degAngle <= -1 ) degAngle = 360 + degAngle;
	if( degAngle >= 360 ) degAngle = degAngle - 360;*/

	return -degAngle;
}

Directions Entity::AngleToDirection( float angle ) const
{
	//easy to customize by changing the number of directions you have
	int directions = 8;
	int degree = 360 / directions;
	angle = angle + degree / 2;
	if( angle >= 0 * degree && angle < 1 * degree )
		return Directions::N;
	if( angle >= 1 * degree && angle < 2 * degree )
		return Directions::NE;
	if( angle >= 2 * degree && angle < 3 * degree )
		return Directions::E;
	if( angle >= 3 * degree && angle < 4 * degree )
		return Directions::SE;
	if( angle >= 4 * degree && angle < 5 * degree )
		return Directions::S;
	if( angle >= 5 * degree && angle < 6 * degree )
		return Directions::SW;
	if( angle >= 6 * degree && angle < 7 * degree )
		return Directions::W;
	if( angle >= 7 * degree && angle < 8 * degree )
		return Directions::NW;

	//Should never happen:
	return Directions::N;
}

bool DrawPriority::operator() (const Entity* entityA, const Entity* entityB) const
{
	return entityA->drawPriority < entityB->drawPriority;
}