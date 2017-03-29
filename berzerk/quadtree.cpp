#include "quadree.h"

Quadtree::Quadtree( int level, sf::IntRect bounds )
{
	this->level = level;
	this->bounds = bounds;
	for( auto node : nodes )
		node = NULL;
}

void Quadtree::Clear()
{
	objects.clear();

	for( Quadtree* node : nodes )
	{
		if( node != NULL )
		{
			node->Clear();
			delete node;
			node = NULL;
		}
	}
}

void Quadtree::Split()
{
	int subWidth = bounds.width / 2;
	int subHeight = bounds.height / 2;
	int x = bounds.left;
	int y = bounds.top;

	nodes[0] = new Quadtree( level + 1, sf::IntRect( x + subWidth, y, subWidth, subHeight ) );
	nodes[1] = new Quadtree( level + 1, sf::IntRect( x, y, subWidth, subHeight ) );
	nodes[2] = new Quadtree( level + 1, sf::IntRect( x, y + subHeight, subWidth, subHeight ) );
	nodes[3] = new Quadtree( level + 1, sf::IntRect( x + subWidth, y + subHeight, subWidth, subHeight ) );
}

int Quadtree::GetIndex( Entity* entity )
{
	int index = -1;
	float verticalMidpoint = (float)bounds.left + ( bounds.width / 2 );
	float horizontalMidpoint = (float)bounds.top + ( bounds.height / 2 );
	sf::FloatRect hitbox = entity->hitbox;

	bool topQuadrant = ( hitbox.top < horizontalMidpoint && hitbox.top + hitbox.height < horizontalMidpoint );
	bool bottomQuadrant = ( hitbox.top > horizontalMidpoint );

	if( hitbox.left < verticalMidpoint && hitbox.left + hitbox.width < verticalMidpoint )
	{
		if( topQuadrant )
			index = 1;
		else if( bottomQuadrant )
			index = 2;
	}
	else if( hitbox.left > verticalMidpoint )
	{
		if( topQuadrant )
			index = 0;
		else if( bottomQuadrant )
			index = 3;
	}

	return index;
}

void Quadtree::Insert( Entity* entity )
{
	if( nodes[0] != NULL )
	{
		int index = GetIndex( entity );

		if( index != -1 )
		{
			nodes[index]->Insert( entity );

			return;
		}
	}

	objects.push_back( entity );

	if( objects.size() > QUAD_MAX_OBJECTS && level < QUAD_MAX_LEVELS )
	{
		if( nodes[0] == NULL )
			Split();

		unsigned int i = 0;
		while( i < objects.size() )
		{
			int index = GetIndex( objects[i] );
			if( index != -1 )
			{
				nodes[index]->Insert( objects[i] );
				objects.erase( objects.begin() + i );
			}
			else
			{
				i++;
			}
		}
	}
}

void Quadtree::Retrieve( std::vector<Entity*> &returnObjects, Entity* entity )
{
	int index = GetIndex( entity );
	if( index != -1 && nodes[0] != NULL )
	{
		nodes[index]->Retrieve( returnObjects, entity );
	}

	returnObjects.insert( returnObjects.end(), objects.begin(), objects.end() );
}

Quadtree::~Quadtree()
{

}
