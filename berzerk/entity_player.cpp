#include "entity_player.h"
#include "error_log.h"

EntityPlayer::EntityPlayer()
{
#ifdef _DEBUG
	shape.setFillColor( sf::Color::Transparent );
	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1.f );
#endif
	// Hitbox will remain consistent size, regardless of animation
	hitbox.width = 32.f;
	hitbox.height = 32.f;
	clock.restart();
	dead = false;
	reset = false;
	lastFire = 0;
	deathTime = 0.f;
	now = clock.getElapsedTime().asMilliseconds();
	drawHitbox = false;
	lastDirection = sf::Vector2f(1, 0);
	lastHoriz = GetDirectionFromVector(lastDirection);
}

void EntityPlayer::SetPos( const sf::Vector2f pos )
{
	hitbox.left = pos.x;
	hitbox.top = pos.y;
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
	case Directions::NE:
	case Directions::SE:
	case Directions::N:
		return "n";
		break;
	case Directions::S:
		return "s";
		break;
	case Directions::NW:
	case Directions::SW:
	case Directions::W:
		return "w";
		break;
	case Directions::E:
		return "e";
		break;
	}
}

Directions EntityPlayer::GetDirectionFromVector(sf::Vector2f vector)
{
	if (direction == sf::Vector2f(-1, -1))
		return Directions::NW;
	else if (direction == sf::Vector2f(0, -1))
		return Directions::N;
	else if (direction == sf::Vector2f(1, -1))
		return Directions::NE;
	else if (direction == sf::Vector2f(-1, 0))
		return Directions::W;
	else if (direction == sf::Vector2f(1, 0))
		return Directions::E;
	else if (direction == sf::Vector2f(-1, 1))
		return Directions::SW;
	else if (direction == sf::Vector2f(0, 1))
		return Directions::S;
	else if (direction == sf::Vector2f(1, 1))
		return Directions::SE;
	else if (direction == sf::Vector2f(0, 0))
		return Directions::S;
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

	// Move based on input
	sf::Vector2f move = sf::Vector2f( 0, 0 );
	// Prevent straferunning
	float realSpeed = PLAYER_SPEED;
	if( ( input.left && input.up ) || ( input.left && input.down ) ||
		( input.right && input.up ) || ( input.right && input.down ) )
	{
		realSpeed = PLAYER_SPEED - DIAG_SPEED;
	}

	// Basic movement and basic direction
	if( input.left )
	{
		if( !input.fire ) move.x -= realSpeed;
		direction.x = -1;
		lastHoriz = Directions::W;
	}
	else if( input.right )
	{
		if( !input.fire ) move.x += realSpeed;
		direction.x = 1;
		lastHoriz = Directions::E;
	}
	else
	{
		direction.x = 0;
	}

	if( input.up )
	{
		if( !input.fire ) move.y -= realSpeed;
		direction.y  = -1;
	}
	else if( input.down )
	{
		if( !input.fire ) move.y += realSpeed;
		direction.y = 1;
	}
	else
	{
		direction.y = 0;
	}

	// Choose diagonal directions
	if( input.left && input.up )
	{
		direction = sf::Vector2f( -1, -1 );
		lastHoriz = Directions::W;
	}
	else if( input.right && input.up )
	{
		direction = sf::Vector2f( 1, -1 );
		lastHoriz = Directions::E;
	}
	else if( input.left && input.down )
	{
		direction = sf::Vector2f( -1, 1 );
		lastHoriz = Directions::W;
	}
	else if( input.right && input.down )
	{
		direction = sf::Vector2f( 1, 1 );
		lastHoriz = Directions::E;
	}

	if (direction != sf::Vector2f(0, 0))
	{
		lastDirection = direction;
	}

	// Swing with delay
	if( input.fire && now - lastFire >= fireDelay )
	{
		entityManager->Add( new EntitySword( GetDirectionFromVector(lastDirection) , this ) );
		lastFire = now;
		game->assetManager.PlaySound( "shoot" );
	}

	Move( move, dt );
	// Change animation
	if( move.x > 0 || move.y > 0 || move.x < 0 || move.y < 0 )
		currentAnim = "player_walk_" + ChooseAnimDirection(direction);
	else if( input.fire )
		currentAnim = "player_fire_" + ChooseAnimDirection(direction);
	else
		currentAnim = "player_stand_" + ChooseAnimDirection(direction);
}

void EntityPlayer::Die()
{
	animManager.ResetAnim( "player_death" );
	currentAnim = "player_death";
	dead = true;
	deathTime = (float)now;
	game->assetManager.PlaySound( "death", false, 50 );
	game->RemoveLife();
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

	// Move back on collision with walls, etc.
	if( dynamic_cast<EntityWall*>( other ) != NULL )
	{
		hitbox.left = lastPos.x;
		hitbox.top = lastPos.y;
	}
}

void EntityPlayer::Move( sf::Vector2f move, const float dt ) // Add vector to produce movement
{
	lastPos = sf::Vector2f(hitbox.left, hitbox.top);
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
#ifdef _DEBUG
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
#endif
	sprite.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );

	// Need to flip sprite if moving W, or second frame of walk_n
	if( (lastHoriz == Directions::W) || (currentAnim == "player_walk_n" && animManager.GetCurrentFrame(currentAnim) > 0) )
	{
		sprite.setScale( -1, 1 );
		// Account for sprite mis-aligning with the hitbox when flipped via negative scale
		sprite.move( sf::Vector2f( hitbox.width, 0 ) );
	}
	else
	{
		sprite.setScale( 1, 1 );
	}

	auto test = animManager.Animate(currentAnim);
	sprite.setTextureRect( test );
}

EntityPlayer::~EntityPlayer()
{

}