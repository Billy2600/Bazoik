#pragma once
#include "entity.h"
#include "entity_player.h"
#include "pugixml.hpp"

//#define ROBOT_FIRE_DELAY 3000 // Firing delay, in milliseconds

// Stats for robots for each level
struct RobotStats
{
	bool stopIfSeePlayer;
	float movementSpeed;
	int fireDelay;
	int numRobots;
	sf::Color color;
};

class EntityRobot : public Entity
{
private:
	sf::Sprite sprite;
	// Fire delay stuff
	sf::Clock clock;
	sf::Vector2f playerPos;
	bool moving;
	std::string currentAnim;
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif

	void LoadSprite();
	// Stats for difficulty
	bool stopIfSeePlayer;
	float movementSpeed;
	int fireDelay;

public:
	bool seePlayer;
	bool drawHitbox;

	EntityRobot(const sf::Vector2f pos, const bool stopIfSeePlayer = false, const float movementSpeed = 50, const int fireDelay = 3000, const sf::Color color = ERROR_COLOR );
	~EntityRobot();
	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	void SetPlayerPos( sf::Vector2f playerPos );
};

