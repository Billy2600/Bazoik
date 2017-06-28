#include "asset_manager.h"

AssetManager::AssetManager()
{
	if ( SDL_Init( SDL_INIT_AUDIO ) < 0 )
	{
		printf( "Could not init SDL audio\n" );
	}

	if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "Could not init SDL_mixer" );
	}
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
	if( sounds.count( name ) == 0 )
	{
		Mix_Chunk *chunk = Mix_LoadWAV( filename.c_str() );
		if ( chunk == NULL )
		{
			printf( "Failed to load sound effect %s\n", filename );
		}

		this->sounds[name] = chunk;
	}
}

void AssetManager::PlaySound( const std::string &name, bool random, Uint8 volume )
{
	if( random )
	{
		std::string versions[3] = { "_hi", "_low", "_mid" };
		std::uniform_int_distribution<unsigned int> rand( 0, 2 );
		unsigned int randSound = rand( rngEngine );

		this->sounds.at( name + versions[randSound] )->volume = volume;
		Mix_PlayChannel( -1, this->sounds.at( name + versions[randSound] ) , 0 );
	}
	else
	{
		Mix_PlayChannel( -1, this->sounds.at( name ), 0 );
	}
}

AssetManager::~AssetManager()
{
	for ( auto sound : sounds )
	{
		Mix_FreeChunk( sound.second );
	}
	Mix_Quit();
	SDL_Quit();
}