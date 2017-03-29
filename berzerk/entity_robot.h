#pragma once
#include "entity.h"
#include "entity_player.h"

#define ROBOT_FIRE_DELAY 3000 // Firing delay, in milliseconds

class EntityRobot : public Entity
{
private:
	sf::RectangleShape shape;
	// Fire delay stuff
	sf::Clock clock;
	sf::Vector2f playerPos;
	bool moving;
	sf::Sprite sprite;
	std::string currentAnim;

	void LoadSprite();

public:
	bool seePlayer;
	bool drawHitbox;

	EntityRobot(const sf::Vector2f pos);
	~EntityRobot();
	virtual void Think( const float dt );
	virtual void Draw() const;
	virtual void HandleCollision( Entity *other );
	void SetPlayerPos( sf::Vector2f playerPos );
};

