#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "entity.h"

#define QUAD_MAX_OBJECTS 10
#define QUAD_MAX_LEVELS 5

class Quadtree
{
private:
	int level;
	std::vector<Entity*> objects;
	sf::IntRect bounds;
	Quadtree *nodes[4];

public:
	Quadtree(int level, sf::IntRect bounds);
	void Clear();
	void Split();
	int GetIndex( Entity* entity );
	void Insert( Entity* entity );
	void Retrieve( std::vector<Entity*> &returnObjects, Entity* entity );
	~Quadtree();
};

