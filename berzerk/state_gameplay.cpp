#include "state_gameplay.h"
#include "state_highscore.h"
#include "state_titlescreen.h"
#include "pugixml.hpp"

Directions StateGameplay::lastMove = Directions::N;
std::vector<Directions> StateGameplay::lastFourMoves = std::vector<Directions>();

bool StateGameplay::chicken = false;

StateGameplay::StateGameplay( Game *game, const bool recordDemo , const bool playDemo, const std::string demoName )
{
	this->game = game;

	player.SetPos( GetPlayerStart(lastMove, player) );
	transition = false;

	// Init entities 
	entityManager.game = game;
	entityManager.Add( &player );
	AssetManager *assetManager = &this->game->assetManager;
	room = Room(game->currentRoom, &entityManager);
	room.SetupRoom();

	txScore.setFont( assetManager->GetFontRef( "joystix" ) );
#ifdef OLD_SFML
	txScore.setColor( sf::Color::Green );
#else
	txScore.setFillColor( sf::Color::Green );
#endif
	txScore.setCharacterSize( 30 );
	txScore.setPosition( sf::Vector2f( 3, 3 ) );
	txScore.setString( game->score );

	respawnPrompt = txScore;
	respawnPrompt.setPosition( sf::Vector2f( 3, 50 ) );
	respawnPrompt.setString( "Press any button to respawn" );

	for( unsigned int i = 0; i < MAX_LIVES; i++ )
	{
		lives[i].setTexture( game->assetManager.GetTextureRef( "sprites" ) );
		lives[i].setTextureRect( animManager.Animate( "life_icon" ) );
		lives[i].setScale( 4, 4 );
		lives[i].setPosition( (5 + lives[i].getGlobalBounds().width) * i, GAME_HEIGHT - lives[i].getGlobalBounds().height - 5 );
	}

	background = sf::Sprite( assetManager->GetTextureRef( "background" ) );

	deathSoundPlayed = false;

	clock.restart();

	this->recordDemo = recordDemo;
	this->playDemo = playDemo;
	if ( playDemo )
	{
		demo.LoadFromFile( demoName );
		//game->level = demo.GetLevel();
	}
	if ( recordDemo )
	{
		//demo.SetLevel( game->level );
	}

	pause.SetGame( game );
	pauseTime = 0;

	if ( game->music.getStatus() != sf::Music::Status::Playing )
	{
		if (!game->music.openFromFile("assets/music.wav"))
		{
			ErrorLog log;
			log.Write("Could not open music file music.wav");
		}
		else
		{
			game->music.play();
		}
	}
}

void StateGameplay::Start()
{
	
}

