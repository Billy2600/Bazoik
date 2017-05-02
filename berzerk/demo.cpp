#include "demo.h"

Demo::Demo()
{
	iterator = 0;
	frame = 0;
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
	if( iterator + 1 >= inputs.size() )
		return PlayerInput();

	if( frame == inputs.at( iterator + 1 ).frame )
		iterator++;

	frame++;
	return inputs.at( iterator ).input;
}

void Demo::LoadFromFile( const std::string path )
{
	inputs.clear();
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( path.c_str() );
	if( !result ) // Error check
	{
		return;
	}

	pugi::xml_node inputNodes = doc.child( "inputs" );
	for( pugi::xml_node input : inputNodes.children( "input" ) )
	{
		FrameInput newInput;
		newInput.input.up = std::stoi( input.attribute( "up" ).value() );
		newInput.input.down = std::stoi( input.attribute( "down" ).value() );
		newInput.input.left = std::stoi( input.attribute( "left" ).value() );
		newInput.input.right = std::stoi( input.attribute( "right" ).value() );
		newInput.input.fire = std::stoi( input.attribute( "fire" ).value() );
		newInput.frame = std::stoi( input.attribute( "frame" ).value() );
		inputs.push_back( newInput );
	}
	frame = 0;
}

void Demo::SaveToFile( const std::string path )
{
	// Build xml and save it to file
	pugi::xml_document doc;
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

	doc.save_file( path.c_str() );
}