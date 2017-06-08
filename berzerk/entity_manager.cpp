#include <cmath>
#include "entity_manager.h"
#include "entity_player.h"

EntityManager::EntityManager()
{
	added = false;
	player = NULL;
}

void EntityManager::Add( Entity* entity )
{
	added = true;
	entity->game = game;
	entity->entityManager = this;
	entities.push_back( entity );

	// Single out the player
	if( dynamic_cast<EntityPlayer*>( entity ) != NULL )
	{
		player = entity;
	}
	
	if( dynamic_cast<EntityWall*>( entity ) != NULL )
	{
		walls.push_back( dynamic_cast<EntityWall*>( entity ) );
	}
}

void EntityManager::CheckCollisions()
{
#ifdef _WIN32
	Quadtree quadtree = Quadtree( 0, sf::IntRect( 0, 0, GAME_WIDTH, GAME_HEIGHT ) );

	for( auto entity : entities )
	{
		quadtree.Insert( entity );
	}

	std::vector<Entity*> returnObjects;
#endif
	for( auto entityA : entities )
	{
#ifdef _WIN32
		returnObjects.clear();
		quadtree.Retrieve( returnObjects, entityA );

		for( auto entityB : returnObjects )
#else
		for(auto entityB: entities) // Temp fix	
#endif
		{
			 // Don't bother if both are walls
			if( ( dynamic_cast<EntityWall*>( entityA ) != NULL ) && ( dynamic_cast<EntityWall*>( entityB ) != NULL ) )
			{
				continue;
			}

			// Don't bother if it's a bullet and its owner
			auto bulletCheckA = dynamic_cast<EntityBullet*>( entityA );
			auto bulletCheckB = dynamic_cast<EntityBullet*>( entityB );

			if( (bulletCheckA != NULL && bulletCheckA->owner == entityB)
				|| ( bulletCheckB != NULL && bulletCheckB->owner == entityA ) )
			{
				continue;
			}

			// Don't bother if one is a dead robot
			auto robotCheckA = dynamic_cast<EntityRobot*>( entityA );
			auto robotCheckB = dynamic_cast<EntityRobot*>( entityB );

			if( robotCheckA != NULL && robotCheckA->IsDead()
				|| robotCheckB != NULL && robotCheckB->IsDead() )
			{
				continue;
			}

			// Detect collision
			if( entityA != entityB && entityA->hitbox.intersects(entityB->hitbox) )
			{
				// Tell both objects about the other
				entityA->HandleCollision( entityB );
				entityB->HandleCollision( entityA );
			}
		}
	}
#ifdef _WIN32
	quadtree.Clear();
#endif

	CheckLineOfSight();
}

void EntityManager::CheckLineOfSight()
{
	for( auto entityA : entities )
	{
		for( auto entityB : entities )
		{
			// Make sure we're working with player and robot
			EntityRobot* robot = NULL;
			if( dynamic_cast<EntityRobot*>( entityA ) != NULL ) robot = dynamic_cast<EntityRobot*>( entityA );
			else if( dynamic_cast<EntityRobot*>( entityB ) != NULL ) robot = dynamic_cast<EntityRobot*>( entityB );
			EntityPlayer* player = NULL;
			if( dynamic_cast<EntityPlayer*>( entityA ) != NULL ) player = dynamic_cast<EntityPlayer*>( entityA );
			else if( dynamic_cast<EntityPlayer*>( entityB ) != NULL ) player = dynamic_cast<EntityPlayer*>( entityB );

			std::vector<EntityRobot*> canSee;
			if( player != NULL && robot != NULL )
			{
				int found = false;
				robot->seePlayer = true;
				for( auto wall : walls )
				{
					if( found ) break;
					liang_barsky_clip_window window = { wall->hitbox.left, wall->hitbox.top, wall->hitbox.left + wall->hitbox.width, wall->hitbox.top + wall->hitbox.height };

					int x1 = static_cast<int>( player->hitbox.left + ( player->hitbox.width / 2 ) );
					int y1 = static_cast<int>( player->hitbox.top + ( player->hitbox.height / 2 ) );
					int x2 = static_cast<int>( robot->hitbox.left + ( robot->hitbox.width / 2 ) );
					int y2 = static_cast<int>( robot->hitbox.top + ( robot->hitbox.height / 2 ) );

					found = clip_line( &window, &x1, &y1, &x2, &y2 );
					if( found )
					{
						robot->seePlayer = false;
					}
				}
			}
		}
	}
}

void EntityManager::CheckDelete()
{
	for( unsigned int i = 0; i < entities.size(); i++ )
	{
		if( entities.at( i )->deleteMe )
		{
			// Make sure no bullets has this as an owner (prevent crashes)
			for( auto entity : entities )
			{
				EntityBullet* bullet = dynamic_cast<EntityBullet*>( entity );
				if( bullet != NULL && bullet->owner == entities.at( i ) )
				{
					bullet->RemoveOwner();
				}
			}

			delete entities.at( i );
			entities.erase( entities.begin() + i );
		}
	}
}

void EntityManager::Think( const float dt )
{
	added = false;

	for( auto entity : entities )
	{
		// Tell the robots where the player is
		if( dynamic_cast<EntityRobot*>( entity ) != NULL && player != NULL)
		{
			dynamic_cast<EntityRobot*>( entity )->SetPlayerPos( sf::Vector2f( player->hitbox.left, player->hitbox.top ) );
		}

		entity->Think( dt );
		if( added ) break;
	}

	CheckDelete();
}

void EntityManager::Draw() const
{
	for( auto entity : entities )
	{
		entity->Draw();
	}
}

void EntityManager::MoveAllEntities(sf::Vector2f move, const float dt)
{
	for( auto entity : entities )
	{
		if( dynamic_cast<EntityBullet*>(entity) == NULL )
			entity->Move( move, dt );
	}
}

int EntityManager::GetRobotCount() const
{
	int count = 0;

	for( auto entity : entities )
	{
		if( dynamic_cast<EntityRobot*>( entity ) != NULL )
			count++;
	}

	return count;
}

EntityManager::~EntityManager()
{
	for( auto entity : entities )
	{
		if( static_cast<EntityPlayer*>(entity) == NULL ) // We didn't add the player on the heap
		{
			delete entity;
		}
	}
	entities.clear();
}
