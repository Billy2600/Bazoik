#pragma once
#include <map>
#include <string>
#include "game_state.h"
#include "gui_button.h"

#define ATTRACT_MODE_DELAY 5000
#define NUM_DEMOS 3

class StateTitleScreen : public GameState
{
private:
	sf::Text title;
	std::map<std::string, GuiButton> buttons;
	bool joystickInput = false; // Will flip once user starts using joystick
	int selectedButton; // Which button is currently selected? (Joystick only)
	void StartGame(); // Start the game
	bool playDemo;
	bool recordDemo;
	int currentDemo;
	int startLevel;
	sf::Clock clkAttractMode; // Timer for attract mode

public:

	StateTitleScreen(Game *game);
	virtual void Start();
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
};