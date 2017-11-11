#pragma once
#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "pugixml.hpp"
#include "error_log.h"

#define ANIM_FRAME_DELAY 100 // Frame delay (as milliseconds)

class AnimManager
{
private:
	static std::map< std::string, std::vector<sf::IntRect> > animations;
	std::map<std::string, sf::Uint8> frameCounters;
	sf::Clock clock;
	std::map<std::string, sf::Int32> lastTime; // Last time we changed frame

public:
	AnimManager();
	static void LoadFromFile( const std::string &filename ); // Load animation data from file
	sf::IntRect Animate( const std::string &name, const bool stopOnLastFrame = false ); // Move to next frame of specified anim; will return coordinates
	void ResetAnim( const std::string &name ); // Manually reset an animation
	bool IsEmpty() const; // Will return true if animation map is empty
	bool IsAnimEmpty( const std::string &name ); // Will return true if specified animation is empty
	~AnimManager();
};

