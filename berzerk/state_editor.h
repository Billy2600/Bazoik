#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "gui_button.h"
#include "entity_door.h"
#include "room.h"

#define SHOW_MESSAGE_TIME 3 // How long to show messages (e.g. saved) on screen (in seconds)

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

	struct EditorEntities
	{
		std::string type;
		sf::Sprite sprite; // Sprite can be used for positioning, bounds, etc.
	};

	struct EditorRooms
	{
		std::vector<EditorEntities> entities;
		std::map<Directions, DoorStates> doorStates;
	};

	std::map<std::string, GuiButton> buttons;
	std::map<std::string, sf::Text> text;
	AnimManager animManager;
	sf::Sprite spBackground;
	sf::Clock lastSave;

	EditorRooms rooms[MAX_ROOM_X][MAX_ROOM_Y];
	sf::Vector2i currentRoom;
	std::map<Directions, EditorDoor> doors; // For working on doors in current room; the actual intractable on-screen element
	std::map<std::string, sf::Sprite> entities; // On-screen entities, for drawing/interacting
	
	bool showMenu;
	sf::RectangleShape menuBg;

	void InitMenu();
	void InitDoors();
	void UpdateDoors();
	void AddEntity(const std::string type, const sf::Vector2f pos);
	void Load(); // Load from XML file
	void Save(); // Save out XML file
	void ChangeRoom(const sf::Vector2i newRoom);

public:

	StateEditor(Game* game);
	virtual void Start();
	virtual void Draw() const;
	virtual void Update(const float dt);
	virtual void HandleInput();
};