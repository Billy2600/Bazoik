#include "entity_player.h"

EntityPlayer::EntityPlayer()
{
	shape.setFillColor( sf::Color::Transparent );
	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1.f );
	clock.restart();
	dead = false;
	reset = false;
	lastFire = 0;
	deathTime = 0.f;
	now = clock.getElapsedTime().asMilliseconds();
	drawHitbox = false;
}

void EntityPlayer::SetPos( const sf::Vector2f pos )
{
	hitbox.left = pos.x;
	hitbox.top = pos.y;
	shape.setPosition( pos );
}

void EntityPlayer::LoadSprite()
{
	// Load texture if we need to
	if( sprite.getTexture() == NULL )
	{
		sprite.setTexture( game->assetManager.GetTextureRef( "sprites" ) );
		sprite.setScale( sf::Vector2f( 4, 4 ) );
		currentAnim = "player_stand";
		// Load hitbox based on sprite info
		sf::IntRect animRect = game->animManager.Animate( currentAnim );
		hitbox.width = animRect.width * sprite.getScale().x;
		hitbox.height = animRect.height * sprite.getScale().y;
		shape.setSize( sf::Vector2f( hitbox.width, hitbox.height ) );
	}
}

void EntityPlayer::Think( const float dt )
{
	LoadSprite();

	this->now = clock.getElapsedTime().asMilliseconds();

	// Early out here if dead
	if( dead )
	{
		// Keep animation going during death
		sf::IntRect animRect = game->animManager.Animate( currentAnim );
		sprite.setTextureRect( animRect );

		// Check if we should also reset the state
		if( now - deathTime >= resetDelay )
		{
			reset = true;
			game->RemoveLife();
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

	// Fire with delay
	if( direction != sf::Vector2f(0,0) && input.fire && now - lastFire >= fireDelay )
	{
		entityManager->Add( new EntityBullet( sf::Vector2f( hitbox.left + (hitbox.width/2), hitbox.top + (hitbox.height/2) ), direction, this ) );
		lastFire = now;
	}

	Move( move, dt );
}

void EntityPlayer::Die()
{
	//sprite.setColor( sf::Color::Red );
	currentAnim = "player_death";
	dead = true;
	deathTime = (float)now;
}

bool EntityPlayer::CheckDead() const
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
	game->window.draw( shape );
}

void EntityPlayer::HandleCollision( Entity *other )
{
	// Early out if dead
	if( dead )
		return;

	// In most cases, die
	if( dynamic_cast<EntityWall*>( other ) != NULL || dynamic_cast<EntityRobot*>( other ) != NULL || dynamic_cast<EntityBullet*>( other ) != NULL )
	{
		Die();
	}
}

void EntityPlayer::Move( sf::Vector2f move, const float dt ) // Add vector to produce movement
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
	sprite.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );

	// Change animation
	if( move.x > 0 || move.y > 0 || move.x < 0 || move.y < 0 )
		currentAnim = "player_walk";
	else if( input.fire )
		currentAnim = "player_fire";
	else
		currentAnim = "player_stand";

	// Flip sprite based on last horizontal movement
	if( lastHoriz == Directions::W )
	{
		sprite.setScale( -4.f, 4.f );
		// Account for sprite mis-aligning with the hitbox when flipped via negative scale
		sprite.move( sf::Vector2f( hitbox.width, 0 ) );
	}
	else
	{
		sprite.setScale( 4.f, 4.f );
	}

	sf::IntRect animRect = game->animManager.Animate( currentAnim );
	sprite.setTextureRect( animRect );
}