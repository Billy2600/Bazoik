#pragma once
#include <string>
#include "game_state.h"

#define NUM_SCORES 10 // How many scores will the table hold?
#define HS_FIRST_CHAR 33 // First valid char
#define HS_LAST_CHAR 90 // Last valid char

class StateHighscore : public GameState
{
private:
	struct Score
	{
		unsigned int num;
		char initials[5];
	};

	sf::Text title;
	sf::Text table; // High score table
	sf::Text countdown; // Show visible countdown on screen after entering highscore
	Score scores[NUM_SCORES];

	bool entryMode; // Are we entering a score?
	sf::Int8 entryIndex; // Which of the initials are we entering?
	unsigned int entryLoc; // Location of new high score in the scores array
	unsigned char entryChar; // Current char
	sf::Clock clock;
	const sf::Int32 blinkDelay = 500; // Input cursor blink delay (in milliseconds)
	const sf::Int32 resetDelay = 5000; // Delay before moving back to title screen

	void CheckNewScore(); // Did we get a new score?
	// Quicksort scores
	void QuickSort( const int left, const int right );
	int Partition( const int left, const int right );
	
	void SaveToFile( const std::string filename ) const;
	void LoadFromFile( const std::string filename );

public:
	StateHighscore(Game *game);
	virtual void Start();
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
	void Close(); // Make sure to call this before closing this state
	~StateHighscore();
};

