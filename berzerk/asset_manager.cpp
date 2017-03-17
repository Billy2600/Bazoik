#include "asset_manager.h"

void AssetManager::LoadTexture( const std::string& name, const std::string &filename )
{
	if( textures.count( name ) == 0 )
	{
		sf::Texture tex;
		tex.loadFromFile( filename );
		this->textures[name] = tex;
	}
}

sf::Texture& AssetManager::GetTextureRef( const std::string& name )
{
	return this->textures.at( name );
}

void AssetManager::LoadFont( const std::string& name, const std::string &filename )
{
	if( fonts.count( name ) == 0 )
	{
		sf::Font font;
		font.loadFromFile( filename );
		this->fonts[name] = font;
	}
}

sf::Font& AssetManager::GetFontRef( const std::string& name )
{
	return this->fonts.at( name );
}

void AssetManager::LoadSound( const std::string& name, const std::string &filename )
{
	if( sounds.count( name ) == 0 )
	{
		sf::SoundBuffer buffer;
		buffer.loadFromFile( filename );
		this->sounds[name] = buffer;
	}
}

sf::SoundBuffer& AssetManager::GetSoundRef( const std::string &name, bool random )
{
	if( random )
	{
		std::string versions[3] = { "_hi", "_low", "_mid" };
		std::uniform_int_distribution<unsigned int> rand( 0, 2 );
		unsigned int randSound = rand( rngEngine );

		return this->sounds.at( name + versions[randSound] );
	}
	else
	{
		return this->sounds.at( name );
	}
}