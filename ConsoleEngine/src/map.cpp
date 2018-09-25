#include "map.h"
#include "entityFactory.h"
#include "utility.h"

#include <fstream>
#include <string>
#include <assert.h>

Map::Map()
{
	for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
	{
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			m_localMap[ x ][ y ] = nullptr;
		}
	}
}

Map::~Map()
{
	DeleteLocalMap();
}

void Map::Initialize()
{
	LoadWorld( "data/map_large.txt" );
	m_localToWorld = Vector2( 0, 0 );
	CreateLocalMap( m_localToWorld );
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

void Map::CreateLocalMap( const Vector2& offset )
{
	EntityFactory factory;

	for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
	{
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			Entity* entityPtr = factory.CreateObject( m_worldMap[ offset.X + x ][ offset.Y + y ] );
			assert( entityPtr );

			assert( !m_localMap[ x ][ y ] );
			m_localMap[ x ][ y ] = entityPtr;
		}
	}
}

void Map::DeleteLocalMap()
{
	for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
	{
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			delete m_localMap[ x ][ y ];
			m_localMap[ x ][ y ] = nullptr;
		}
	}
}

void Map::HandleStreaming( Dir dir )
{
	Uint8 newObj = 0;
	Uint8 deletedObj = 0;

	EntityFactory factory;
	if( dir == Dir::Up )
	{
		//delete bottom line
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			delete m_localMap[ x ][ config::LOC_MAP_SIZE_Y - 1 ];
			++deletedObj;
		}

		// move lines down
		for( int y = config::LOC_MAP_SIZE_Y - 1; y > 0; --y )
		{
			for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
			{
				m_localMap[ x ][ y ] = m_localMap[ x ][ y - 1 ];
			}
		}

		//create and insert top line
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			Entity* entityPtr = factory.CreateObject( m_worldMap[ m_localToWorld.X + x ][ m_localToWorld.Y ] );
			assert( entityPtr );
			++newObj;

			m_localMap[ x ][ 0 ] = entityPtr;
		}
	}
	else if( dir == Dir::Down )
	{
		//delete top line
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			delete m_localMap[ x ][ 0 ];
			++deletedObj;
		}

		// move lines up
		for( int y = 1; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
			{
				m_localMap[ x ][ y - 1 ] = m_localMap[ x ][ y ];
			}
		}

		//create and insert bottom line
		for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
		{
			Entity* entityPtr = factory.CreateObject( m_worldMap[ m_localToWorld.X + x ][ m_localToWorld.Y + config::LOC_MAP_SIZE_Y - 1 ] );
			assert( entityPtr );
			++newObj;

			m_localMap[ x ][ config::LOC_MAP_SIZE_Y - 1 ] = entityPtr;
		}
	}
	else if( dir == Dir::Right )
	{
		DeleteLocalMap();
		CreateLocalMap( m_localToWorld );
	}
	else if( dir == Dir::Left )
	{	
		DeleteLocalMap();
		CreateLocalMap( m_localToWorld );
	}

	assert( newObj == deletedObj );
}


const Vector2& Map::GetLocalToWorld() const
{
	return m_localToWorld;
}

const Entity* Map::GetEntityAt( const Vector2& pos ) const
{
	return m_localMap[ pos.X ][ pos.Y ];
}