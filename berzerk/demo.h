#pragma once
#include <vector>
#include <string>
#include "entity_player.h"
#include "pugixml.hpp"

struct FrameInput
{
	PlayerInput input;
	unsigned int frame;
};

class Demo
{
private:
	std::vector<FrameInput> inputs;
	int iterator; // Iterator for input vector
	unsigned int frame; // Current frame
	std::vector<sf::IntRect> walls;

public:
	Demo();
	// Record and Play are run every frame
	void Record( const PlayerInput input );
	PlayerInput Play();
	// You must provide path and filename
	void LoadFromFile( const std::string path );
	void SaveToFile( const std::string path );
	void SetWalls( const std::vector<sf::IntRect> walls );
};

