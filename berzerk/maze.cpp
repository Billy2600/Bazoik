#include "maze.h"

bool Maze::CheckBounds(sf::Vector2u pos) const
{
	// Bounds checking
	if (pos.x >= 0 && pos.x < MAZE_WIDTH && 
		pos.y >= 0 && pos.y < MAZE_HEIGHT)
		return true;
	else
		return false;
}

void Maze::MarkVisited(sf::Vector2u pos)
{
	if (CheckBounds(pos))
	{
		map[pos.x][pos.y].visited = true;
		numVisited++;
	}
}

Maze::Maze()
{
	ClearMap();
}

bool Maze::IsDone() const
{
	return done;
}

void Maze::Generate()
{
	if (done) return; // Don't do any of this if we're done

	// Visit every tile
	if (numVisited < MAZE_WIDTH * MAZE_HEIGHT)
	{
		// Surrounding tile positions, not including diagonals
		sf::Vector2u n[4] = { sf::Vector2u(x + 1, y), sf::Vector2u(x - 1, y),
			sf::Vector2u(x, y + 1), sf::Vector2u(x, y - 1) };
		// Make sure no neighbors are out of bounds
		for (int i = 0; i < 4; i++)
		{
			if (!CheckBounds(n[i]))
				n[i] = sf::Vector2u(x,y); // Hopefully has been visited
		}

		int i = 0;
		// Does current cell have unvisited neighbors?
		if (!map[n[0].x][n[0].y].visited || !map[n[1].x][n[1].y].visited ||
			!map[n[2].x][n[2].y].visited || !map[n[3].x][n[3].y].visited)
		{
			// Randomly choose neighbor that hasn't been visited
			bool chosen = false;
			unsigned int rndNeighbor = 0; // random neighbor tile
			std::uniform_int_distribution<unsigned int> rand( 0, 3 );
			while (!chosen)
			{
				// Choose neighbor and check if its visited
				rndNeighbor = rand( rngEngine );
				if( !map[n[rndNeighbor].x][n[rndNeighbor].y].visited ) // Hasn't been visited
				{
					chosen = true; // Set chosen flag
				}
			}

			// Attempt to get out of error state
			if( !CheckBounds( sf::Vector2u( x, y ) ) )
			{
				if( !CheckBounds( sf::Vector2u( x, 1 ) ) ) x++;
				if( !CheckBounds( sf::Vector2u( 1, y ) ) ) y++;
				return;
			}

			// Push current tile onto stack for backtracking
			stTiles.push(sf::Vector2u(x, y));

			// Remove wall between current tile and chosen tile
			if (n[rndNeighbor].x < x)
			{
				map[n[rndNeighbor].x][n[rndNeighbor].y].right = false;
				map[x][y].left = false;
			}
			else if (n[rndNeighbor].x > x)
			{
				map[n[rndNeighbor].x][n[rndNeighbor].y].left = false;
				map[x][y].right = false;
			}
			if (n[rndNeighbor].y < y)
			{
				map[n[rndNeighbor].x][n[rndNeighbor].y].bottom = false;
				map[x][y].top = false;
			}
			else if (n[rndNeighbor].y > y)
			{
				map[n[rndNeighbor].x][n[rndNeighbor].y].top = false;
				map[x][y].bottom = false;
			}

			// Make chosen tile the current one and mark it as visited
			x = n[rndNeighbor].x;
			y = n[rndNeighbor].y;
			MarkVisited(n[rndNeighbor]);
		}
		// Is the stack not empty?
		else if (!stTiles.empty())
		{
			// Make top tile on stack current tile and pop it off
			sf::Vector2u newPos = stTiles.top();
			if( CheckBounds( newPos ) )
			{
				stTiles.pop();
				x = newPos.x;
				y = newPos.y;
			}
		}
		// Pick a random unvisited tile
		else
		{
			std::uniform_int_distribution<unsigned int> rand1( 0, MAZE_WIDTH );
			std::uniform_int_distribution<unsigned int> rand2( 0, MAZE_HEIGHT );
			unsigned int randX = rand1( rngEngine );
			unsigned int randY = rand2( rngEngine );
			// Check if tile has already been visited
			if (!map[randX][randY].visited)
			{
				// Make it current tile
				x = randX;
				y = randY;
				// Mark it visited
				MarkVisited(sf::Vector2u(x, y));
			}
		}
	}
	// No more left to visit, set as done
	else
	{
		//exit = sf::Vector2u(x, y);
		done = true;
		CreateGaps();
	}
}

