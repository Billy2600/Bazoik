#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class AssetManager
{
private:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;

public:
	// Add/load textures
	void LoadTexture( const std::string &name, const std::string &filename );
	sf::Texture& GetTextureRef( const std::string &name );

	// Add/load fonts
	void LoadFont( const std::string& name, const std::string &filename );
	sf::Font& GetFontRef( const std::string &name );
};