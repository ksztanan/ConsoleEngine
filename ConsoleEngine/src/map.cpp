#include "map.h"
#include "entityFactory.h"
#include "utility.h"
#include "engine.h"

#include <fstream>
#include <string>
#include <assert.h>

Map::Map()
	: m_localToWorld( Vector2( 0, 0 ) )
{
	for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
	{
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			m_localMap[ x ][ y ] = nullptr;
		}
	}

	LoadWorld( "data/map_large.txt" );
}

Map::~Map()
{
	DeleteLocalMap();
}

void Map::MoveLocalMap( Dir dir )
{
	Vector2 offset;
	utility::DirToOffset( dir, offset );
	m_localToWorld += offset;

	HandleStreaming( dir );
}

void Map::LoadWorld( const char* fileName )
{
	std::ifstream file( fileName, std::ifstream::in );

	if( file.is_open() )
	{
		for( int y = 0; y < config::WORLD_MAP_SIZE_Y; ++y )
		{
			std::string line;
			std::getline( file, line );
			assert( line.size() <= config::WORLD_MAP_SIZE_X );

			for( int x = 0; x < config::WORLD_MAP_SIZE_X; ++x )
			{
				m_worldMap[ x ][ y ] = line[ x ];
			}
		}

		file.close();
	}
	else
	{
		assert( false );
		return;
	}
}

void Map::CreateLocalMap()
{
	EntityFactory factory;

	for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
	{
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			const Vector2 pos = Vector2( m_localToWorld.X + x, m_localToWorld.Y + y );
			Entity* entityPtr = CreateObjectAtPos( factory, pos );

			assert( !m_localMap[ x ][ y ] );
			m_localMap[ x ][ y ] = entityPtr;
		}
	}
}

void Map::DeleteLocalMap()
{
	for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
	{
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			delete m_localMap[ x ][ y ];
			m_localMap[ x ][ y ] = nullptr;
		}
	}
}

void Map::HandleStreaming( Dir dir )
{
	EntityFactory factory;
	if( dir == Dir::Up )
	{
		//delete bottom row
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			delete m_localMap[ x ][ config::LOC_MAP_SIZE_Y - 1 ];
		}

		// move rows down
		for( int y = config::LOC_MAP_SIZE_Y - 1; y > 0; --y )
		{
			for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
			{
				m_localMap[ x ][ y ] = m_localMap[ x ][ y - 1 ];
			}
		}

		//create and insert top row
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			const Vector2 pos = Vector2( m_localToWorld.X + x, m_localToWorld.Y );
			Entity* entityPtr = CreateObjectAtPos( factory, pos );

			m_localMap[ x ][ 0 ] = entityPtr;
		}
	}
	else if( dir == Dir::Down )
	{
		//delete top row
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			delete m_localMap[ x ][ 0 ];
		}

		// move rows up
		for( int y = 1; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
			{
				m_localMap[ x ][ y - 1 ] = m_localMap[ x ][ y ];
			}
		}

		//create and insert bottom row
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			const Vector2 pos = Vector2( m_localToWorld.X + x, m_localToWorld.Y + config::LOC_MAP_SIZE_Y - 1 );
			Entity* entityPtr = CreateObjectAtPos( factory, pos );

			m_localMap[ x ][ config::LOC_MAP_SIZE_Y - 1 ] = entityPtr;
		}
	}
	else if( dir == Dir::Right )
	{
		//delete left column
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			delete m_localMap[ 0 ][ y ];
		}

		// move columns left
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 1; x < config::LOC_MAP_SIZE_X; ++x )
			{
				m_localMap[ x - 1 ][ y ] = m_localMap[ x ][ y ];
			}
		}

		//create and insert right column
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			const Vector2 pos = Vector2( m_localToWorld.X + config::LOC_MAP_SIZE_X - 1, m_localToWorld.Y + y );
			Entity* entityPtr = CreateObjectAtPos( factory, pos );

			m_localMap[ config::LOC_MAP_SIZE_X - 1 ][ y ] = entityPtr;
		}
	}
	else if( dir == Dir::Left )
	{	
		//delete right column
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			delete m_localMap[ config::LOC_MAP_SIZE_X - 1 ][ y ];
		}

		// move columns right
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = config::LOC_MAP_SIZE_X - 1; x > 0; --x )
			{
				m_localMap[ x ][ y ] = m_localMap[ x - 1 ][ y ];
			}
		}

		//create and insert left column
		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			const Vector2 pos = Vector2( m_localToWorld.X, m_localToWorld.Y + y );
			Entity* entityPtr = CreateObjectAtPos( factory, pos );

			m_localMap[ 0 ][ y ] = entityPtr;
		}
	}
}

Entity* Map::CreateObjectAtPos( EntityFactory& factory, const Vector2& pos )
{
	Entity* entityPtr = factory.CreateObject( m_worldMap[ pos.X ][ pos.Y ] );
	assert( entityPtr );

	int size = m_destroyedEntities.size();
	for( int i = 0; i < size; ++i )
	{
		if( m_destroyedEntities[ i ] == pos )
		{
			if( Terrain* terrain = (Terrain*)entityPtr )
			{
				terrain->Destroy();
			}
			break;
		}
	}

	return entityPtr;
}

void Map::SetLocalToWorld( const Vector2& l2w )
{
	m_localToWorld = l2w;
}

void Map::SetDestroyedEntities( const std::vector< Vector2 >& entities )
{
	m_destroyedEntities.reserve( entities.size() );
	m_destroyedEntities = entities;
}

const Vector2& Map::GetLocalToWorld() const
{
	return m_localToWorld;
}

const std::vector< Vector2 >& Map::GetDestroyedEntities() const
{
	return m_destroyedEntities;
}

Entity* Map::GetEntityAt( const Vector2& pos ) const
{
	return m_localMap[ pos.X ][ pos.Y ];
}

void Map::DestroyEntityAt( const Vector2& pos )
{
	const Vector2 testedPos = m_localToWorld + pos;
	for( auto destroyedEnt : m_destroyedEntities )
	{
		if( destroyedEnt == testedPos )
		{
			return;
		}
	}

	m_destroyedEntities.push_back( testedPos );
}

void Map::OnLoad()
{
	DeleteLocalMap();
	CreateLocalMap();
}
