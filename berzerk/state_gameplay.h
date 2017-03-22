#pragma once
#include <vector>
#include "game_state.h"
#include "entity_player.h"
#include "entity_wall.h"
#include "entity_robot.h"
#include "entity_manager.h"
#include "maze.h"

#define TRANS_SPEED 500 // Speed of screen transition
#define MAX_LIVES 9 // Maximum number of lives to show on screen

class StateGameplay : public GameState
{
private:

	EntityPlayer player;
	PlayerInput input;
	EntityManager entityManager;
	Maze maze;
	bool wallsCreated;
	bool enemiesSpawned;
	static bool chicken; // Did you kill all the robots? Static so it will carry over
	sf::Text txScore;
	sf::Sprite lives[MAX_LIVES];

	static Directions lastMove;
	bool transition; // Is the screen moving?
	bool captured; // Did we capture an image of the screen yet?
	sf::Clock clock;
	//const sf::Int32 transDelay = 1500;
	sf::Int32 transStart;
	sf::Sprite sprTrans; // Screenshot of screen for transition
	sf::Texture txTrans;

	// Sound stuff
	sf::Sound sfx;

	void ScreenTransition( const float dt ); // Transition screen

public:

	StateGameplay( Game *game );
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
	~StateGameplay();
};