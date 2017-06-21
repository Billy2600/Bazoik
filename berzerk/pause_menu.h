#pragma once
#include "game.h"
#include "gui_button.h"

class PauseMenu
{
private:
	Game* game;
	sf::Text title;
	std::map<std::string, GuiButton> buttons;
	bool joystickInput = false; // Will flip once user starts using joystick
	int selectedButton; // Which button is currently selected? (Joystick only)
	sf::RectangleShape overlay; // Overlay to darken screen

public:
	bool open; // If menu is open or not
	bool quit; // Was quit selected?

	PauseMenu();
	void SetGame( Game *game );
	void Draw() const;
	void HandleInput();
	~PauseMenu();
};

