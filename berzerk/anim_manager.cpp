#include "anim_manager.h"

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

	for( pugi::xml_node anim : animNodes.children( "anim" ) )
	{
		// AnimManagers
		std::string n( anim.attribute( "name" ).value() );
		//std::string value( key.first_child().value() );
		frameCounters[n] = 0;

		// Frames
		for( pugi::xml_node frame : anim.children( "frame" ) )
		{
			int x = std::stoi( frame.attribute( "x" ).value() );
			int y = std::stoi( frame.attribute( "y" ).value() );
			int w = std::stoi( frame.attribute( "w" ).value() );
			int h = std::stoi( frame.attribute( "h" ).value() );
			animations[n].push_back( sf::IntRect( x, y, w, h ) );
		}
	}
}

sf::IntRect AnimManager::Animate( const std::string &name )
{
	if( clock.getElapsedTime().asMilliseconds() > ANIM_FRAME_DELAY )
	{
		frameCounters[name]++;
		if( frameCounters[name] >= animations[name].size() )
			frameCounters[name] = 0;

		clock.restart();
	}

	if( !IsEmpty( name ) )
		return animations[name].at( frameCounters[name] );
	else
		return sf::IntRect( 0, 0, 0, 0 );
}

bool AnimManager::IsEmpty() const
{
	return animations.empty();
}

bool AnimManager::IsEmpty( const std::string &name )
{
	return animations[name].empty();
}
