#include "entity_door.h"
#include "entity_player.h"
#include "entity_wall.h"

EntityDoor::EntityDoor(DoorStates initialState = DoorStates::None, Directions initialDirection = Directions::W)
{
	drawPriority = 3;
	state = initialState;
	direction = initialDirection;

	SetPositionRotationBasedOnDirection(direction);

	if (initialState == DoorStates::Open)
	{
		doorOpened = true;
	}

	doorAlreadyOpenCheck = false;

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

	// This stuff is here so sprite can change if door state is changed
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
	sprite.setPosition(position);
	sprite.setTextureRect(animRect);
#ifdef  _DEBUG
	shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
#endif // _DEBUG

	if (DoorAlreadyOpened())
		doorOpened = true;

	if (doorOpened)
		OpenDoor();
}

void EntityDoor::Draw() const
{
#ifdef _DEBUG
	game->window.draw(shape);
#endif // __DEBUG
	game->window.draw(sprite);
	game->window.draw(wallPiece);
}

void EntityDoor::Move(sf::Vector2f move, const float dt)
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
	position.x += move.x * dt;
	position.y += move.y * dt;

	const sf::Vector2f moveDelta = sf::Vector2f(move.x * dt, move.y * dt);
	sprite.setPosition(sprite.getPosition() + moveDelta);
#ifdef  _DEBUG
	shape.setPosition(shape.getPosition() + moveDelta);
#endif //  _DEBUG
	wallPiece.setPosition(wallPiece.getPosition() + moveDelta);
}

void EntityDoor::HandleCollision(Entity* other)
{
	if (dynamic_cast<EntityPlayer*>(other) != NULL)
	{
		if (state == DoorStates::Closed || (state == DoorStates::Locked && game->UseKey()))
		{
			doorOpened = true;
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

void EntityDoor::OpenDoor()
{
	if (entityManager != NULL)
	{
		sf::Vector2f floorOffset;
		switch (direction)
		{
		case Directions::N: floorOffset = sf::Vector2f(16, 40); break;
		case Directions::E: floorOffset = sf::Vector2f(0, 16); break;
		case Directions::S: floorOffset = sf::Vector2f(48, 0); break;
		case Directions::W: floorOffset = sf::Vector2f(40, 48); break;
		}

		entityManager->Add(new EntityDoorFloor(sf::Vector2f(hitbox.left, hitbox.top) + floorOffset, direction, sprite.getRotation(), game));

		// Move hitbox somehwere we know won't be collided with
		hitbox.left = 9000;
		hitbox.top = 9000;

		if (state == DoorStates::Locked)
		{
			game->openedDoors.push_back(std::make_pair(game->currentRoom, direction));
		}

		state = DoorStates::Open;
		doorOpened = false;
	}
}

bool EntityDoor::DoorAlreadyOpened()
{
	if (!doorAlreadyOpenCheck)
	{
		doorAlreadyOpenCheck = true;

		if (state != DoorStates::Locked)
			return false;

		if (game->openedDoors.empty())
			return false;

		return std::find(game->openedDoors.begin(), game->openedDoors.end(), std::make_pair(game->currentRoom, direction)) != game->openedDoors.end();
	}

	return false;
}

void EntityDoor::SetPositionRotationBasedOnDirection(const Directions direction)
{
	wallPiece = sf::RectangleShape();

	const sf::Vector2f topLeft(48, 48);
	switch (direction)
	{
	case Directions::S:
		hitbox.top = topLeft.y + (WALL_HEIGHT * 2) + DOOR_WIDTH;
		hitbox.left = topLeft.x + WALL_WIDTH;
		hitbox.width = DOOR_WIDTH;
		hitbox.height = DOOR_HEIGHT;
		sprite.setOrigin(sf::Vector2f(hitbox.width, hitbox.height));
		sprite.setRotation(180.f);
		wallPiece.setFillColor(sf::Color(0, 132, 132));
		wallPiece.setSize(sf::Vector2f(64, 8));
		wallPiece.setPosition(sf::Vector2f(hitbox.left, hitbox.top + hitbox.height + 8));
		break;
	case Directions::E:
		hitbox.top = topLeft.y + WALL_HEIGHT;
		hitbox.left = topLeft.x + (WALL_WIDTH * 2) + DOOR_WIDTH;
		hitbox.width = DOOR_HEIGHT;
		hitbox.height = DOOR_WIDTH;
		sprite.setOrigin(sf::Vector2f(0, hitbox.width));
		sprite.setRotation(90.f);
		wallPiece.setFillColor(sf::Color(0, 132, 132));
		wallPiece.setSize(sf::Vector2f(8, 64));
		wallPiece.setPosition(sf::Vector2f(hitbox.left + hitbox.width + 8, hitbox.top));
		break;
	case Directions::W:
		hitbox.top = topLeft.y + WALL_HEIGHT;
		hitbox.left = topLeft.x - DOOR_HEIGHT;
		hitbox.width = DOOR_HEIGHT;
		hitbox.height = DOOR_WIDTH;
		sprite.setOrigin(sf::Vector2f(hitbox.height, 0));
		sprite.setRotation(270.f);
		wallPiece.setFillColor(sf::Color(0, 132, 132));
		wallPiece.setSize(sf::Vector2f(8, 64));
		wallPiece.setPosition(sf::Vector2f(hitbox.left - 8, hitbox.top));
		break;
	case Directions::N:
	default:
		hitbox.top = topLeft.y - DOOR_HEIGHT;
		hitbox.left = topLeft.x + WALL_WIDTH;
		hitbox.width = DOOR_WIDTH;
		hitbox.height = DOOR_HEIGHT;
		// Not technically needed, but let's set 'em anyway
		sprite.setOrigin(sf::Vector2f(0, 0));
		sprite.setRotation(0.f);
		// Definitely needed for open doors
		wallPiece.setFillColor(sf::Color(0, 132, 132));
		wallPiece.setSize(sf::Vector2f(64, 8));
		wallPiece.setPosition(sf::Vector2f(hitbox.left, hitbox.top - 8));
		break;
	}

	position = sf::Vector2f(hitbox.left, hitbox.top);
}

DoorStates EntityDoor::GetState() const
{
	return state;
}