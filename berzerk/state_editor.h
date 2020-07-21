#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "gui_button.h"
#include "entity_door.h"
#include "room.h"

#define SHOW_MESSAGE_TIME 3 // How long to show messages (e.g. saved) on screen (in seconds)
#define ENTITY_SPAWN_POS sf::Vector2f(0,0)

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

		bool operator ==(const EditorEntities& rhs) // Need to overload == operator for vector deletes
		{
			return (
				this->sprite.getTextureRect() == rhs.sprite.getTextureRect()
				&& this->sprite.getTexture() == rhs.sprite.getTexture()
				&& this->sprite.getGlobalBounds() == rhs.sprite.getGlobalBounds()
				&& this->type == rhs.type
			);
		}
	};

	struct EditorRooms
	{
		std::vector<EditorEntities> entities;
		std::map<Directions, DoorStates> doorStates;
	};

	std::map<std::string, GuiButton> buttons;
	std::map<std::string, sf::Text> text;
	std::map<std::string, sf::Sprite> menuEntities; // Buttons for entities we can add; string identifier is also the type
	AnimManager animManager;
	sf::Sprite spBackground;
	sf::Clock lastSave;
	sf::Sprite* currentlyDragging; // Which object are we currently dragging? NULL when none
	sf::Vector2f draggingOffset; // Offset used while dragging object

	EditorRooms rooms[MAX_ROOM_X][MAX_ROOM_Y];
	sf::Vector2i currentRoom;
	std::map<Directions, EditorDoor> doors; // For working on doors in current room; the actual intractable on-screen element
	
	bool showMenu;
	sf::RectangleShape menuBg;
	bool deleteMode;
	sf::Sprite deleteIcon; // Show delete icon where mouse is

	void InitMenu();
	void InitDoors();
	void UpdateDoors();
	void AddEntity(const std::string type, const sf::Vector2f pos, sf::Vector2i room = sf::Vector2i(-999, -999));
	void Load(); // Load from XML file
	void Save(); // Save out XML file
	void ChangeRoom(const sf::Vector2i newRoom);
	void RemoveEntity(const EditorEntities entity); // Purposefully taking in by value

public:

	StateEditor(Game* game);
	virtual void Start();
	virtual void Draw() const;
	virtual void Update(const float dt);
	virtual void HandleInput();
};