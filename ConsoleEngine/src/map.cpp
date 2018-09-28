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
	OnLoad();
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
			Entity* entityPtr = factory.CreateObject( m_worldMap[ m_localToWorld.X + x ][ m_localToWorld.Y ] );
			assert( entityPtr );

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
			Entity* entityPtr = factory.CreateObject( m_worldMap[ m_localToWorld.X + x ][ m_localToWorld.Y + config::LOC_MAP_SIZE_Y - 1 ] );
			assert( entityPtr );

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
			Entity* entityPtr = factory.CreateObject( m_worldMap[ m_localToWorld.X + config::LOC_MAP_SIZE_X - 1 ][ m_localToWorld.Y + y ] );
			assert( entityPtr );

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
			Entity* entityPtr = factory.CreateObject( m_worldMap[ m_localToWorld.X ][ m_localToWorld.Y + y ] );
			assert( entityPtr );

			m_localMap[ 0 ][ y ] = entityPtr;
		}
	}
}

void Map::SetLocalToWorld( const Vector2& l2w )
{
	m_localToWorld = l2w;
}

const Vector2& Map::GetLocalToWorld() const
{
	return m_localToWorld;
}

Entity* Map::GetEntityAt( const Vector2& pos ) const
{
	return m_localMap[ pos.X ][ pos.Y ];
}

void Map::OnSave()
{
	engine::Engine::Get().GetSaveSystem().Save( SaveDataType::MapPos, this );
}

void Map::OnLoad()
{
	engine::Engine::Get().GetSaveSystem().Load( SaveDataType::MapPos, this );

	DeleteLocalMap();
	CreateLocalMap( m_localToWorld );
}
