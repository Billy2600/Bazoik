#pragma once
#include "entity.h"
#include "entity_player.h"
#include "pugixml.hpp"

//#define ROBOT_FIRE_DELAY 3000 // Firing delay, in milliseconds

// Stats for robots for each level
struct RobotStats
{
	bool stopIfSeePlayer;
	bool canShoot;
	float movementSpeed;
	int fireDelay;
	int numRobots;
	sf::Color color;
	float scale;
};

class EntityRobot : public Entity
{
private:
	sf::Sprite sprite;
	sf::Sprite subSprite; // displays on top so we can parts of the robot that don't change color
	sf::Sound sfx;
	int hits;
	bool dead;
	sf::Clock clock;
	sf::Int32 lastFire;
	const sf::Int32 deathDelay = 500; // Delay before deletion
	sf::Int32 deathTime;
	sf::Vector2f playerPos;
	bool moving;
	std::string currentAnim;
	std::string subAnim;
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif

	void LoadSprite();
	// Stats for difficulty
	RobotStats stats;

public:
	bool seePlayer;
	bool drawHitbox;

	EntityRobot(const sf::Vector2f pos, const RobotStats stats );
	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	void SetPlayerPos( sf::Vector2f playerPos );
	bool IsDead() const;
	~EntityRobot();
};

