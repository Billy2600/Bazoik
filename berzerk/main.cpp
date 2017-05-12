#include <SFML/Graphics.hpp>
#include "game.h"
#include "state_splash.h"

int main()
{
	// Init game with splash screen
	Game game;
	game.states.push( new StateSplash( &game ) );
	game.GameLoop();

	return 0;
}