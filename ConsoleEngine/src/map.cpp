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

	RefreshLocalMap( m_localToWorld );
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

void Map::RefreshLocalMap( const Vector2& offset )
{
	DeleteLocalMap();
	CreateLocalMap( offset );
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

const Vector2& Map::GetLocalToWorld() const
{
	return m_localToWorld;
}

const Entity* Map::GetEntityAt( const Vector2& pos ) const
{
	return m_localMap[ pos.X ][ pos.Y ];
}
