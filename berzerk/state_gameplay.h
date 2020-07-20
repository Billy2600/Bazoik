#pragma once
#include <vector>
#include <random>
#include "game_state.h"
#include "entity_player.h"
#include "entity_wall.h"
#include "entity_door.h"
#include "entity_manager.h"
#include "demo.h"
#include "pause_menu.h"
#include "room.h"

// Different transition speeds so we have time for robot voices (take less time to move screen vertically)
#define VERT_TRANS_SPEED 250
#define HORZ_TRANS_SPEED 400

class StateGameplay : public GameState
{
private:

	EntityPlayer player;
	PlayerInput input;
	EntityManager entityManager;
	AnimManager animManager;
	Room room;

	sf::Sprite background;
	sf::Sprite uiHitPoint[MAX_HP];
	sf::Sprite uiKey;
	sf::Text uiNumKeys;
	sf::RectangleShape uiMap[MAX_ROOM_X][MAX_ROOM_Y];
	sf::RectangleShape shTextRoom;
	sf::Text roomText;

	static Directions lastMove;
	bool transition; // Is the screen moving?
	sf::Clock clock;
	sf::Int32 transStart;
	sf::FloatRect transBoundry;

	Demo demo;
	bool recordDemo;
	bool playDemo;

	void LoadBackgroundAndUiElements();
	void ScreenTransition( const float dt ); // Transition screen
	bool ResetIfDead(); // Reset if player died
	void ReturnToTitle(); // Return to title screen
	sf::Vector2f GetPlayerStart(const Directions lastMove, EntityPlayer& player) const;

	PauseMenu pause;
	sf::Int32 pauseTime; // Record time we paused for use with the Otto delay

public:

	StateGameplay( Game *game, const bool recordDemo = false, const bool playDemo = false, const std::string demoName = "");
	virtual void Start();
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
	~StateGameplay();
};