#include "state_gameplay.h"
#include "state_highscore.h"
#include "state_titlescreen.h"
#include "pugixml.hpp"

Directions StateGameplay::lastMove = Directions::W;
std::vector<Directions> StateGameplay::lastFourMoves = std::vector<Directions>();

bool StateGameplay::chicken = false;

StateGameplay::StateGameplay( Game *game, const bool recordDemo , const bool playDemo, const std::string demoName )
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
#ifdef OLD_SFML
	txScore.setColor( sf::Color::Green );
#else
	txScore.setFillColor( sf::Color::Green );
#endif
	txScore.setCharacterSize( 30 );
	txScore.setPosition( sf::Vector2f( 3, 3 ) );
	txScore.setString( game->score );

	for( unsigned int i = 0; i < MAX_LIVES; i++ )
	{
		lives[i].setTexture( game->assetManager.GetTextureRef( "sprites" ) );
		lives[i].setTextureRect( game->animManager.Animate( "life_icon" ) );
		lives[i].setScale( 4, 4 );
		lives[i].setPosition( (5 + lives[i].getGlobalBounds().width) * i, GAME_HEIGHT - lives[i].getGlobalBounds().height - 5 );
	}

	ottoSpawned = false;

	deathSoundPlayed = false;

	clock.restart();

	this->recordDemo = recordDemo;
	this->playDemo = playDemo;
	if ( playDemo )
	{
		demo.LoadFromFile( demoName );
		game->level = demo.GetLevel();
	}
	if ( recordDemo )
	{
		demo.SetLevel( game->level );
	}

	pause.SetGame( game );
	pauseTime = 0;
}

void StateGameplay::Start()
{

}

