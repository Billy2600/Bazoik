#pragma once
#pragma once
#include "game_state.h"

class StateCredits : public GameState
{
private:
	sf::Text title;
	sf::Text txCredits;
	sf::Clock clock;

public:

	StateCredits( Game *game );
	virtual void Start();
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
};
