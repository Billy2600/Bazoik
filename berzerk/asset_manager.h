#pragma once
#include <map>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AssetManager
{
private:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	std::map < std::string, sf::SoundBuffer> sounds;
	std::mt19937 rngEngine = std::mt19937( (unsigned int)time( 0 ) ); // To randomly pick sounds

public:
	// Add/load textures
	void LoadTexture( const std::string &name, const std::string &filename );
	sf::Texture& GetTextureRef( const std::string &name );

	// Add/load fonts
	void LoadFont( const std::string& name, const std::string &filename );
	sf::Font& GetFontRef( const std::string &name );

	// Add/load sounds
	void LoadSound( const std::string& name, const std::string &filename );
	sf::SoundBuffer& GetSoundRef( const std::string &name, bool random = false ); // Choose whether it'll randomly give you low/med/hi version
};