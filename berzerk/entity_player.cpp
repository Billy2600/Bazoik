#include "entity_player.h"
#include "entity_enemy.h"
#include "error_log.h"

EntityPlayer::EntityPlayer()
{
	drawPriority = 2;
#ifdef _DEBUG
	shape.setFillColor( sf::Color::Transparent );
	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1.f );
#endif
	// Hitbox will remain consistent size, regardless of animation
	hitbox.width = 32.f;
	hitbox.height = 16.f;
	clock.restart();
	dead = false;
	reset = false;
	lastFire = 0;
	deathTime = 0.f;
	now = clock.getElapsedTime().asMilliseconds();
	drawHitbox = false;
	lastDirection = sf::Vector2f(0, -1);
	lastHoriz = GetDirectionFromVector(lastDirection);
}

void EntityPlayer::SetPos( const sf::Vector2f pos )
{
	hitbox.left = pos.x;
	hitbox.top = pos.y;
	sprite.setPosition( GetSpritePos() );
#ifdef _DEBUG
	shape.setPosition( pos );
#endif
}

void EntityPlayer::LoadSprite()
{
	// Load texture if we need to
	if( sprite.getTexture() == NULL )
	{
		sprite.setTexture( game->assetManager.GetTextureRef( "sprites" ) );
		currentAnim = "player_stand_s";
		// Load hitbox based on sprite info
		sf::IntRect animRect = animManager.Animate( currentAnim );
#ifdef _DEBUG
		shape.setSize( sf::Vector2f( hitbox.width, hitbox.height ) );
#endif
	}
}

std::string EntityPlayer::ChooseAnimDirection( const sf::Vector2f moveVec )
{
	auto direction = GetDirectionFromVector(moveVec);
	switch (direction)
	{
	case Directions::NW:
	case Directions::NE:
	case Directions::N:
		return "n";
		break;
	case Directions::S:
		return "s";
		break;
	case Directions::SE:
	case Directions::SW:
	case Directions::W:
		return "w";
		break;
	case Directions::E:
		return "e";
		break;
	}
}

Directions EntityPlayer::GetDirectionFromVector(const sf::Vector2f vector)
{
	if (vector == sf::Vector2f(-1, -1))
		return Directions::NW;
	else if (vector == sf::Vector2f(0, -1))
		return Directions::N;
	else if (vector == sf::Vector2f(1, -1))
		return Directions::NE;
	else if (vector == sf::Vector2f(-1, 0))
		return Directions::W;
	else if (vector == sf::Vector2f(1, 0))
		return Directions::E;
	else if (vector == sf::Vector2f(-1, 1))
		return Directions::SW;
	else if (vector == sf::Vector2f(0, 1))
		return Directions::S;
	else if (vector == sf::Vector2f(1, 1))
		return Directions::SE;
	else if (vector == sf::Vector2f(0, 0))
		return Directions::S;
}

sf::Vector2f EntityPlayer::GetSpritePos() const
{
	return sf::Vector2f(hitbox.left, hitbox.top - 16.f);
}

void EntityPlayer::Think( const float dt )
{
	LoadSprite();

	this->now = clock.getElapsedTime().asMilliseconds();

	// Early out here if dead
	if( dead )
	{
		// Keep animation going during death
		sf::IntRect animRect = animManager.Animate( currentAnim, true );
		sprite.setTextureRect( animRect );

		// Check if we should also reset the state
		if( now - deathTime >= resetDelay )
		{
			reset = true;
		}
		return;
	}

	if (knockback)
		KnockbackMove(dt);
	else
		InputMove(dt);
}

void EntityPlayer::Die()
{
	animManager.ResetAnim( "player_death" );
	currentAnim = "player_death";
	dead = true;
	deathTime = (float)now;
	game->assetManager.PlaySound( "death", false, 50 );
}

void EntityPlayer::KnockbackMove(float dt)
{
	sprite.setColor(sf::Color(255, 0, 0, 150));

	if ((now - knockbackStartTime) > KNOCKBACK_TIME)
	{
		knockback = false;
		sprite.setColor(sf::Color::White);
	}

	auto move = sf::Vector2f(knockbackDirection.x * PLAYER_SPEED, knockbackDirection.y * PLAYER_SPEED);
	Move(move, dt);
}

