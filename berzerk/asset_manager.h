#pragma once
#include <map>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SDL.h>
#undef main // We don't care about SDL's main
#include <SDL_mixer.h>

class AssetManager
{
private:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	std::map < std::string, Mix_Chunk*> sounds;
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
	void PlaySound( const std::string &name, bool random = false, Uint8 volume = 128 ); // Choose whether it'll randomly give you low/med/hi version

	~AssetManager();
};