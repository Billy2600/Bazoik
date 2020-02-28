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
#define MAX_LIVES 9 // Maximum number of lives to show on screen
#define ALL_ROBOTS_SCORE 300 // Score added for killing all robots

class StateGameplay : public GameState
{
private:

	EntityPlayer player;
	PlayerInput input;
	EntityManager entityManager;
	static bool chicken; // Did you kill all the robots? Static so it will carry over
	sf::Text txScore;
	sf::Text respawnPrompt;
	sf::Sprite lives[MAX_LIVES];
	AnimManager animManager;
	Room room;

	sf::Sprite background;

	static Directions lastMove;
	bool transition; // Is the screen moving?
	sf::Clock clock;
	sf::Int32 transStart;
	sf::FloatRect transBoundry;

	Demo demo;
	bool recordDemo;
	bool playDemo;

	// Sound stuff
	bool deathSoundPlayed; // Did we play the death sound yet?

	void ScreenTransition( const float dt ); // Transition screen
	void PlayTransitionSound();
	bool ResetIfDead(); // Reset if player died
	void ReturnToTitle(); // Return to title screen
	sf::Vector2f GetPlayerStart(const Directions lastMove, EntityPlayer& player) const;

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