void EntityPlayer::InputMove(float dt)
{
	sf::Vector2f move = sf::Vector2f(0, 0);
	// Prevent straferunning
	float realSpeed = PLAYER_SPEED;
	if ((input.left && input.up) || (input.left && input.down) ||
		(input.right && input.up) || (input.right && input.down))
	{
		realSpeed = PLAYER_SPEED - DIAG_SPEED;
	}

	// Basic movement and basic direction
	if (input.left)
	{
		if (!input.fire) move.x -= realSpeed;
		direction.x = -1;
		lastHoriz = Directions::W;
	}
	else if (input.right)
	{
		if (!input.fire) move.x += realSpeed;
		direction.x = 1;
		lastHoriz = Directions::E;
	}
	else
	{
		direction.x = 0;
	}

	if (input.up)
	{
		if (!input.fire) move.y -= realSpeed;
		direction.y = -1;
	}
	else if (input.down)
	{
		if (!input.fire) move.y += realSpeed;
		direction.y = 1;
	}
	else
	{
		direction.y = 0;
	}

	// Choose diagonal directions
	if (input.left && input.up)
	{
		direction = sf::Vector2f(-1, -1);
		lastHoriz = Directions::W;
	}
	else if (input.right && input.up)
	{
		direction = sf::Vector2f(1, -1);
		lastHoriz = Directions::E;
	}
	else if (input.left && input.down)
	{
		direction = sf::Vector2f(-1, 1);
		lastHoriz = Directions::W;
	}
	else if (input.right && input.down)
	{
		direction = sf::Vector2f(1, 1);
		lastHoriz = Directions::E;
	}

	if (direction != sf::Vector2f(0, 0))
	{
		lastDirection = direction;
	}

	// Swing with delay
	if (input.fire && now - lastFire >= fireDelay)
	{
		entityManager->Add(new EntitySword(GetDirectionFromVector(lastDirection), this));
		lastFire = now;
		game->assetManager.PlaySound("shoot");
	}

	// Perform move
	Move(move, dt);

	// Change animation
	auto animDirection = ChooseAnimDirection(lastDirection);
	if (move.x > 0 || move.y > 0 || move.x < 0 || move.y < 0)
		currentAnim = "player_walk_" + animDirection;
	else if (input.fire)
		currentAnim = "player_fire_" + animDirection;
	else
		currentAnim = "player_stand_" + animDirection;
}

bool EntityPlayer::IsDead() const
{
	return dead;
}

bool EntityPlayer::CheckReset() const
{
	return reset;
}

void EntityPlayer::SetInput( const PlayerInput input )
{
	if(!knockback) // Ignore inputs during knockback
		this->input = input;
}

void EntityPlayer::Draw() const
{
	game->window.draw( sprite );
#ifdef _DEBUG
	game->window.draw( shape );
#endif
}

void EntityPlayer::HandleCollision( Entity *other )
{
	// Early out if dead
	if( dead )
		return;

	if (dynamic_cast<EntityEnemy*>(other) != NULL)
	{
		knockbackStartTime = now;
		knockback = true;
		if (direction == sf::Vector2f(0, 0))
		{
			knockbackDirection = sf::Vector2f(
				other->hitbox.left < hitbox.left ? 1 : -1,
				other->hitbox.top < hitbox.top ? 1 : -1
			);
		}
		else
		{
			knockbackDirection = sf::Vector2f(direction.x * -1, direction.y * -1);
		}
	}
}

void EntityPlayer::Move(sf::Vector2f move, const float dt) // Add vector to produce movement
{
	bool allowMoveX = entityManager->TryMove(this, sf::Vector2f(move.x, 0), dt); // Check x and y separately, to allow 'sliding' along walls, etc.
	bool allowMoveY = entityManager->TryMove(this, sf::Vector2f(0, move.y), dt);

	if (move != sf::Vector2f(0, 0) && (allowMoveX || allowMoveY))
	{
		if(allowMoveX) hitbox.left += move.x * dt;
		if(allowMoveY) hitbox.top += move.y * dt;
#ifdef _DEBUG
		shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
#endif
		sprite.setPosition( GetSpritePos() );

		// Need to flip sprite if moving W, or second frame of walk_n
		if ((lastHoriz == Directions::W && currentAnim != "player_fire_n") || (currentAnim == "player_walk_n" && animManager.GetCurrentFrame(currentAnim) > 0))
		{
			sprite.setScale(-1, 1);
			// Account for sprite mis-aligning with the hitbox when flipped via negative scale
			sprite.move(sf::Vector2f(hitbox.width, 0));
		}
		else
		{
			sprite.setScale(1, 1);
		}
	}

	// Keep animating even if we didn't actually move anywhere
	sprite.setTextureRect( animManager.Animate(currentAnim) );
}

EntityPlayer::~EntityPlayer()
{

}