#include "entity.h"

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
		return N;
	if( angle >= 1 * degree && angle < 2 * degree )
		return NE;
	if( angle >= 2 * degree && angle < 3 * degree )
		return E;
	if( angle >= 3 * degree && angle < 4 * degree )
		return SE;
	if( angle >= 4 * degree && angle < 5 * degree )
		return S;
	if( angle >= 5 * degree && angle < 6 * degree )
		return SW;
	if( angle >= 6 * degree && angle < 7 * degree )
		return W;
	if( angle >= 7 * degree && angle < 8 * degree )
		return NW;

	//Should never happen:
	return N;
}