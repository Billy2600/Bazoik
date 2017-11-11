#include "demo.h"
#include "error_log.h"

Demo::Demo(const int level)
{
	iterator = 0;
	frame = 0;
	this->level = level;
	done = false;
}

void Demo::Record( const PlayerInput input )
{
	// Early out if it's the same as the last one
	if( inputs.size() > 0 && input == inputs.back().input )
	{
		frame++;
		return;
	}

	// Add it
	FrameInput newInput { input, frame++ };
	inputs.push_back( newInput );
}

PlayerInput Demo::Play()
{
	if ( iterator + 1 >= inputs.size() )
	{
		done = true;
		return PlayerInput(); // No movement
	}

	if( frame == inputs.at( iterator + 1 ).frame )
		iterator++;

	frame++;
	return inputs.at( iterator ).input;
}

void Demo::LoadFromFile( const std::string path )
{
	ErrorLog log;
	log.Write( "Loading demo " + path + " from file" );

	inputs.clear();
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( path.c_str() );
	if ( !result ) // Error check
		return;

	pugi::xml_node levelNode = doc.child( "level" );
	if ( levelNode == NULL )
		return;

	level = std::stoi( levelNode.attribute("num").value() );

	pugi::xml_node inputNodes = doc.child( "inputs" );
	if ( inputNodes == NULL )
		return;

	for( pugi::xml_node input : inputNodes.children( "input" ) )
	{
		if ( input == NULL )
			return;

		FrameInput newInput;
		newInput.input.up = std::stoi( input.attribute( "up" ).value() );
		newInput.input.down = std::stoi( input.attribute( "down" ).value() );
		newInput.input.left = std::stoi( input.attribute( "left" ).value() );
		newInput.input.right = std::stoi( input.attribute( "right" ).value() );
		newInput.input.fire = std::stoi( input.attribute( "fire" ).value() );
		newInput.frame = std::stoi( input.attribute( "frame" ).value() );
		inputs.push_back( newInput );
	}

	pugi::xml_node wallNodes = doc.child( "walls" );
	if ( wallNodes == NULL )
		return;

	for ( pugi::xml_node wall : wallNodes.children( "wall" ) )
	{
		if ( wall == NULL )
			return;

		walls.push_back( sf::IntRect(
			std::stoi( wall.attribute( "x" ).value() ),
			std::stoi( wall.attribute( "y" ).value() ),
			std::stoi( wall.attribute( "w" ).value() ),
			std::stoi( wall.attribute( "h" ).value() ) ) );
	}

	pugi::xml_node robotNodes = doc.child( "robots" );
	if ( robotNodes == NULL )
		return;

	for ( pugi::xml_node robot : robotNodes.children( "robot" ) )
	{
		if ( robot == NULL )
			return;

		robotPositions.push_back( sf::Vector2f(
			std::stof( robot.attribute( "x" ).value() ),
			std::stof( robot.attribute( "y" ).value() ) ) );
	}

	frame = 0;
}

void Demo::SaveToFile( const std::string path ) const
{
	ErrorLog log;
	log.Write( "Saving demo " + path + " from file" );

	// Build xml and save it to file
	pugi::xml_document doc;

	pugi::xml_node levelNode = doc.append_child( "level" );
	levelNode.append_attribute( "num" ).set_value( level );

	pugi::xml_node inputNodes = doc.append_child( "inputs" );
	for( auto fInput : inputs )
	{
		pugi::xml_node inputNode = inputNodes.append_child( "input" );
		inputNode.append_attribute( "down" ).set_value( (int)fInput.input.down );
		inputNode.append_attribute( "left" ).set_value( (int)fInput.input.left );
		inputNode.append_attribute( "right" ).set_value( (int)fInput.input.right );
		inputNode.append_attribute( "up" ).set_value( (int)fInput.input.up );
		inputNode.append_attribute( "fire" ).set_value( (int)fInput.input.fire );
		inputNode.append_attribute( "frame" ).set_value( (int)fInput.frame );
	}

	pugi::xml_node wallNodes = doc.append_child( "walls" );
	for ( auto wall : walls )
	{
		pugi::xml_node wallNode = wallNodes.append_child( "wall" );
		wallNode.append_attribute( "x" ).set_value( wall.left );
		wallNode.append_attribute( "y" ).set_value( wall.top );
		wallNode.append_attribute( "w" ).set_value( wall.width );
		wallNode.append_attribute( "h" ).set_value( wall.height );
	}

	pugi::xml_node robotNodes = doc.append_child( "robots" );
	for ( auto robot : robotPositions )
	{
		pugi::xml_node robotNode = robotNodes.append_child( "robot" );
		robotNode.append_attribute( "x" ).set_value( robot.x );
		robotNode.append_attribute( "y" ).set_value( robot.y );
	}

	doc.save_file( path.c_str() );
}

void Demo::SetWalls( const std::vector<sf::IntRect> walls )
{
	this->walls = walls;
}

std::vector<sf::IntRect> Demo::GetWalls() const
{
	return walls;
}

void Demo::SetRobotPositions( const std::vector<sf::Vector2f> positions )
{
	robotPositions = positions;
}

std::vector<sf::Vector2f> Demo::GetRobotPositions() const
{
	return robotPositions;
}

void Demo::SetLevel( const int level )
{
	this->level = level;
}

int Demo::GetLevel() const
{
	return level;
}

bool Demo::IsDone() const
{
	return done;
}