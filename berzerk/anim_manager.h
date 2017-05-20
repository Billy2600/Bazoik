#pragma once
#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "pugixml.hpp"

#define ANIM_FRAME_DELAY 100 // Frame delay (as milliseconds)

class AnimManager
{
private:
	std::map< std::string, std::vector<sf::IntRect> > animations;
	std::map<std::string, sf::Uint8> frameCounters;
	sf::Clock clock;
	std::map<std::string, sf::Int32> lastTime; // Last time we changed frame

public:
	AnimManager();
	void LoadFromFile( const std::string &filename ); // Load animation data from file
	sf::IntRect Animate( const std::string &name, const bool stopOnLastFrame = false ); // Move to next frame of specified anim; will return coordinates
	bool IsEmpty() const; // Will return true if animation map is empty
	bool IsEmpty( const std::string &name ); // Will return true if specified animation is empty
	~AnimManager();
};

