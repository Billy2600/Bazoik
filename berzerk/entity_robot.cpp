#include <cmath>
#include "entity_robot.h"

EntityRobot::EntityRobot( const sf::Vector2f pos, const RobotStats stats )
{
	hitbox.top = pos.y;
	hitbox.left = pos.x;
#ifdef _DEBUG
	shape.setFillColor( sf::Color::Transparent );
	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1.f );
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
#endif
	hits = 0;
	dead = false;
	seePlayer = false;
	moving = false;
	drawHitbox = false;
	lastFire = 0;
	deathTime = 0;
	currentAnim = "robot_idle";
	hitbox.width = 32 * stats.scale;
	hitbox.height = 32 * stats.scale;

	this->stats = stats;
	sprite.setColor( stats.color );
}

void EntityRobot::LoadSprite()
{
	// Load texture if we need to
	if( sprite.getTexture() == NULL )
	{
		sprite.setTexture( game->assetManager.GetTextureRef( "sprites" ) );
		sf::IntRect animRect = animManager.Animate( currentAnim );
		sprite.setScale( stats.scale, stats.scale );
		subSprite = sprite;
		subSprite.setColor( sf::Color::White );
#ifdef _DEBUG
		shape.setSize( sf::Vector2f( hitbox.width, hitbox.height ) );
#endif
	}
}

void EntityRobot::Think( const float dt )
{
	sf::Int32 now = clock.getElapsedTime().asMilliseconds();
	LoadSprite();
	
#ifdef _DEBUG
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
#endif
	sprite.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
	sprite.setTextureRect( animManager.Animate( currentAnim, ( currentAnim == "robot_death" ) ) );
	subSprite.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
	subSprite.setTextureRect( animManager.Animate( subAnim, ( subAnim == "robot_explosion" ) ) );

	if( dead && now - deathTime < deathDelay )
		return;
	else if( dead && now - deathTime >= deathDelay )
	{
		deleteMe = true;
		animManager.ResetAnim( "robot_death" );
		animManager.ResetAnim( "robot_explosion" );
		return;
	}

	if( hitbox.left > GAME_WIDTH || hitbox.top > GAME_HEIGHT )
		return;

	// Get vector compared to player and normalize it
	sf::Vector2f playerVec = playerPos - sf::Vector2f( hitbox.left, hitbox.top );
	float playerVecMagnitude = sqrtf( playerVec.x * playerVec.x ) + ( playerVec.y * playerVec.y );
	sf::Vector2f normalizedPlayerVec = playerVec / playerVecMagnitude;

	// Fire
	if( now - lastFire >= stats.fireDelay && seePlayer && stats.canShoot )
	{
		entityManager->Add( new EntityBullet( sf::Vector2f( hitbox.left + ( hitbox.width / 2 ), hitbox.top + ( hitbox.height / 2 ) ), normalizedPlayerVec, this ) );
		lastFire = now;
		game->assetManager.PlaySound( "shoot" );
	}

	// Move towards player
	if( seePlayer || (moving && !stats.stopIfSeePlayer) )
	{
		moving = true;

		sf::Vector2f moveVec;
		float angle = atan2f( normalizedPlayerVec.y, normalizedPlayerVec.x );
		moveVec.x = cosf( angle ) * stats.movementSpeed;
		moveVec.y = sinf( angle ) * stats.movementSpeed;

		this->Move( moveVec, dt );
		if ( stats.movementSpeed > 1 ) // Remain in idle anim at slow speeds
		{
			currentAnim = "robot_walk";
			subAnim = "robot_eye";
		}
	}
	else
	{
		currentAnim = "robot_idle";
		subAnim = "robot_eye";
	}
}

void EntityRobot::Draw() const
{
	game->window.draw( sprite );
	game->window.draw( subSprite );
#ifdef _DEBUG
	game->window.draw( shape );
#endif
}

void EntityRobot::HandleCollision( Entity *other )
{
	if( dynamic_cast<EntityBullet*>( other ) != NULL || dynamic_cast<EntityWall*>( other ) != NULL )
	{   
		hits++;
		if( !dead ) // Only do this stuff once
		{
			if ( hits >= (int)stats.scale )
			{
				unsigned int prevScore = game->score;
				game->score += 50;
				currentAnim = "robot_death";
				subAnim = "robot_explosion";
				deathTime = clock.getElapsedTime().asMilliseconds();
				// Do we award an extra life?
				if ( prevScore % EXTRA_LIFE_SCORE != 0 && game->score % EXTRA_LIFE_SCORE == 0 )
					game->AddLife();
				game->assetManager.PlaySound( "robot_die" );
				dead = true;
			}
			else
			{
				sf::Color prevColor = sprite.getColor();
				float factor = stats.scale / hits;
				sprite.setColor( sf::Color( prevColor.r / factor, prevColor.g / factor, prevColor.b / factor) );
			}
		}
	}
}

void EntityRobot::SetPlayerPos( sf::Vector2f playerPos )
{
	this->playerPos = playerPos;
}

bool EntityRobot::IsDead() const
{
	return dead;
}

void EntityRobot::Move( sf::Vector2f move, const float dt )
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
	const sf::Vector2f newPos( hitbox.left, hitbox.top );
	sprite.setPosition( newPos );
	subSprite.setPosition( newPos );
#ifdef _DEBUG
	shape.setPosition( newPos );
#endif
}

void EntityRobot::Stop()
{
	moving = false;
}

EntityRobot::~EntityRobot()
{
	
}
