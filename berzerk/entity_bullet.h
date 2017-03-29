#pragma once
#include "entity.h"

#define BULLET_SPEED 500
#define BULLET_WIDTH 2
// Different dimensions baesd on who fired (accurate to original)
#define PLAYER_BULLET_HEIGHT 30
#define ROBOT_BULLET_HEIGHT 20

class EntityBullet : public Entity
{
private:
	sf::RectangleShape shape;
	sf::Vector2f direction;

public:
	Entity* owner;

	EntityBullet( sf::Vector2f pos, sf::Vector2f direction, Entity* owner );
	void SetAngle(); // Set angle based on direction
	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	virtual void Move( sf::Vector2f move, const float dt );
	void RemoveOwner();
};