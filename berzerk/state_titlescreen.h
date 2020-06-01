#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "gui_button.h"

#define ATTRACT_MODE_DELAY 5000
#define NUM_DEMOS 3

class StateTitleScreen : public GameState
{
private:
	sf::Text debug;
	std::map<std::string, GuiButton> buttons;
	bool joystickInput = false; // Will flip once user starts using joystick
	int selectedButton; // Which button is currently selected? (Joystick only)
	bool playDemo;
	bool recordDemo;
	int currentDemo;
	int startLevel;
	sf::Clock clkAttractMode; // Timer for attract mode
	sf::Texture txImage;
	sf::Sprite spImage;

	void StartGame(); // Start the game

public:

	StateTitleScreen(Game *game);
	virtual void Start();
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
};