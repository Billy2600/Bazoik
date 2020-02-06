#include "entity_door.h"
#include "entity_player.h"

EntityDoor::EntityDoor(DoorStates initialState = DoorStates::None, Directions initialDirection = Directions::W)
{
	state = initialState;
	direction = initialDirection;

	SetPositionRotationBasedOnDirection(direction);

#ifdef _DEBUG
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setSize( sf::Vector2f(hitbox.width, hitbox.height) );
	shape.setPosition( sf::Vector2f(hitbox.top, hitbox.left) );
#endif
}

void EntityDoor::Think(const float dt)
{
	
}

void EntityDoor::Draw() const
{
#ifdef _DEBUG
	game->window.draw(shape);
#endif // __DEBUG
}

void EntityDoor::Move(sf::Vector2f move, const float dt)
{

}

void EntityDoor::HandleCollision(Entity* other)
{
	if (dynamic_cast<EntityPlayer*>(other) != NULL)
	{
		if (state == DoorStates::Closed)
		{
			state = DoorStates::Open;
		}
	}
}

void EntityDoor::SetPositionRotationBasedOnDirection(const Directions direction)
{
	switch (direction)
	{
	case Directions::S:
		hitbox.width = DOOR_WIDTH;
		hitbox.height = DOOR_HEIGHT;
		break;
	case Directions::E:
		hitbox.width = DOOR_HEIGHT;
		hitbox.height = DOOR_WIDTH;
		break;
	case Directions::W:
		hitbox.width = DOOR_HEIGHT;
		hitbox.height = DOOR_WIDTH;
		break;
	case Directions::N:
	default:
		hitbox.width = DOOR_WIDTH;
		hitbox.height = DOOR_HEIGHT;
		break;
	}
}