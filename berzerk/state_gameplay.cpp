#include "state_gameplay.h"
#include "state_highscore.h"
#include "state_titlescreen.h"

Directions StateGameplay::lastMove = Directions::W;

bool StateGameplay::chicken = false;

StateGameplay::StateGameplay( Game *game )
{
	this->game = game;

	player.SetPos( maze.GetPlayerStart(lastMove, player) );
	transition = false;
	captured = false;

	// Init entities 
	entityManager.game = game;
	entityManager.Add( &player );
	AssetManager *assetManager = &this->game->assetManager;

	wallsCreated = false;
	enemiesSpawned = false;

	txScore.setFont( assetManager->GetFontRef( "joystix" ) );
	txScore.setFillColor( sf::Color::Green );
	txScore.setCharacterSize( 30 );
	txScore.setPosition( sf::Vector2f( 3, 3 ) );
	txScore.setString( game->score );

	for( unsigned int i = 0; i < MAX_LIVES; i++ )
	{
		lives[i].setTexture( game->assetManager.GetTextureRef( "sprites" ) );
		lives[i].setTextureRect( sf::IntRect( 89, 0, 5, 8 ) );
		lives[i].setScale( 4, 4 );
		lives[i].setPosition( (5 + lives[i].getGlobalBounds().width) * i, GAME_HEIGHT - lives[i].getGlobalBounds().height - 5 );
	}

	clock.restart();
}

void StateGameplay::HandleInput()
{
	sf::Event event;

	// Reset state if player is dead
	if( player.CheckDead() )
	{
		game->ResetState();
		return;
	}

	while( game->window.pollEvent( event ) )
	{
		// Close window
		if( event.type == sf::Event::Closed )
			game->window.close();

		if( event.type == sf::Event::KeyPressed )
		{
			if( event.key.code == sf::Keyboard::Escape )
			{
				game->PopState();
				break; // If you don't break here, it will crash
			}

			if( event.key.code == sf::Keyboard::Key::F12 )
			{
				this->game->AddLife();
			}
			if( event.key.code == sf::Keyboard::Key::F11 )
			{
				transition = true;
				lastMove = Directions::N;
			}
			if( event.key.code == sf::Keyboard::Key::F10 )
			{
				this->game->SwitchState( new StateGameplay( this->game ) );
				break;
			}
		}

		if( event.type == sf::Event::MouseButtonPressed )
		{
			/*if( event.mouseButton.button == sf::Mouse::Left )
			{
				input.fire = true;
			}*/
		}

		if( event.type == sf::Event::MouseButtonReleased )
		{
			/*if( event.mouseButton.button == sf::Mouse::Left )
			{
				input.fire = false;
			}*/
		}

		// Early out if we're in the middle of a transition
		if( transition )
		{
			return;
		}

		// Keys presed
		if( game->inputManager.TestKeyDown( "right", event ) ) input.right = true;

		if( game->inputManager.TestKeyDown( "left", event ) ) input.left = true;

		if( game->inputManager.TestKeyDown( "up", event ) ) input.up = true;

		if( game->inputManager.TestKeyDown( "down", event ) ) input.down = true;

		if( game->inputManager.TestKeyDown( "fire", event ) ) input.fire = true;

		// Keys released
		if( game->inputManager.TestKeyUp( "right", event ) ) input.right = false;

		if( game->inputManager.TestKeyUp( "left", event ) ) input.left = false;

		if( game->inputManager.TestKeyUp( "up", event ) ) input.up = false;
		
		if( game->inputManager.TestKeyUp( "down", event ) ) input.down = false;

		if( game->inputManager.TestKeyUp( "fire", event ) ) input.fire = false;
	}

	player.SetInput( input );
}

void StateGameplay::Update( const float dt )
{
	while( !maze.IsDone() )
	{
		maze.Generate();
	}
	if( maze.IsDone() && !wallsCreated )
	{
		maze.CreateWalls( entityManager );
		if( game->level >= 5 )
		{
			maze.BlockExit( entityManager, lastMove );
		}
		wallsCreated = true;
	}
	if( maze.IsDone() && !enemiesSpawned )
	{
		maze.SpawnEnemies( entityManager, lastMove );
		enemiesSpawned = true;
	}

	entityManager.Think( dt );

	if( !transition )
	{
		entityManager.CheckCollisions();
		txScore.setString( std::to_string( game->score ) );

		// Begin screen transition if player moves outside screen
		sf::Vector2f plPos( player.hitbox.left, player.hitbox.top );
		sf::Int32 now = clock.getElapsedTime().asMilliseconds();
		if( plPos.x > GAME_WIDTH )
		{
			transition = true;
			game->level++;
			lastMove = Directions::W;
			transStart = now;
		}
		else if( (plPos.x + player.hitbox.width) < 0 )
		{
			transition = true;
			game->level++;
			lastMove = Directions::E;
			transStart = now;
		}
		else if( plPos.y > GAME_HEIGHT)
		{
			transition = true;
			game->level++;
			lastMove = Directions::N;
			transStart = now;
		}
		else if( (plPos.y + player.hitbox.height) < 0 )
		{
			transition = true;
			game->level++;
			lastMove = Directions::S;
			transStart = now;
		}

		// Did we run out of lives?
		if( game->GetLives() == 0 )
		{
			this->game->SwitchState( new StateHighscore( this->game ) );
			return;
		}
	}
	else
	{
		ScreenTransition(dt);
	}
}

void StateGameplay::Draw() const
{
	if( transition && captured )
	{
		game->window.draw( sprTrans );
		return; // Don't draw anything else
	}

	entityManager.Draw();
	game->window.draw( txScore );
	for( unsigned int i = 0; i < MAX_LIVES; i++ )
	{
		if( i >= game->GetLives() )
			break;

		game->window.draw( lives[i] );
	}
}

void StateGameplay::ScreenTransition( const float dt )
{
	if( !captured )
	{
		// Capture the screen
		txTrans.create( GAME_WIDTH, GAME_HEIGHT );
		txTrans.update( game->window );
		sprTrans.setTexture( txTrans );
		captured = true;

		// Play sound
		if( entityManager.GetRobotCount() > 0 ) chicken = true;
		else chicken = false;

		if( chicken )
		{
			sfx.setBuffer( game->assetManager.GetSoundRef( "chicken", true ) );
			sfx.play();
		}
		else
		{
			sfx.setBuffer( game->assetManager.GetSoundRef( "intruder", true ) );
			sfx.play();
		}
	}
	else
	{
		sf::Vector2f move;
		switch( lastMove )
		{
		case Directions::N:
			move = sf::Vector2f( 0, -TRANS_SPEED );
			break;
		case Directions::E:
			move = sf::Vector2f( TRANS_SPEED, 0 );
			break;
		case Directions::S:
			move = sf::Vector2f( 0, TRANS_SPEED );
			break;
		case Directions::W:
			move = sf::Vector2f( -TRANS_SPEED, 0 );
			break;
		}

		sprTrans.move( move * dt );

		// Check for done
		sf::FloatRect sprBounds = sprTrans.getGlobalBounds();
		if( ( sprBounds.left + sprBounds.width ) < 0 || sprBounds.left > GAME_WIDTH ||
			( sprBounds.top + sprBounds.height ) < 0 || sprBounds.top > GAME_HEIGHT )
		{
			this->game->SwitchState( new StateGameplay( this->game ) );
		}
	}
}

StateGameplay::~StateGameplay()
{
	//maze.ClearMap();
	sfx.stop();
}