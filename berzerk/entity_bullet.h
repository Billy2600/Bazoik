#pragma once
#include "entity.h"
#include "powerup_types.h"

#define BULLET_SPEED 500
#define BULLET_WIDTH 2
// Different dimensions baesd on who fired (accurate to original)
#define PLAYER_BULLET_HEIGHT 30
#define ROBOT_BULLET_HEIGHT 20

#define BULLET_RICOCHET_SPAWN_DELAY 500 // Delay on collision checks right after we spawned a ricochet bullet

class EntityBullet : public Entity
{
private:
	sf::RectangleShape shape;
	sf::Vector2f direction;
	PowerupType type;
	sf::Clock clock;
	sf::Int8 children;

public:
	Entity* owner;

	EntityBullet( sf::Vector2f pos, sf::Vector2f direction, Entity* owner, PowerupType type = PowerupType::None );
	void SetAngle(); // Set angle based on direction
	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	virtual void Move( sf::Vector2f move, const float dt );
	void RemoveOwner();
};