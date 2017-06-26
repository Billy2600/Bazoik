#include <iterator>
#include "input_manager.h"
#include "game.h"

InputManager::InputManager()
{
}

int InputManager::FindIndexFromName( const std::string name, const InputType type ) const
{
	if( type == InputType::Keyboard )
	{
		for( unsigned int i = 0; i < keyNames.size(); i++ )
		{
			if( keyNames[i] == name )
			{
				return (signed int)i;
			}
		}
	}
	else if( type == InputType::Axis )
	{
		for( unsigned int i = 0; i < axisNames.size(); i++ )
		{
			if( axisNames[i] == name )
			{
				return (signed int)i;
			}
		}
	}
}

void InputManager::LoadDefaults()
{
	Keys key;
	key.inputType = InputType::Keyboard;
	key.keyCode = sf::Keyboard::Key::W;
	keys["up"] = key;
	key.keyCode = sf::Keyboard::Key::S;
	keys["down"] = key;
	key.keyCode = sf::Keyboard::Key::D;
	keys["right"] = key;
	key.keyCode = sf::Keyboard::Key::A;
	keys["left"] = key;
	key.keyCode = sf::Keyboard::Key::RControl;
	keys["fire"] = key;
	fullscreen = false;
}

void InputManager::LoadFromFile( const std::string filename )
{
	pugi::xml_document doc;
	std::string path = game->GetConfigDir() + filename;
	pugi::xml_parse_result result = doc.load_file( path.c_str() );
	if( !result ) // Error check
	{
		LoadDefaults();
		return;
	}

	try
	{
		pugi::xml_node keyNodes = doc.child( "keys" );

		for ( pugi::xml_node key : keyNodes.children( "key" ) )
		{
			std::string n( key.attribute( "name" ).value() );
			std::string type( key.attribute( "type" ).value() );
			std::string value( key.first_child().value() );

			if ( type == "keyboard" )
			{
				this->keys[n].inputType = InputType::Keyboard;
				this->keys[n].keyCode = static_cast<sf::Keyboard::Key>( FindIndexFromName( value ) );
			}
			else if ( type == "joystick" )
			{
				this->keys[n].inputType = InputType::Joystick;
				this->keys[n].joystickButton = std::stoi( value );
			}
			else if ( type == "mouse" )
			{
				this->keys[n].inputType = InputType::Mouse;
				this->keys[n].mouseButton = static_cast<sf::Mouse::Button>( FindIndexFromName( value ) );
			}
			else if ( type == "axis" )
			{
				this->keys[n].inputType = InputType::Axis;
				this->keys[n].axisPos = std::strtof( key.attribute( "axis" ).value(), NULL );
				this->keys[n].axis = static_cast<sf::Joystick::Axis>( FindIndexFromName( value, InputType::Axis ) );
			}
		}

		pugi::xml_node nodeFullscreen = doc.child( "fullscreen" );
		this->fullscreen = std::stoi( nodeFullscreen.first_child().value() );
	}
	catch( int e )
	{
		game->ThrowError( "Error while reading " + filename );
	}
}

void InputManager::SaveToFile( const std::string filename ) const
{
	try
	{
		// Build xml and save it to file
		pugi::xml_document doc;
		pugi::xml_node keyNodes = doc.append_child( "keys" );
		for ( auto key : this->keys )
		{
			pugi::xml_node keyNode = keyNodes.append_child( "key" );
			if ( key.second.inputType == InputType::Keyboard )
			{
				keyNode.append_attribute( "name" ).set_value( key.first.c_str() );
				keyNode.append_attribute( "type" ).set_value( "keyboard" );
				keyNode.append_child( pugi::node_pcdata ).set_value( keyNames[key.second.keyCode].c_str() );
			}
			if ( key.second.inputType == InputType::Joystick )
			{
				keyNode.append_attribute( "name" ).set_value( key.first.c_str() );
				keyNode.append_attribute( "type" ).set_value( "joystick" );
				keyNode.append_child( pugi::node_pcdata ).set_value( std::to_string( key.second.joystickButton ).c_str() );
			}
			if ( key.second.inputType == InputType::Axis )
			{
				keyNode.append_attribute( "name" ).set_value( key.first.c_str() );
				keyNode.append_attribute( "type" ).set_value( "axis" );
				keyNode.append_attribute( "axis" ).set_value( std::to_string( key.second.axisPos ).c_str() );
				keyNode.append_child( pugi::node_pcdata ).set_value( axisNames[key.second.axis].c_str() );
			}
		}

		std::string fullscreenValue = "0";
		if ( this->fullscreen ) fullscreenValue = "1";
		pugi::xml_node nodeFullscreen = doc.append_child( "fullscreen" );
		nodeFullscreen.append_child( pugi::node_pcdata ).set_value( fullscreenValue.c_str() );

		std::string path = game->GetConfigDir() + filename;
		doc.save_file( path.c_str() );
	}
	catch ( int e )
	{
		game->ThrowError( "Error while reading " + filename );
	}
}

