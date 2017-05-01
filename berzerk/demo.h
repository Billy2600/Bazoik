#pragma once
#include <vector>
#include <string>
#include "entity_player.h"
#include "pugixml.hpp"

struct FrameInput
{
	PlayerInput input;
	sf::Int32 time;
};

class Demo
{
private:
	sf::Clock clock;
	std::vector<FrameInput> inputs;
	int iterator;

public:
	Demo();
	// Record and Play are run every frame
	void Record( const PlayerInput input );
	PlayerInput Play();
	// You must provide path and filename
	void LoadFromFile( const std::string path );
	void SaveToFile( const std::string path );
};