void Maze::CreateGaps()
{
	const int halfHeight = MAZE_HEIGHT / 2;
	const int halfWidth = MAZE_WIDTH / 2;

	for( unsigned int y = 0; y < MAZE_HEIGHT; y++ )
	{
		for( unsigned int x = 0; x < MAZE_WIDTH; x++ )
		{
			// Cut out sides in maze
			if( y == 0 && x == halfWidth )
				map[x][y].top = false;
			if( y == MAZE_HEIGHT - 1 && x == halfWidth )
				map[x][y].bottom = false;
			if( y == halfHeight && x == 0 )
				map[x][y].left = false;
			if( y == halfHeight && x == MAZE_WIDTH - 1 )
				map[x][y].right = false;
		}
	}
}

void Maze::ClearMap()
{
	// X/Y iterators
	unsigned int x;
	unsigned int y;

	for (y = 0; y < MAZE_HEIGHT; y++)
	{
		for (x = 0; x < MAZE_WIDTH; x++)
		{
			const bool val = true; // All initialize to this
			map[x][y].top =		val;
			map[x][y].bottom =	val;
			map[x][y].left =	val;
			map[x][y].right =	val;
			// Set visited to false
			map[x][y].visited = false;
		}
	}
	// Reset number of tiles visited
	numVisited = 0;
	done = false;
	
	std::uniform_int_distribution<unsigned int> rand1( 0, MAZE_WIDTH );
	std::uniform_int_distribution<unsigned int> rand2( 0, MAZE_HEIGHT );
	// Start with random tile
	this->x = rand1( rngEngine );
	this->y = rand2( rngEngine );
	// Mark it visited
	MarkVisited(sf::Vector2u(this->x, this->y));
}

void Maze::CreateWalls( EntityManager &entityManager ) const
{
	// x/y iterators
	unsigned int x;
	unsigned int y;

	// Draw tiles
	for( y = 0; y < MAZE_HEIGHT; y++ )
	{
		for( x = 0; x < MAZE_WIDTH; x++ )
		{
			// Vertex array for maze tile
			sf::Vertex lines[8];

			// Create lines based on which sides are filled
			if( map[x][y].top )
			{
				entityManager.Add( new EntityWall( sf::Vector2f( (float)(x * tileWidth), (float)(y * tileHeight) ),
					sf::Vector2f( (float)tileWidth, WALL_THICKNESS ) ) );
			}
			if( map[x][y].bottom )
			{
				sf::Vector2f pos = sf::Vector2f( (float)( x * tileWidth ), (float)( y * tileHeight + tileHeight ) );
				// Move bottom row up
				float width = (float)tileWidth + WALL_THICKNESS; // Prevent gaps in corners
				if( y == MAZE_HEIGHT - 1 )
				{
					pos.y -= WALL_THICKNESS;
					width -= WALL_THICKNESS;
				}

				entityManager.Add( new EntityWall( pos, sf::Vector2f( width, WALL_THICKNESS ) ) );
			}
			if( map[x][y].left )
			{
				entityManager.Add( new EntityWall( sf::Vector2f( (float)(x * tileWidth), (float)(y * tileHeight) ),
					sf::Vector2f( WALL_THICKNESS, (float)tileHeight ) ) );
			}
			if( map[x][y].right )
			{
				sf::Vector2f pos = sf::Vector2f( (float)( x * tileWidth + tileWidth ), (float)( y * tileHeight ) );
				if( x == MAZE_WIDTH - 1 )
				{
					pos.x -= WALL_THICKNESS / 2; // Not sure why this is half offset...
				}
				entityManager.Add( new EntityWall( pos, sf::Vector2f( WALL_THICKNESS, (float)tileHeight ) ) );
			}
		}
	}
}