void StateGameplay::HandleInput()
{
	if ( pause.open )
	{
		pause.HandleInput();
		if ( pause.quit )
		{
			lastMove = Directions::W; // Make sure next started game starts at W
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
			sfx.stop();
			sfx.resetBuffer(); // Prevents crash on close
			game->Close();
			return;
		}

		if ( ResetIfDead() )
			return; // Do not continue if we're resetting the state

		if( event.type == sf::Event::KeyPressed )
		{
			if( event.key.code == sf::Keyboard::Escape || game->inputManager.TestKeyDown( "pause", event ) )
			{
				if ( recordDemo || playDemo )
				{
					lastMove = Directions::W; // Make sure next started game starts at W
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
			lastMove = Directions::W;  // Default lastmove on next started game
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

	if ( !playDemo )
	{
		while ( !maze.IsDone() )
		{
			maze.Generate();
		}
	}
	else
		maze.SkipGenerate();

	if ( maze.IsDone() && !wallsCreated )
	{
		if ( recordDemo )
			demo.SetWalls( maze.CreateWalls( entityManager ) );
		else if( playDemo )
			maze.LoadWalls( demo.GetWalls(), entityManager );
		else
			maze.CreateWalls( entityManager );

		if ( game->level >= 5 )
		{
			maze.BlockExit( entityManager, lastMove );
		}
		wallsCreated = true;
	}
	if( maze.IsDone() && !enemiesSpawned )
	{
		if ( recordDemo )
			demo.SetRobotPositions( maze.SpawnEnemies( entityManager, lastMove, LoadRobotStats() ) );
		else if ( playDemo )
			maze.LoadEnemies( demo.GetRobotPositions(), entityManager, LoadRobotStats() );
		else
			maze.SpawnEnemies( entityManager, lastMove, LoadRobotStats() );
		enemiesSpawned = true;

		sfx.setBuffer( game->assetManager.GetSoundRef( "humanoid", true ) );
		sfx.play();
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
			AddLastMove( lastMove );
			transStart = now;
		}
		else if( ( plPos.x + player.hitbox.width ) < 0 )
		{
			transition = true;
			game->level++;
			lastMove = Directions::E;
			AddLastMove( lastMove );
			transStart = now;
		}
		else if( plPos.y > GAME_HEIGHT )
		{
			transition = true;
			game->level++;
			lastMove = Directions::N;
			AddLastMove( lastMove );
			transStart = now;
		}
		else if( ( plPos.y + player.hitbox.height ) < 0 )
		{
			transition = true;
			game->level++;
			lastMove = Directions::S;
			AddLastMove( lastMove );
			transStart = now;
		}

		// Did we run out of lives?
		if( game->GetLives() <= 0 && player.CheckReset() )
		{
			this->game->SwitchState( new StateHighscore( this->game ) );
			return;
		}

		// Spawn Otto if delay has been reached
		if( pauseTime + now >= OTTO_DELAY && !ottoSpawned )
		{
			otto = new EntityOtto( sf::Vector2f( 0, player.hitbox.top ), player.hitbox.top, player.hitbox.top + player.hitbox.height, CheckEasterEgg() );
			entityManager.Add( otto );
			ottoSpawned = true;

			sfx.setBuffer( game->assetManager.GetSoundRef( "intruder_alert", true ) );
			sfx.play();
		}
		// Tell Otto where to move
		if( ottoSpawned )
		{
			otto->SetMinMaxHeight( player.hitbox.top - OTTO_BUFFER, player.hitbox.top + player.hitbox.height + OTTO_BUFFER );
		}

		if( player.IsDead() && !deathSoundPlayed )
		{
			if( chicken )
			{
				sfx.setBuffer( game->assetManager.GetSoundRef( "got_chicken", true ) );
			}
			else
			{
				sfx.setBuffer( game->assetManager.GetSoundRef( "got_humanoid", true ) );
			}
			sfx.play();

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

	if ( pause.open )
		pause.Draw();
}

void StateGameplay::ScreenTransition( const float dt )
{
	if( !captured )
	{
		// Capture the screen
		txTrans.create( game->window.getSize().x, game->window.getSize().y );

		// Update view so transition looks right with resized screen
		sf::View view;
		view = game->window.getDefaultView();
		view.setSize( game->window.getSize().x, game->window.getSize().y );
		view.setCenter( game->window.getSize().x / 2, game->window.getSize().y / 2 );
		game->window.setView( view );

		txTrans.update( game->window );
		sprTrans.setTexture( txTrans, true );
		captured = true;

		// Play sound
		if( entityManager.GetRobotCount() > 0 ) chicken = true;
		else chicken = false;

		if( chicken )
		{
			sfx.setBuffer( game->assetManager.GetSoundRef( "chicken", true ) );
		}
		else
		{
			sfx.setBuffer( game->assetManager.GetSoundRef( "intruder", true ) );
		}
		sfx.play();
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
			sfx.stop();
			game->window.setView( game->window.getDefaultView() );
			this->game->SwitchState( new StateGameplay( this->game ) );
		}
	}
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
	sfx.stop();
	sfx.resetBuffer();
	game->PopState();
}

RobotStats StateGameplay::LoadRobotStats()
{
	RobotStats stats{ false, true, 50, 3000, 5, ERROR_COLOR }; // Default values in case of error

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( "assets/robotstats.xml" );
	if( !result ) // Error check
	{
		return stats;
	}

	try
	{
		pugi::xml_node levelNodes = doc.child( "levels" );

		for ( pugi::xml_node level : levelNodes.children( "level" ) )
		{
			if ( game->level >= (unsigned int)std::stoi( level.attribute( "min" ).value() ) )
			{
				if ( level.attribute( "stop_if_see_player") != NULL && std::stoi( level.attribute( "stop_if_see_player" ).value() ) == 0 )
					stats.stopIfSeePlayer = false;
				else
					stats.stopIfSeePlayer = true;

				if ( level.attribute( "speed" ) != NULL ) stats.movementSpeed = (float)std::stoi( level.attribute( "speed" ).value() );
				else stats.movementSpeed = 5;

				if ( level.attribute( "firedelay" ) != NULL ) stats.fireDelay = std::stoi( level.attribute( "firedelay" ).value() );
				else stats.fireDelay = 100;

				if ( level.attribute( "num_bots" ) != NULL ) stats.numRobots = std::stoi( level.attribute( "num_bots" ).value() );
				else stats.numRobots = 5;

				if( level.attribute( "scale" ) != NULL ) stats.scale = std::stof( level.attribute( "scale" ).value() );
				else stats.scale = 1;

				if ( level.attribute( "can_shoot" ) != NULL && level.attribute( "can_shoot" ).value() == "0" )
					stats.canShoot = false;
				else
					stats.canShoot = true;

				if ( level.child( "color" ) != NULL )
				{
					pugi::xml_node color = level.child( "color" );
					int r = std::stoi( color.attribute( "r" ).value() );
					int g = std::stoi( color.attribute( "g" ).value() );
					int b = std::stoi( color.attribute( "b" ).value() );
					stats.color = sf::Color( r, g, b );
				}
				else
					stats.color = ERROR_COLOR;
			}
		}

		return stats;
	}
	catch ( int e )
	{
		game->ThrowError( "Error while loading assets/robotstats.xml" );
		return stats;
	}
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
	sfx.stop();
	sfx.resetBuffer();
}