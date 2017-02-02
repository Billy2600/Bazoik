#include <SFML/Graphics.hpp>
#include "game.h"
#include "state_titlescreen.h"

int main()
{
	// Init game with title screen
	Game game;
	game.states.push( new StateTitleScreen( &game ) );
	game.GameLoop();

	return 0;
}