void Maze::BlockExit( EntityManager &entityManager, Directions lastMove ) const
{
	if( lastMove == Directions::N )
	{
		entityManager.Add( new EntityWall( sf::Vector2f( (float)( tileWidth * int( MAZE_WIDTH / 2 ) ), 0 ),
			sf::Vector2f( (float)tileWidth, WALL_THICKNESS / 2 ), sf::Color::Red ) );
	}
	else if( lastMove == Directions::S )
	{
		entityManager.Add( new EntityWall( sf::Vector2f( (float)( tileWidth * int( MAZE_WIDTH / 2 ) ), (float)( tileWidth * MAZE_HEIGHT ) - ( WALL_THICKNESS *3 ) ),
			sf::Vector2f( (float)tileWidth, WALL_THICKNESS / 2 ), sf::Color::Red ) );
	}
	else if( lastMove == Directions::W )
	{
		entityManager.Add( new EntityWall( sf::Vector2f( 0, (float)tileHeight ),
			sf::Vector2f( WALL_THICKNESS / 2, (float)tileHeight ), sf::Color::Red ) );
	}
	else if( lastMove == Directions::E )
	{
		entityManager.Add( new EntityWall( sf::Vector2f( (float)( tileWidth * MAZE_WIDTH ), (float)tileHeight ),
			sf::Vector2f( WALL_THICKNESS, (float)tileHeight ), sf::Color::Red ) );
	}
}

void Maze::SpawnEnemies( EntityManager& entityManager, const Directions lastMove, RobotStats stats )
{
	std::vector<sf::Vector2u> used;

	// Pick random tile
	for( int i = 0; i < stats.numRobots; i++ )
	{
		std::uniform_int_distribution<unsigned int> randX( 0, MAZE_WIDTH - 1 );
		std::uniform_int_distribution<unsigned int> randY( 0, MAZE_HEIGHT - 1 );
		unsigned int tileX = randX( rngEngine );
		unsigned int tileY = randY( rngEngine );
		// Don't spawn in the same tile as player
		if( (lastMove == Directions::S && tileX == (unsigned int)( MAZE_WIDTH / 2 ) && tileY == MAZE_HEIGHT - 1) ||
			( lastMove == Directions::N && tileX == (unsigned int)( MAZE_WIDTH / 2 ) && tileY == 0 ) ||
			( lastMove == Directions::E && tileX == MAZE_WIDTH - 1 && tileY == (unsigned int)( MAZE_HEIGHT / 2 ) ) ||
			( lastMove == Directions::W && tileX == 0 && tileY == (unsigned int)( MAZE_HEIGHT / 2 ) )
		)
		{
			i--;
			continue;
		}
		// Don't spawn enemies inside same tile, unless there's a lot of them
		if( stats.numRobots <= (MAZE_WIDTH * MAZE_HEIGHT) )
		{
			bool repeatLoop = false;
			for( auto tile : used )
			{
				if( sf::Vector2u( tileX, tileY ) == tile )
				{
					repeatLoop = true; // Repeat parent loop, not this one
					break;
				}
			}
			if( repeatLoop )
			{
				i--;
				continue;
			}
		}

		used.push_back( sf::Vector2u( tileX, tileY ) );

		// Get bounds of tile
		EntityRobot tempRobot( sf::Vector2f( 0, 0 ) ); // Used for sizing
		float minX = (float)((tileX * tileWidth) + WALL_THICKNESS);
		float maxX = (float)((tileX * tileWidth) + tileWidth - tempRobot.hitbox.width);
		float minY = (float)((tileY * tileHeight) + WALL_THICKNESS);
		float maxY = (float)((tileY * tileHeight) + tileHeight - tempRobot.hitbox.height);

		// Pick random point inside bounds
		std::uniform_real_distribution<float> randBoundX( minX, maxX );
		std::uniform_real_distribution<float> randBoundY( minY, maxY );
		float x = randBoundX( rngEngine );
		float y = randBoundY( rngEngine );

		entityManager.Add( new EntityRobot( sf::Vector2f( x, y ), stats.stopIfSeePlayer, stats.movementSpeed, stats.fireDelay, stats.color ) );
	}
}

sf::Vector2f Maze::GetPlayerStart( const Directions lastMove, EntityPlayer &player ) const
{
	// Spawn player based on last move
	switch( lastMove )
	{
	// Keep in mind you'll end up on the other side that you exited
	case Directions::S:
		return sf::Vector2f( GAME_WIDTH / 2, GAME_HEIGHT - player.hitbox.height - WALL_THICKNESS );
		break;
	case Directions::N:
		return sf::Vector2f( GAME_WIDTH /2, WALL_THICKNESS);
		break;
	case Directions::E:
		return sf::Vector2f( GAME_WIDTH - player.hitbox.width - WALL_THICKNESS, GAME_HEIGHT / 2 );
		break;
	case Directions::W:
	default:
		return sf::Vector2f( WALL_THICKNESS, GAME_HEIGHT / 2 );
		break;
	}

	// Should never happen
	return sf::Vector2f( 0, 0 );
}