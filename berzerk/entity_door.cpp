#include "entity_door.h"
#include "entity_player.h"
#include "entity_wall.h"

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
	shape.setPosition( sf::Vector2f(hitbox.left, hitbox.top) );
#endif
}

void EntityDoor::Think(const float dt)
{
	LoadSprite();

	sf::IntRect animRect;
	switch (state)
	{
	case DoorStates::None:
		animRect = sf::IntRect(0, 0, 0, 0);
		break;
	case DoorStates::Closed:
		animRect = animManager.Animate("door_closed", true);
		break;
	case DoorStates::Locked:
		animRect = animManager.Animate("door_locked", true);
		break;
	case DoorStates::Open:
		animRect = animManager.Animate("door_open", true);
		break;
	}
	sprite.setTextureRect(animRect);
}

void EntityDoor::Draw() const
{
#ifdef _DEBUG
	game->window.draw(shape);
#endif // __DEBUG

	game->window.draw(sprite);
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

void EntityDoor::LoadSprite()
{
	// Load texture if we need to
	if (sprite.getTexture() == NULL)
	{
		sprite.setTexture(game->assetManager.GetTextureRef("sprites"));
	}
}

void EntityDoor::SetPositionRotationBasedOnDirection(const Directions direction)
{
	const sf::Vector2f topLeft(48, 48);
	switch (direction)
	{
	case Directions::S:
		hitbox.top = topLeft.y + (WALL_HEIGHT * 2) + DOOR_WIDTH;
		hitbox.left = topLeft.x + WALL_WIDTH;
		hitbox.width = DOOR_WIDTH;
		hitbox.height = DOOR_HEIGHT;
		sprite.setOrigin(sf::Vector2f(hitbox.width, hitbox.height));
		sprite.setRotation(180);
		break;
	case Directions::E:
		hitbox.top = topLeft.y + WALL_HEIGHT;
		hitbox.left = topLeft.x + (WALL_WIDTH * 2) + DOOR_WIDTH;
		hitbox.width = DOOR_HEIGHT;
		hitbox.height = DOOR_WIDTH;
		sprite.setOrigin(sf::Vector2f(0, hitbox.width));
		sprite.setRotation(90);
		break;
	case Directions::W:
		hitbox.top = topLeft.y + WALL_HEIGHT;
		hitbox.left = topLeft.x - DOOR_HEIGHT;
		hitbox.width = DOOR_HEIGHT;
		hitbox.height = DOOR_WIDTH;
		sprite.setOrigin(sf::Vector2f(hitbox.height, 0));
		sprite.setRotation(270);
		break;
	case Directions::N:
	default:
		hitbox.top = topLeft.y - DOOR_HEIGHT;
		hitbox.left = topLeft.x + WALL_WIDTH;
		hitbox.width = DOOR_WIDTH;
		hitbox.height = DOOR_HEIGHT;
		// Not technically needed, but let's set 'em anyway
		sprite.setOrigin(sf::Vector2f(0, 0));
		sprite.setRotation(0);
		break;
	}

	sprite.setPosition( sf::Vector2f(hitbox.left, hitbox.top) );
}

DoorStates EntityDoor::GetState() const
{
	return state;
}