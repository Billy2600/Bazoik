#include "asset_manager.h"
#include "error_log.h"

AssetManager::AssetManager()
{

}

void AssetManager::LoadTexture( const std::string& name, const std::string &filename )
{
	ErrorLog log;
	log.Write( "Loading texture " + name + " from file" );

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
	ErrorLog log;
	log.Write( "Loading font " + name + " from file" );

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

void AssetManager::LoadSound( const std::string &name, const std::string &filename )
{
	ErrorLog log;
	log.Write( "Loading sound " + name + " from file" );

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

void AssetManager::StopSound( const std::string &name )
{
	for ( int i = 0; i < MAX_SOUNDS; i++ )
	{
		if ( soundBuffers.find( name ) == soundBuffers.end() )
			return; // Early out if we couldn't find one with that name

		if ( sounds[i].getBuffer() != NULL && sounds[i].getBuffer() == &soundBuffers.at( name ) )
		{
			sounds[i].stop();
			return;
		}
	}
}

void AssetManager::StopAllSounds()
{
	for ( int i = 0; i < MAX_SOUNDS; i++ )
	{
		this->sounds[i].stop();
	}
}

AssetManager::~AssetManager()
{
	for ( int i = 0; i < MAX_SOUNDS; i++ )
	{
		this->sounds[i].stop();
		this->sounds[i].resetBuffer();
	}
}