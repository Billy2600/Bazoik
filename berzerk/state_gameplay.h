#pragma once
#include <vector>
#include "game_state.h"
#include "entity_player.h"
#include "entity_wall.h"
#include "entity_robot.h"
#include "entity_manager.h"
#include "maze.h"
#include "demo.h"
#include "pause_menu.h"

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

	EntityOtto *otto;
	bool ottoSpawned; // Has Otto already been spawned?

	Demo demo;
	bool recordDemo;
	bool playDemo;

	// Sound stuff
	sf::Sound sfx;
	bool deathSoundPlayed; // Did we play the death sound yet?

	void ScreenTransition( const float dt ); // Transition screen
	bool ResetIfDead(); // Reset if player died
	void ReturnToTitle(); // Return to title screen

	RobotStats LoadRobotStats(); // Load robot stats from xml file

	static std::vector<Directions> lastFourMoves; // Used to keep track for easter egg
	void AddLastMove( Directions move ); // Add last move to vector
	bool CheckEasterEgg() const; // Check if we should fire easter egg

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