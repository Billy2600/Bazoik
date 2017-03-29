#include <cmath>
#include "entity_robot.h"

EntityRobot::EntityRobot( const sf::Vector2f pos )
{
	hitbox.top = pos.y;
	hitbox.left = pos.x;

	shape.setFillColor( sf::Color::Transparent );
	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1.f );
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
	seePlayer = false;
	moving = false;
	drawHitbox = false;
	currentAnim = "robot_idle";
	hitbox.width = 27;
	hitbox.height = 33;
}

EntityRobot::~EntityRobot()
{

}

void EntityRobot::LoadSprite()
{
	// Load texture if we need to
	if( sprite.getTexture() == NULL )
	{
		sprite.setTexture( game->assetManager.GetTextureRef( "sprites" ) );
		sprite.setScale( sf::Vector2f( SPRITE_SCALE, SPRITE_SCALE ) );
		// Load hitbox based on sprite info
		sf::IntRect animRect = game->animManager.Animate( currentAnim );
		shape.setSize( sf::Vector2f( hitbox.width, hitbox.height ) );
	}
}

void EntityRobot::Think( const float dt )
{
	LoadSprite();
	
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
	sprite.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
	sprite.setTextureRect( game->animManager.Animate( currentAnim ) );

	if( hitbox.left > GAME_WIDTH || hitbox.top > GAME_HEIGHT )
		return;

	// Get vector compared to player and normalize it
	sf::Vector2f playerVec = playerPos - sf::Vector2f( hitbox.left, hitbox.top );
	float playerVecMagnitude = sqrtf( playerVec.x * playerVec.x ) + ( playerVec.y * playerVec.y );
	sf::Vector2f normalizedPlayerVec = playerVec / playerVecMagnitude;

	// Fire
	if( clock.getElapsedTime().asMilliseconds() > ROBOT_FIRE_DELAY && seePlayer )
	{
		entityManager->Add( new EntityBullet( sf::Vector2f( hitbox.left + ( hitbox.width / 2 ), hitbox.top + ( hitbox.height / 2 ) ), normalizedPlayerVec, this ) );
		clock.restart();
	}

	// Move towards player
	if( seePlayer || (moving && game->level < 5) )
	{
		moving = true;
		this->Move( sf::Vector2f(playerVec.x / 7, playerVec.y / 7), dt );
		currentAnim = "robot_walk";
	}
	else
	{
		currentAnim = "robot_idle";
	}

	//if(seePlayer) shape.setFillColor( sf::Color::Cyan );
	//else shape.setFillColor( sf::Color::Blue );
}

void EntityRobot::Draw() const
{
	game->window.draw( sprite );
	game->window.draw( shape );
}

void EntityRobot::HandleCollision( Entity *other )
{
	if( dynamic_cast<EntityBullet*>( other ) != NULL || dynamic_cast<EntityWall*>( other ) != NULL )
	{
		if( !deleteMe ) // Only do this stuff once
		{
			unsigned int prevScore = game->score;
			game->score += 50; 
			// Do we award an extra life?
			if( prevScore % EXTRA_LIFE_SCORE != 0 && game->score % EXTRA_LIFE_SCORE == 0 )
				game->AddLife();
		}

		deleteMe = true;
	}
}

void EntityRobot::SetPlayerPos( sf::Vector2f playerPos )
{
	this->playerPos = playerPos;
}