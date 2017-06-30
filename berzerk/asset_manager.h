#pragma once
#include <map>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define MAX_SOUNDS 10 // Maximum number of sounds that can play at the same time

class AssetManager
{
private:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	std::map<std::string, sf::SoundBuffer> soundBuffers;
	sf::Sound sounds[MAX_SOUNDS];
	std::mt19937 rngEngine = std::mt19937( (unsigned int)time( 0 ) ); // To randomly pick sounds

public:

	AssetManager();
	// Add/load textures
	void LoadTexture( const std::string &name, const std::string &filename );
	sf::Texture& GetTextureRef( const std::string &name );

	// Add/load fonts
	void LoadFont( const std::string& name, const std::string &filename );
	sf::Font& GetFontRef( const std::string &name );

	// Add/load sounds
	void LoadSound( const std::string& name, const std::string &filename );
	void PlaySound( const std::string &name, const bool random = false, const float volume = 100 ); // Choose whether it'll randomly give you low/med/hi version

	~AssetManager();
};