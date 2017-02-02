#include "asset_manager.h"

void AssetManager::LoadTexture( const std::string& name, const std::string &filename )
{
	sf::Texture tex;
	tex.loadFromFile( filename );
	this->textures[name] = tex;
}

sf::Texture& AssetManager::GetTextureRef( const std::string& name )
{
	return this->textures.at( name );
}

void AssetManager::LoadFont( const std::string& name, const std::string &filename )
{
	sf::Font font;
	font.loadFromFile( filename );
	this->fonts[name] = font;
}

sf::Font& AssetManager::GetFontRef( const std::string& name )
{
	return this->fonts.at( name );
}