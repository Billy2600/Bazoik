#pragma once
#include "game_state.h"
#include "gui_button.h"

class StateOptions : public GameState
{
private:
	sf::Text title;
	sf::Text rebind;
	bool awaitingRebind; // Are we waiting for a key to be pressed?
	bool error; // Are we displaying an error message?
	std::string bind; // Which bind are we setting?
	std::map<std::string, GuiButton> buttons;
	bool joystickInput = false; // Will flip once user starts using joystick
	int selectedButton; // Which button is currently selected? (Joystick only)

public:
	Game *game;

	StateOptions( Game *game );
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
	void Return(); // Return to previous state
	void SetBind( InputManager::Keys newKey );
};