#include "asset_manager.h"

AssetManager::AssetManager()
{

}

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
	if( soundBuffers.count( name ) == 0 )
	{
		this->soundBuffers[name].loadFromFile( filename );
	}
}

void AssetManager::PlaySound( const std::string &name, const bool random, const float volume )
{
	sf::Sound *sfx = NULL;
	// Choose first sound that's not playing
	for ( int i = 0; i < MAX_SOUNDS; i++ )
	{
		if ( this->sounds[i].getStatus() != sf::SoundSource::Status::Playing )
		{
			sfx = &this->sounds[i];
			break;
		}
	}
	if ( sfx == NULL )
		return;
	
	if ( random )
	{
		std::string versions[3] = { "_hi", "_low", "_mid" };
		std::uniform_int_distribution<unsigned int> rand( 0, 2 );
		unsigned int randSound = rand( rngEngine );

		sfx->setBuffer( this->soundBuffers.at( name + versions[randSound] ) );
	}
	else
	{
		sfx->setBuffer( this->soundBuffers.at( name ) );
	}

	sfx->setVolume( volume );
	sfx->play();
}

AssetManager::~AssetManager()
{
	for ( int i = 0; i < MAX_SOUNDS; i++ )
	{
		this->sounds[i].stop();
		this->sounds[i].resetBuffer();
	}
}