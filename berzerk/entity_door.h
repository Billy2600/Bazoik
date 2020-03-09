#pragma once
#include "entity.h"
#include "anim_manager.h"

#define DOOR_WIDTH 64
#define DOOR_HEIGHT 40

enum class DoorStates
{
	None,
	Closed,
	Locked,
	Open
};

class EntityDoor : public Entity
{
private:
#ifdef _DEBUG
	sf::RectangleShape shape;
#endif
	DoorStates state; // What state is this door in?
	Directions direction; // What direction should door be facing
	AnimManager animManager;
	sf::Sprite sprite;
	sf::Vector2f position; // Need to store position seperately, as sprite and hitbox can be in different places
	bool doorOpened; // Door opened flag

	void SetPositionRotationBasedOnDirection(const Directions direction);
	void LoadSprite();
	void OpenDoor();

public:
	EntityDoor(DoorStates initialState, Directions initialDirection);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void Move(sf::Vector2f move, const float dt);
	virtual void HandleCollision(Entity* other);
	DoorStates GetState() const;
};

// Entity for drawing the floor under the frame and player, for open doors
class EntityDoorFloor : public Entity
{
private:
	sf::Sprite sprite;

public:
	EntityDoorFloor(sf::Vector2f pos, Game* game);
	virtual void Draw() const;
	virtual void Think(const float dt);

	// Don't really need these, but compiler will throw a fit if we don't add them
	virtual void Move(sf::Vector2f move, const float dt) {}
	virtual void HandleCollision(Entity* other) {}
};