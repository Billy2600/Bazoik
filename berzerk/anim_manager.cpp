#include "anim_manager.h"

std::map< std::string, std::vector<sf::IntRect> > AnimManager::animations;

AnimManager::AnimManager()
{
	
}

AnimManager::~AnimManager()
{

}

void AnimManager::LoadFromFile( const std::string &filename )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( filename.c_str() );
	if( !result ) // Error check
	{
		return;
	}

	pugi::xml_node animNodes = doc.child( "animations" );
	if ( animNodes == NULL )
		return;

	for( pugi::xml_node anim : animNodes.children( "anim" ) )
	{
		if ( anim == NULL )
			return;

		// AnimManagers
		std::string n( anim.attribute( "name" ).value() );
		//std::string value( key.first_child().value() );
		//frameCounters[n] = 0;

		// Frames
		for( pugi::xml_node frame : anim.children( "frame" ) )
		{
			if ( frame == NULL )
				return;

			int x = std::stoi( frame.attribute( "x" ).value() );
			int y = std::stoi( frame.attribute( "y" ).value() );
			int w = std::stoi( frame.attribute( "w" ).value() );
			int h = std::stoi( frame.attribute( "h" ).value() );
			animations[n].push_back( sf::IntRect( x, y, w, h ) );
		}
	}
}

sf::IntRect AnimManager::Animate( const std::string &name, const bool stopOnLastFrame )
{
	if( clock.getElapsedTime().asMilliseconds() > lastTime[name] + ANIM_FRAME_DELAY )
	{
		frameCounters[name]++;
		if ( frameCounters[name] >= animations[name].size() )
		{
			if ( stopOnLastFrame )
				return animations[name].back();
			else
				frameCounters[name] = 0;
		}

		lastTime[name] = clock.getElapsedTime().asMilliseconds();
	}

	if( !IsAnimEmpty( name ) )
		return animations[name].at( frameCounters[name] );
	else
		return sf::IntRect( 0, 0, 0, 0 );
}

void AnimManager::ResetAnim( const std::string &name )
{
	frameCounters[name] = 0;
}

bool AnimManager::IsEmpty() const
{
	return animations.empty();
}

bool AnimManager::IsAnimEmpty( const std::string &name )
{
	return animations[name].empty();
}