void InputManager::SetBind( const std::string name, const Keys key )
{
	keys[name] = key;
}

bool InputManager::CheckExistingBind( const Keys key ) const
{
	for( auto existKey : keys )
	{
		if( existKey.second.inputType == key.inputType ) // This is where we start getting suspcious
		{
			if( key.inputType == InputType::Keyboard && key.keyCode == existKey.second.keyCode )
				return true;
			if( key.inputType == InputType::Joystick && key.joystickButton == existKey.second.joystickButton )
				return true;
			if( key.inputType == InputType::Axis && key.axis == existKey.second.axis && key.axisPos == existKey.second.axisPos )
				return true;
			if( key.inputType == InputType::Mouse && key.mouseButton == existKey.second.mouseButton )
				return true;
		}
	}

	return false;
}

bool InputManager::TestKeyDown(const std::string name, const sf::Event e ) const
{
	if( keys.find( name ) == keys.end() )
	{
		// Not found
		return false;
	}

	Keys k = keys.at(name);

	if( k.inputType == Mouse &&
		e.type == sf::Event::MouseButtonPressed &&
		k.mouseButton == e.mouseButton.button )
	{
		return true;
	}

	if( k.inputType == Keyboard &&
		e.type == sf::Event::KeyPressed &&
		k.keyCode == e.key.code )
	{
		return true;
	}

	if( k.inputType == Joystick &&
		e.type == sf::Event::JoystickButtonPressed &&
		k.joystickButton == e.joystickButton.button )
	{
		return true;
	}

	if( k.inputType == Axis &&
		e.type == sf::Event::JoystickMoved &&
		k.axis == e.joystickMove.axis )
	{

		if( k.axisPos < 0 && e.joystickMove.position < 0 )
			return true;
		else if( k.axisPos > 0 && e.joystickMove.position > 0 )
			return true;
	}

	return false;
}

bool InputManager::TestKeyUp( const std::string name, const sf::Event e ) const
{
	if( keys.find( name ) == keys.end() )
	{
		// Not found
		return false;
	}

	Keys k = keys.at( name );

	if( k.inputType == Mouse &&
		e.type == sf::Event::MouseButtonReleased &&
		k.mouseButton == e.mouseButton.button )
	{
		return true;
	}

	if( k.inputType == Keyboard &&
		e.type == sf::Event::KeyReleased &&
		k.keyCode == e.key.code )
	{
		return true;
	}

	if( k.inputType == Joystick &&
		e.type == sf::Event::JoystickButtonReleased &&
		k.joystickButton == e.joystickButton.button )
	{
		return true;
	}

	if( k.inputType == Axis &&
		e.type == sf::Event::JoystickMoved &&
		k.axis == e.joystickMove.axis )
	{
		if( k.axisPos < 0 && e.joystickMove.position > -JOYSTICK_DEADZONE )
			return true;
		if( k.axisPos > 0 && e.joystickMove.position < JOYSTICK_DEADZONE )
			return true; 
	}

	return false;
}

std::string InputManager::GetKeyName( const std::string name ) const
{
	if( keys.find( name ) != keys.end() )
	{
		if( keys.at( name ).inputType == InputType::Keyboard )
		{
			if( keys.at( name ).keyCode > (signed int)keyNames.size() || keys.at( name ).keyCode < 0 ) // Bounds checking
				return "UNBOUND";

			return keyNames[keys.at( name ).keyCode];
		}
		if( keys.at( name ).inputType == InputType::Joystick )
		{
			return "Joystick Button " + std::to_string( keys.at( name ).joystickButton );
		}
		if( keys.at( name ).inputType == InputType::Axis )
		{
			if( keys.at( name ).axis > (signed int)axisNames.size() || keys.at( name ).axis < 0 ) // Bounds checking
				return "UNBOUND";

			std::string pos = "+"; // Indicate left/right, up/down, etc.
			if( keys.at( name ).axisPos < 0 ) pos = "-";
			return "Joystick Axis " + axisNames[keys.at(name).axis] + pos;
		}

		return "UNBOUND";
	}
	else
	{
		return "UNBOUND";
	}
}

InputManager::~InputManager()
{

}
