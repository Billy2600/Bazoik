#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "pugixml.hpp"

#define JOYSTICK_DEADZONE 50

class Game;

class InputManager
{
public:
	Game *game; // Make sure to set this pointer before use; we need at least one function from it

	enum InputType
	{
		Keyboard,
		Mouse,
		Joystick,
		Axis
	};

	struct Keys
	{
		InputType inputType;
		sf::Keyboard::Key keyCode;
		sf::Mouse::Button mouseButton;
		unsigned int joystickButton;
		sf::Joystick::Axis axis;
		float axisPos;
	};

	std::map<std::string, Keys> keys;

	InputManager();
	int FindIndexFromName( const std::string name, const InputType type = InputType::Keyboard ) const; // Find keyName index from its name 
	void LoadDefaults(); // Load default keybinds
	void LoadFromFile( const std::string filename ); // Load keybinds from file
	void SaveToFile( const std::string filename ) const; // Save keybinds out to file
	void SetBind( const std::string name, const Keys key ); // Add/edit a keybind
	bool CheckExistingBind( const Keys key ) const; // Is this key already being used for a bind?
	bool TestKeyDown( const std::string name , const sf::Event e ) const; // Test a keybind event, pressed down
	bool TestKeyUp( const std::string name, const sf::Event e ) const; // Test a keybind event, released
	std::string GetKeyName( const std::string name ) const; // Get keyname for a bind
	~InputManager();

	// Names matching keycode numbers
	const std::string keyNames[98] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"NUM0", "NUM1", "NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7", "NUM8", "NUM9", "ESC", "L CTRL", "L SHIFT", "L ALT", "L SYS", "R CTRL", "R SHIFT", "R ALT", "L SYS",
		"MENU", "[", "]", ";", ",", ".", "'", "/", "\\", "~", "=", "-", "SPACE", "ENTER", "BACKSPACE", "TAB", "PAGE UP", "PAGE DOWN", "END", "HOME", "INS", "DEL",
		"+", "-", "*", "/", "LEFT", "RIGHT", "UP", "DOWN", "NUMPAD 0", "NUMPAD 1", "NUMPAD 2", "NUMPAD 3", "NUMPAD 4", "NUMPAD 5", "NUMPAD 6", "NUMPAD 7", "NUMPAD 8", "NUMPAD 9",
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "PAUSE"
	};

	const std::string axisNames[8] = { "X", "Y", "Z", "R", "U", "V", "PovX", "PovY" };
};

