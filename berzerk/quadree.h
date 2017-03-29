#pragma once
#include <vector>
#include <array>
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
	std::array<Quadtree*, 4> nodes;

public:
	Quadtree(int level, sf::IntRect bounds);
	void Clear();
	void Split();
	int GetIndex( Entity* entity );
	void Insert( Entity* entity );
	void Retrieve( std::vector<Entity*> &returnObjects, Entity* entity );
	~Quadtree();
};

