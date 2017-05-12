#pragma once
#include <vector>
#include "game_state.h"
#include "entity_player.h"
#include "entity_wall.h"
#include "entity_robot.h"
#include "entity_manager.h"
#include "maze.h"
#include "demo.h"

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
	sf::Int32 transStart;
	sf::Sprite sprTrans; // Screenshot of screen for transition
	sf::Texture txTrans;

	const sf::Int32 ottoDelay = 10000;
	EntityOtto *otto;
	bool ottoSpawned; // Has Otto already been spawned?

	// Sound stuff
	sf::Sound sfx;
	bool deathSoundPlayed; // Did we play the death sound yet?

	void ScreenTransition( const float dt ); // Transition screen
	bool ResetIfDead(); // Reset if player died

	RobotStats LoadRobotStats(); // Load robot stats from xml file

	Demo demo;
	bool recordDemo;
	bool playDemo;

public:

	StateGameplay( Game *game, bool recordDemo = false, bool playDemo = false );
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
	~StateGameplay();
};