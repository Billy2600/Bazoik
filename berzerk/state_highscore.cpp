#include <sstream>
#include <array>
#include "state_highscore.h"
#include "pugixml.hpp"
#ifndef _WIN32
#include <cstring>
#define strcpy_s strcpy
#endif

StateHighscore::StateHighscore( Game *game )
{
	this->game = game;
	AssetManager *assetManager = &this->game->assetManager;
	entryMode = false;
	entryIndex = 0;
	entryChar = 'A';

	title.setFont( assetManager->GetFontRef( "joystix" ) );
#ifdef OLD_SFML
	title.setColor( sf::Color::Green );
#else
	title.setFillColor( sf::Color::Green );
#endif
	title.setCharacterSize( 90 );
	title.setPosition( sf::Vector2f( 3, 3 ) );
	title.setString( "HIGH SCORE" );

	table = title;
	table.setCharacterSize( 30 );
	table.setPosition( sf::Vector2f( 3, 90 ) );
	table.setString( "" );

	LoadFromFile( "scores.xml" );
	QuickSort(0, NUM_SCORES-1);
	CheckNewScore();

	clock.restart();
}

void StateHighscore::Start()
{

}

void StateHighscore::CheckNewScore()
{
	if( game->score > scores[NUM_SCORES-1].num )
	{
		scores[NUM_SCORES - 1] = { game->score, { ' ', ' ', ' ', '\0', ' ' } };
		entryMode = true;
	}

	QuickSort( 0, NUM_SCORES - 1 );
}

// Obviously I didn't come up with quicksort
void StateHighscore::QuickSort( const int low, const int hi )
{
	if( low < hi )
	{
		int p = Partition( low, hi );
		QuickSort( low, p - 1 );
		QuickSort( p + 1, hi );
	}
}

int StateHighscore::Partition( const int low, const int hi )
{
	int pivot = (signed int)scores[hi].num;
	int i = low;

	for( int j = low; j < hi; j++ )
	{
		if( scores[j].num >= (unsigned int)pivot ) // Sort descending
		{
			std::swap( scores[i], scores[j] );
			i++;
		}
	}

	std::swap( scores[i], scores[hi] );
	return i;
}

void StateHighscore::SaveToFile( const std::string filename ) const
{
	// Build xml and save it to file
	pugi::xml_document doc;
	pugi::xml_node scoreNodes = doc.append_child( "scores" );

	for( auto score : scores )
	{
		pugi::xml_node scoreNode = scoreNodes.append_child( "score" );
		scoreNode.append_attribute( "name" ).set_value( score.initials );
		scoreNode.append_child( pugi::node_pcdata ).set_value( std::to_string(score.num).c_str() );
	}

	std::string path = game->GetConfigDir() + filename;
	doc.save_file( path.c_str() );
}

void StateHighscore::LoadFromFile( const std::string filename )
{
	pugi::xml_document doc;
	std::string path = game->GetConfigDir() + filename;
	pugi::xml_parse_result result = doc.load_file( path.c_str() );
	if( !result ) // Error check
	{
		return;
	}

	pugi::xml_node scoreNodes = doc.child( "scores" );

	int i = 0;
	for( pugi::xml_node score : scoreNodes.children( "score" ) )
	{
		std::string name( score.first_child().value() );
		scores[i].num = std::stoi( name );
		strcpy_s( scores[i].initials, score.attribute( "name" ).value() );
		i++;
	}
}

void StateHighscore::Draw() const
{
	game->window.draw( title );
	game->window.draw( table );
}

void StateHighscore::Update( const float dt )
{
	std::stringstream ss;
	unsigned int i = 1; // Not being used for array offset, so we can set it to 1
	for( auto score : scores )
	{
		// Keep spacing consistent with double digit numbers
		std::string space;
		if( i > 9 ) space = "";
		else space = " ";

		// Display entry mode
		if( entryMode && score.initials[4] == ' ' )
		{
			entryLoc = i - 1; // Take down location

			if( entryChar < 0 ) entryChar = 0;
			if( entryChar > 255 ) entryChar = 255;

			// Blink cursor
			if( entryIndex < 3 )
			{
				if( clock.getElapsedTime().asMilliseconds() > blinkDelay )
				{
					if( scores[entryLoc].initials[entryIndex + 1] == ' ' )
						scores[entryLoc].initials[entryIndex + 1] = '_';
					else
						scores[entryLoc].initials[entryIndex + 1] = ' ';

					clock.restart();
				}
			}

			scores[entryLoc].initials[entryIndex] = entryChar;

			// Finish up
			if( entryIndex > 2 )
			{
				scores[entryLoc].initials[3] = '\0';
				entryMode = false;
				clock.restart();
			}
		}
			
		ss << i << ". " << space << score.num << " " << score.initials  << "\n";
		i++;
	}

	table.setString( ss.str() );

	if( clock.getElapsedTime().asMilliseconds() > resetDelay )
	{
		Close();
		game->PopState();
		return;
	}
}

void StateHighscore::HandleInput()
{
	sf::Event event;

	while( game->window.pollEvent( event ) )
	{
		// Close window
		if( event.type == sf::Event::Closed )
		{
			Close();
			game->window.close();
		}

		if( event.type == sf::Event::KeyPressed )
		{
			if( event.key.code == sf::Keyboard::Escape )
			{
				Close();
				game->PopState();
				break; // If you don't break here, it will crash
			}
		}

		// Initials input
		if( entryMode )
		{
			if( game->inputManager.TestKeyDown( "right", event ) )
			{
				entryChar++;
			}

			if( game->inputManager.TestKeyDown( "left", event ) )
			{
				entryChar--;
			}

			if( game->inputManager.TestKeyDown( "fire", event ) )
			{
				entryIndex++;
			}
		}
	}
}

void StateHighscore::Close()
{
	SaveToFile( "scores.xml" );
	game->score = 0;
}

StateHighscore::~StateHighscore()
{
	Close();
}
