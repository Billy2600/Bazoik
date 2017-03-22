#pragma once
#include <map>
#include <string>
#include "game_state.h"
#include "gui_button.h"

class StateTitleScreen : public GameState
{
private:
	sf::Text title;
	std::map<std::string, GuiButton> buttons;
	bool joystickInput = false; // Will flip once user starts using joystick
	int selectedButton; // Which button is currently selected? (Joystick only)
	void StartGame(); // Start the game
	void LoadSounds(); // Load all sounds

public:

	StateTitleScreen(Game *game);
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
};