void StateGameplay::HandleInput()
{
	// Show or hide cursor based on whether the pause menu is open
	game->window.setMouseCursorVisible( pause.open );

	if ( pause.open )
	{
		pause.HandleInput();
		if ( pause.quit )
		{
			game->music.stop();
			ReturnToTitle();
			return;
		}

		// Prevent controls from getting stuck when pause menu is opened
		input.right = false;
		input.left = false;
		input.up = false;
		input.down = false;
		input.fire = false;
		return;
	}

	sf::Event event;

	while( game->window.pollEvent( event ) )
	{
		// Close window
		if ( event.type == sf::Event::Closed )
		{
			game->Close();
			return;
		}

		if ( ResetIfDead() )
			return; // Do not continue if we're resetting the state

		if( event.type == sf::Event::KeyPressed )
		{
			if ( event.key.code == sf::Keyboard::Escape || game->inputManager.TestKeyDown( "pause", event ) )
			{
				if ( recordDemo || playDemo )
				{
					ReturnToTitle();
				}
				else
				{
					pauseTime = clock.getElapsedTime().asMilliseconds();
					pause.open = true; // Pause menu will close itself
				}
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
			if( event.key.code == sf::Keyboard::Key::F9 )
			{
				demo.SaveToFile( game->GetConfigDir() + "demo1.xml" );
			}

			if ( event.key.code == sf::Keyboard::Key::Equal )
			{
				game->music.setVolume( game->music.getVolume() + 10 );
			}

			if ( event.key.code == sf::Keyboard::Key::Dash )
			{
				game->music.setVolume( game->music.getVolume() - 10 );
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

	if( playDemo && !recordDemo )
	{
		player.SetInput( demo.Play() );
		// Return to title screen if demo is done
		if ( demo.IsDone() )
		{
			ReturnToTitle();
			return;
		}
	}
	else if( !playDemo )
	{
		player.SetInput( input );
		if( recordDemo ) // This is here so you can't record while playing a demo
			demo.Record( input );
	}
}

void StateGameplay::Update( const float dt )
{
	if ( pause.open )
	{
		clock.restart();
		return;
	}

	if( !transition )
	{
		entityManager.Think( dt );
		entityManager.CheckCollisions();
		//txScore.setString( std::to_string( game->score ) );
		txScore.setString( std::to_string(game->currentRoom.x) + "," + std::to_string(game->currentRoom.y) );

		// Begin screen transition if player moves outside screen
		sf::Vector2f plPos( player.hitbox.left, player.hitbox.top );
		sf::Int32 now = clock.getElapsedTime().asMilliseconds();
		bool startTrans = false;

		// Decide if we should start transition
		if( plPos.x > GAME_WIDTH )
		{
			lastMove = Directions::W;
			startTrans = true;
		}
		else if( ( plPos.x + player.hitbox.width ) < 0 )
		{
			lastMove = Directions::E;
			startTrans = true;
		}
		else if( plPos.y > GAME_HEIGHT )
		{
			lastMove = Directions::N;
			startTrans = true;
		}
		else if( ( plPos.y + player.hitbox.height ) < 0 )
		{
			lastMove = Directions::S;
			startTrans = true;
		}

		// Kick off transition
		if ( startTrans )
		{
			transition = true;
			AddLastMove( lastMove );
			transStart = now;
			PlayTransitionSound();
		}

		// Did we run out of lives?
		if( game->GetLives() <= 0 && player.CheckReset() )
		{
			game->music.stop();
			game->assetManager.StopSound( "death" );
			lastMove = Directions::W;
			game->window.setMouseCursorVisible( true );
			this->game->SwitchState( new StateHighscore( this->game ) );
			return;
		}

		if( player.IsDead() && !deathSoundPlayed )
		{
			if( chicken )
			{
				game->assetManager.PlaySound( "got_chicken", true );
			}
			else
			{
				game->assetManager.PlaySound( "got_humanoid", true );
			}

			deathSoundPlayed = true;
		}
	}
	else
	{
		ScreenTransition(dt);
	}
}

void StateGameplay::Draw() const
{
	game->window.draw(background);

	entityManager.Draw();

	game->window.draw( txScore );

	if ( player.IsDead() && !playDemo && !recordDemo )
	{
		game->window.draw( respawnPrompt );
	}

	for( unsigned int i = 0; i < MAX_LIVES; i++ )
	{
		if( i >= game->GetLives() )
			break;

		game->window.draw( lives[i] );
	}

	if ( pause.open )
		pause.Draw();
}

void StateGameplay::ScreenTransition( const float dt )
{
	sf::Vector2f move;
	sf::Vector2i nextRoom = game->currentRoom;

	switch( lastMove )
	{
	case Directions::N:
		move = sf::Vector2f( 0, -VERT_TRANS_SPEED );
		nextRoom.y++;
		break;
	case Directions::E:
		move = sf::Vector2f( HORZ_TRANS_SPEED, 0 );
		nextRoom.x--;
		break;
	case Directions::S:
		move = sf::Vector2f( 0, VERT_TRANS_SPEED );
		nextRoom.y--;
		break;
	case Directions::W:
		move = sf::Vector2f( -HORZ_TRANS_SPEED, 0 );
		nextRoom.x++;
		break;
	}

	transBoundry.width = GAME_WIDTH;
	transBoundry.height = GAME_HEIGHT;
	transBoundry.left += move.x * dt;
	transBoundry.top += move.y * dt;

	player.SetPos( sf::Vector2f( -1000, -1000 ) ); // Move player off of screen

	entityManager.MoveAllEntities( move, dt );

	// Check for done
	if ( ( transBoundry.left + transBoundry.width ) < 0 || transBoundry.left > GAME_WIDTH ||
		( transBoundry.top + transBoundry.height ) < 0 || transBoundry.top > GAME_HEIGHT )
	{
		this->game->currentRoom = nextRoom;
		this->game->SwitchState( new StateGameplay( this->game ) );
	}
}

void StateGameplay::PlayTransitionSound()
{
	/*if ( entityManager.GetRobotCount() > 0 ) chicken = true;
	else chicken = false;

	if ( chicken )
	{
		game->assetManager.PlaySound( "chicken", true );
	}
	else
	{
		game->assetManager.PlaySound( "intruder", true );
	}*/
}

bool StateGameplay::ResetIfDead()
{
	if ( player.CheckReset() )
	{
		if ( playDemo )
		{
			ReturnToTitle();
		}
		else
		{
			if ( recordDemo )
			{
				PlayerInput last;
				last.down = true;
				demo.Record( last ); // Add input on current frame, so it doesn't end too early 
				demo.SaveToFile( game->GetConfigDir() + "demo1.xml" );
			}
			game->ResetState();
		}
		return true;
	}
	return false;
}

void StateGameplay::ReturnToTitle()
{
	game->window.setMouseCursorVisible( true );
	lastMove = Directions::W;
	//game->music.stop();
	game->PopState();
}

sf::Vector2f StateGameplay::GetPlayerStart(const Directions lastMove, EntityPlayer& player) const
{
	// Spawn player based on last move
	switch (lastMove)
	{
		// Keep in mind you'll end up on the other side that you exited
	case Directions::S:
		return sf::Vector2f(GAME_WIDTH / 2, GAME_HEIGHT - player.hitbox.height - WALL_WIDTH);
		break;
	case Directions::N:
		return sf::Vector2f(GAME_WIDTH / 2, WALL_WIDTH);
		break;
	case Directions::E:
		return sf::Vector2f(GAME_WIDTH - player.hitbox.width - WALL_HEIGHT, GAME_HEIGHT / 2);
		break;
	case Directions::W:
	default:
		return sf::Vector2f(WALL_HEIGHT, GAME_HEIGHT / 2);
		break;
	}

	// Should never happen
	return sf::Vector2f(0, 0);
}

void StateGameplay::AddLastMove( Directions move )
{
	lastFourMoves.push_back( move );
	if ( lastFourMoves.size() > 4 ) // Limit to four
		lastFourMoves.erase( lastFourMoves.begin() );
}

bool StateGameplay::CheckEasterEgg() const
{
	if ( lastFourMoves.size() == 4
		&& lastFourMoves[0] == Directions::S
		&& lastFourMoves[1] == Directions::E
		&& lastFourMoves[2] == Directions::N
		&& lastFourMoves[3] == Directions::E
		)
	{
		return true;
	}

	return false;
}

StateGameplay::~StateGameplay()
{
	//maze.ClearMap();
}