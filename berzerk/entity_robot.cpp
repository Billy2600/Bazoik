#include <cmath>
#include "entity_robot.h"

EntityRobot::EntityRobot( const sf::Vector2f pos )
{
	hitbox.top = pos.y;
	hitbox.left = pos.x;
#ifdef _DEBUG
	shape.setFillColor( sf::Color::Transparent );
	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1.f );
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
#endif
	seePlayer = false;
	moving = false;
	drawHitbox = false;
	currentAnim = "robot_idle";
	hitbox.width = 27;
	hitbox.height = 33;

	stopIfSeePlayer = false;
	movementSpeed = 50;
	fireDelay = 3000;
	statsLoaded = false;
	sprite.setColor( sf::Color::Red );
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
#ifdef _DEBUG
		shape.setSize( sf::Vector2f( hitbox.width, hitbox.height ) );
#endif
	}
}

void EntityRobot::LoadStatsFromFile()
{
	if( statsLoaded )
		return;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( "assets/robotstats.xml" );
	if( !result ) // Error check
	{
		return;
	}

	pugi::xml_node levelNodes = doc.child( "levels" );

	for( pugi::xml_node level : levelNodes.children( "level" ) )
	{
		if( game->level >= std::stoi( level.attribute( "min" ).value() ) )
		{
			if( std::stoi( level.attribute( "stop_if_see_player" ).value() ) == 0 )
				stopIfSeePlayer = false;
			else
				stopIfSeePlayer = true;

			movementSpeed = std::stof( level.attribute( "speed" ).value() );
			fireDelay = std::stoi( level.attribute( "firedelay" ).value() );

			pugi::xml_node color = level.child( "color" );
			int r = std::stoi( color.attribute( "r" ).value() );
			int g = std::stoi( color.attribute( "g" ).value() );
			int b = std::stoi( color.attribute( "b" ).value() );
			sprite.setColor( sf::Color( r, g, b ) );
		}
	}
}

void EntityRobot::Think( const float dt )
{
	LoadSprite();
	LoadStatsFromFile();
	
#ifdef _DEBUG
	shape.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
#endif
	sprite.setPosition( sf::Vector2f( hitbox.left, hitbox.top ) );
	sprite.setTextureRect( game->animManager.Animate( currentAnim ) );

	if( hitbox.left > GAME_WIDTH || hitbox.top > GAME_HEIGHT )
		return;

	// Get vector compared to player and normalize it
	sf::Vector2f playerVec = playerPos - sf::Vector2f( hitbox.left, hitbox.top );
	float playerVecMagnitude = sqrtf( playerVec.x * playerVec.x ) + ( playerVec.y * playerVec.y );
	sf::Vector2f normalizedPlayerVec = playerVec / playerVecMagnitude;

	// Fire
	if( clock.getElapsedTime().asMilliseconds() > fireDelay && seePlayer )
	{
		entityManager->Add( new EntityBullet( sf::Vector2f( hitbox.left + ( hitbox.width / 2 ), hitbox.top + ( hitbox.height / 2 ) ), normalizedPlayerVec, this ) );
		clock.restart();
	}

	// Move towards player
	if( seePlayer || (moving && !stopIfSeePlayer) )
	{
		moving = true;

		sf::Vector2f moveVec;
		float angle = atan2f( normalizedPlayerVec.y, normalizedPlayerVec.x );
		moveVec.x = cosf( angle ) * movementSpeed;
		moveVec.y = sinf( angle ) * movementSpeed;

		this->Move( moveVec, dt );
		currentAnim = "robot_walk";
	}
	else
	{
		currentAnim = "robot_idle";
	}
}

void EntityRobot::Draw() const
{
	game->window.draw( sprite );
#ifdef _DEBUG
	game->window.draw( shape );
#endif
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