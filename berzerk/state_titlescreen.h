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
	void StartGame(); // Start the game

public:

	StateTitleScreen(Game *game);
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
};