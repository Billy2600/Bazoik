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
	int level; // Level demo was recorded on/should be played on
	std::vector<sf::IntRect> walls;
	std::vector<sf::Vector2f> robotPositions;
	bool done;

public:
	Demo(const int level = 1);
	// Record and Play are run every frame
	void Record( const PlayerInput input );
	PlayerInput Play();
	// You must provide path and filename
	void LoadFromFile( const std::string path );
	void SaveToFile( const std::string path );
	void SetWalls( const std::vector<sf::IntRect> walls );
	std::vector<sf::IntRect> GetWalls() const;
	void SetRobotPositions( const std::vector<sf::Vector2f> positions );
	std::vector<sf::Vector2f> GetRobotPositions() const;
	void SetLevel( const int level );
	int GetLevel() const;
	bool IsDone() const; // Did the demo finish playing?
};

