#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "gui_button.h"
#include "entity_door.h"

class StateEditor : public GameState
{
private:
	struct EditorDoor
	{
		sf::Sprite sprite;
		sf::RectangleShape shape;
		DoorStates state;
		sf::FloatRect hitbox;
	};

	std::map<std::string, GuiButton> buttons;
	AnimManager animManager;
	sf::Sprite spBackground;
	std::map<std::string, sf::Vector2f> objects;
	std::map<char, EditorDoor> doors; // n, s, e, w doors
	
	bool showMenu;
	sf::RectangleShape menuBg;

	void InitMenu();
	void InitDoors();
	void Load(); // Load from XML file
	void Save(); // Save out XML file

public:

	StateEditor(Game* game);
	virtual void Start();
	virtual void Draw() const;
	virtual void Update(const float dt);
	virtual void HandleInput();
};