#pragma once
#include "entity.h"
#include "entity_player.h"

#define ROBOT_FIRE_DELAY 3000 // Firing delay, in milliseconds

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

