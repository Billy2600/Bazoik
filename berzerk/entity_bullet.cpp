#include "entity_bullet.h"
#include "entity_robot.h"

EntityBullet::EntityBullet( sf::Vector2f pos, sf::Vector2f direction, Entity* owner = NULL )
{
	hitbox.top = pos.y;
	hitbox.left = pos.x;
	hitbox.height = 10;
	hitbox.width = 1;
	shape.setPosition( pos );
	shape.setFillColor( sf::Color::Red );
	shape.setSize( sf::Vector2f( 1, 20 ) );
	this->direction = direction;
	this->owner = owner;
	SetAngle();
}

void EntityBullet::SetAngle()
{
	shape.setRotation( ( atan2(direction.y, direction.x) / 3.14159265f * 180.f ) - 90 );
}

void EntityBullet::Think( const float dt )
{
	if( deleteMe ) return;
	// Move based on direction
	
	// Don't allow diagonal shots to move faster
	float realSpeed = BULLET_SPEED;
	if( ( direction.x < 0 && direction.y < 0 ) || ( direction.x > 0 && direction.y > 0 ) ||
		( direction.x > 0 && direction.y < 0 ) || ( direction.x < 0 && direction.y > 0 ) )
	{
		realSpeed = BULLET_SPEED - DIAG_SPEED;
	}

	// Calculate angle + speed
	sf::Vector2f moveVec;
	float angle = atan2f( direction.y, direction.x );
	moveVec.x = cosf( angle ) * realSpeed;
	moveVec.y = sinf( angle ) * realSpeed;

	Move( moveVec, dt );
}

void EntityBullet::Draw() const
{
	game->window.draw( shape );
}

void EntityBullet::HandleCollision( Entity *other )
{
	// Delete in all cases, except when colliding with owner
	// Other object will decide what to do (usually die)
	if( other != owner )
	{
		deleteMe = true;
	}
}

void EntityBullet::Move( sf::Vector2f move, const float dt ) // Add vector to produce movement
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
}

void EntityBullet::RemoveOwner()
{
	owner = NULL;
}