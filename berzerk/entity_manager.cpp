#include <cmath>
#include "entity_manager.h"
#include "entity_player.h"
#include "entity_enemy.h"

EntityManager::EntityManager()
{
	added = false;
	player = NULL;
	game = NULL;
}

void EntityManager::SortByDrawPriority()
{
	std::sort(entities.begin(), entities.end(), DrawPriority());
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

	SortByDrawPriority();
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

#ifndef _WIN32
			if( entityA == entityB )
            {
                continue;
            }
#endif

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

			// Same with sword
			auto swordCheckA = dynamic_cast<EntitySword*>(entityA);
			auto swordCheckB = dynamic_cast<EntitySword*>(entityB);
			if ( (swordCheckA != NULL && swordCheckA->owner == entityB)
				|| (swordCheckB != NULL && swordCheckB->owner == entityA) )
			{
				continue;
			}

			// Detect collision
			if( (entityA != NULL && entityB != NULL) && (entityA != entityB && entityA->hitbox.intersects(entityB->hitbox) ) )
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
			// Make sure we're working with player and enemy
			EntityEnemy* enemy = NULL;
			if( dynamic_cast<EntityEnemy*>( entityA ) != NULL ) enemy = dynamic_cast<EntityEnemy*>( entityA );
			else if( dynamic_cast<EntityEnemy*>( entityB ) != NULL ) enemy = dynamic_cast<EntityEnemy*>( entityB );
			EntityPlayer* player = NULL;
			if( dynamic_cast<EntityPlayer*>( entityA ) != NULL ) player = dynamic_cast<EntityPlayer*>( entityA );
			else if( dynamic_cast<EntityPlayer*>( entityB ) != NULL ) player = dynamic_cast<EntityPlayer*>( entityB );

			std::vector<EntityEnemy*> canSee;
			if( player != NULL && enemy != NULL )
			{
				int found = false;
				enemy->seePlayer = true;
				for( auto wall : walls )
				{
					if( found ) break;
					liang_barsky_clip_window window = { wall->hitbox.left, wall->hitbox.top, wall->hitbox.left + wall->hitbox.width, wall->hitbox.top + wall->hitbox.height };

					int x1 = static_cast<int>( player->hitbox.left + ( player->hitbox.width / 2 ) );
					int y1 = static_cast<int>( player->hitbox.top + ( player->hitbox.height / 2 ) );
					int x2 = static_cast<int>( enemy->hitbox.left + ( enemy->hitbox.width / 2 ) );
					int y2 = static_cast<int>( enemy->hitbox.top + ( enemy->hitbox.height / 2 ) );

					found = clip_line( &window, &x1, &y1, &x2, &y2 );
					if( found )
					{
						enemy->seePlayer = false;
					}
				}
			}
		}
	}
}

void EntityManager::CheckDelete()
{
	bool itemDeleted = false;
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
			itemDeleted = true;
		}
	}

	// Re-sort if we deleted anything
	if (itemDeleted)
	{
		SortByDrawPriority();
	}
}

void EntityManager::Think( const float dt )
{
	added = false;

	for( auto entity : entities )
	{
		// Tell the enemies where the player is
		if( dynamic_cast<EntityEnemy*>( entity ) != NULL && player != NULL)
		{
			dynamic_cast<EntityEnemy*>( entity )->SetPlayerPos( sf::Vector2f( player->hitbox.left, player->hitbox.top ) );
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
		//if ( dynamic_cast<EntityRobot*>( entity ) != NULL )
		//	dynamic_cast<EntityRobot*>( entity )->Stop(); // Stop all robots

			entity->Move( move, dt );
	}
}

bool EntityManager::TryMove(Entity* entity, const sf::Vector2f move, const float dt, const bool ignoreEnemies) const
{
	Quadtree quadtree = Quadtree(0, sf::IntRect(0, 0, GAME_WIDTH, GAME_HEIGHT));

	for (auto entity : entities)
	{
		quadtree.Insert(entity);
	}

	std::vector<Entity*> returnObjects;

	sf::FloatRect newHitbox = entity->hitbox;
	newHitbox.top += move.y * dt;
	newHitbox.left += move.x * dt;

	returnObjects.clear();
	quadtree.Retrieve(returnObjects, entity); // Get possible collisions for entity passed in

	for (auto entityB : returnObjects)
	{
		if (ignoreEnemies && entityB != NULL && dynamic_cast<EntityEnemy*>(entityB) != NULL)
			continue;

		// Detect collision
		if ((entity != NULL && entityB != NULL) && (entity != entityB && newHitbox.intersects(entityB->hitbox)))
		{
			entity->HandleCollision(entityB);
			entityB->HandleCollision(entity);
			if (entityB->deleteMe) // If this entity is now trying to delete itself, we can keep moving
				continue;

			return false; // You cannot move
		}
	}

	quadtree.Clear();

	return true; // No possible collisions detected, you can move
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
