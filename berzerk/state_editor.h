#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "gui_button.h"

class StateEditor : public GameState
{
private:
	std::map<std::string, GuiButton> buttons;
	sf::Texture txBackground;
	sf::Sprite spBackground;
	
	bool showMenu;
	sf::RectangleShape menuBg;

	void InitMenu();
	void Load();
	void Save(); // Save out XML file

public:

	StateEditor(Game* game);
	virtual void Start();
	virtual void Draw() const;
	virtual void Update(const float dt);
	virtual void HandleInput();
};