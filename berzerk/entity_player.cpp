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
	hitbox.width = 16.f;
	hitbox.height = 32.f;
	clock.restart();
	hurt = false;
	mercyInvincible = false;
	lastHurt = 0;
	dead = false;
	reset = false;
	lastFire = 0;
	deathTime = 0.f;
	now = clock.getElapsedTime().asMilliseconds();
	drawHitbox = false;
	health = 5;
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
		//sprite.setScale( sf::Vector2f( SPRITE_SCALE, SPRITE_SCALE ) );
		currentAnim = "player_stand";
		// Load hitbox based on sprite info
		sf::IntRect animRect = animManager.Animate( currentAnim );
#ifdef _DEBUG
		shape.setSize( sf::Vector2f( hitbox.width, hitbox.height ) );
#endif
	}
}

void EntityPlayer::ChooseFireAnim( sf::Vector2f direction )
{
	if( direction == sf::Vector2f( -1, -1 ) )
		currentAnim = "player_fire_nw";
	else if( direction == sf::Vector2f( 0, -1 ) )
		currentAnim = "player_fire_n";
	else if( direction == sf::Vector2f( 1, -1 ) )
		currentAnim = "player_fire_ne";
	else if( direction == sf::Vector2f( -1, 0 ) )
		currentAnim = "player_fire_w";
	else if( direction == sf::Vector2f( 1, 0 ) )
		currentAnim = "player_fire_e";
	else if( direction == sf::Vector2f( -1, 1 ) )
		currentAnim = "player_fire_sw";
	else if( direction == sf::Vector2f( 0, 1 ) )
		currentAnim = "player_fire_s";
	else if( direction == sf::Vector2f( 1, 1) )
		currentAnim = "player_fire_se";
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
		move.x -= realSpeed;
		direction.x = -1;
		lastHoriz = Directions::W;
	}
	else if( input.right )
	{
		move.x += realSpeed;
		direction.x = 1;
		lastHoriz = Directions::E;
	}
	else
	{
		direction.x = 0;
	}

	if( input.up )
	{
		move.y -= realSpeed;
		direction.y  = -1;
	}
	else if( input.down )
	{
		move.y += realSpeed;
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
	bool firing = input.fireUp || input.fireLeft || input.fireRight || input.fireDown;
	auto fireDirection = sf::Vector2f(0, 0);

	if(firing && now - lastFire >= fireDelay )
	{
		if (input.fireLeft)
			fireDirection.x = -1;
		else if (input.fireRight)
			fireDirection.x = 1;

		if (input.fireUp)
			fireDirection.y = -1;
		else if (input.fireDown)
			fireDirection.y = 1;

		entityManager->Add( new EntityBullet( sf::Vector2f( hitbox.left + (hitbox.width/2), hitbox.top + (hitbox.height/3) ), fireDirection, this ) );
		lastFire = now;
		game->assetManager.PlaySound( "shoot" );
	}

	if (hurt)
	{
		move = moveHurtVec;
		sprite.setColor(sf::Color::Red);

		if (now - lastHurt >= hurtDelay)
			hurt = false;
	}
	else
	{
		sprite.setColor(sf::Color::White);
	}

	if (mercyInvincible)
	{
		if (now - lastHurt >= mercyInvincibilityDelay)
			mercyInvincible = false;
	}

	Move( move, dt );
	// Change animation
	if( move.x > 0 || move.y > 0 || move.x < 0 || move.y < 0 )
		currentAnim = "player_walk";
	else if(firing)
		ChooseFireAnim(fireDirection);
	else
		currentAnim = "player_stand";
}

void EntityPlayer::Hurt(sf::Vector2f attacker)
{
	if (hurt)
		return;

	if (health <= 1)
	{
		Die();
		return;
	}

	lastHurt = now;
	hurt = true;
	mercyInvincible = true;
	moveHurtVec = GetMoveTowardsVec(attacker, 100.f);
	// Move away from it instead
	moveHurtVec.x = moveHurtVec.x * -1;
	moveHurtVec.y = moveHurtVec.y * -1;
	health -= 1;
}

void EntityPlayer::Die()
{
	health = 0;
	//sprite.setColor( sf::Color::Red );
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

sf::Int32 EntityPlayer::GetHealth() const
{
	return health;
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
	if (mercyInvincible && now % 2 == 0)
		return;

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
	else if (dynamic_cast<EntityRobot*>( other ) != NULL && !mercyInvincible )
	{
		Hurt(sf::Vector2f(other->hitbox.left, other->hitbox.top));
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

	// Flip sprite based on last horizontal movement if walking
	if( lastHoriz == Directions::W && (currentAnim == "player_stand" || currentAnim == "player_walk") )
	{
		//sprite.setScale( -SPRITE_SCALE, SPRITE_SCALE );
		sprite.setScale( -1, 1 );
		// Account for sprite mis-aligning with the hitbox when flipped via negative scale
		sprite.move( sf::Vector2f( hitbox.width, 0 ) );
	}
	else
	{
		//sprite.setScale( SPRITE_SCALE, SPRITE_SCALE );
		sprite.setScale( 1, 1 );
	}

	sprite.setTextureRect( animManager.Animate( currentAnim ) );
}

EntityPlayer::~EntityPlayer()
{

}