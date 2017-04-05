#pragma once
#include <vector>
#include "entity.h"
#include "entity_manager.h"
#include "entity_wall.h"
#include "entity_robot.h"
#include "entity_bullet.h"

#define PLAYER_SPEED 200

struct PlayerInput
{
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool fire = false;
};

class EntityPlayer : public Entity
{
private:
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif
	PlayerInput input;

	sf::Clock clock; // Used for measuring delays
	sf::Int32 now; // Current time, updated every step
	sf::Int32 lastFire; // Last time we fired
	const sf::Int32 fireDelay = 500; // Delay between fired shots (in milliseconds)
	sf::Vector2f direction; // Currently facing direction (will direct bullets)
	Directions lastHoriz; // Last horizontal direction we moved (for animations)

	float deathTime; // Time we died
	const sf::Int32 resetDelay = 500; // Delay before resetting the state after dying
	bool dead; // Death flag
	bool reset; // Reset flag

	void Die(); // Perform death

	sf::Sprite sprite;
	std::string currentAnim;
	void LoadSprite();

public:
	bool drawHitbox;

	EntityPlayer();
	bool CheckDead() const; // Signal to state that it's time to reset
	void SetInput( const PlayerInput input );
	void SetPos( const sf::Vector2f pos );
	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void Move( sf::Vector2f move, const float dt );
	virtual void HandleCollision( Entity *other );
};