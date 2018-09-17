#include "gameSession.h"
#include "engine.h"
#include "windows.h"
#include "entityFactory.h"
#include "entity.h"
#include "utility.h"
#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>

namespace engine
{
	GameSession::GameSession()
		: m_player( nullptr )
	{
		for( int y = 0; y < LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 0; x < LOC_MAP_SIZE_X; ++x )
			{
				m_localMap[ x ][ y ] = nullptr;
			}
		}

		Initialize();
	}

	GameSession::~GameSession()
	{
		DeleteLocalArea();

		if( m_player )
		{
			delete m_player;
		}
	}

	void GameSession::Update()
	{
		const Vector2 prevLocalToWorld = m_localToWorld;

		HandleMovement();
	
		if( m_localToWorld != prevLocalToWorld )
		{
			RefreshLocalArea( m_localToWorld );
		}

		Draw();
		DrawDebug();
	}

	void GameSession::Initialize()
	{
		LoadWorld( "map_large.txt" );
		m_localToWorld = Vector2( 0, 0 );
		CreateLocalArea( m_localToWorld );

		m_player = new Player();
	}

	void GameSession::LoadWorld( const char* fileName )
	{
		std::ifstream file( fileName, std::ifstream::in );

		if( file.is_open() )
		{
			for( int y = 0; y < WORLD_MAP_SIZE_Y; ++y )
			{
				std::string line;
				std::getline( file, line );
				assert( line.size() <= WORLD_MAP_SIZE_X );

				for( int x = 0; x < WORLD_MAP_SIZE_X; ++x )
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

	void GameSession::CreateLocalArea( const Vector2& offset )
	{
		EntityFactory factory;

		for( int y = 0; y < LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 0; x < LOC_MAP_SIZE_X; ++x )
			{
				Entity* entityPtr = factory.CreateObject( m_worldMap[ offset.X + x ][ offset.Y + y ] );
				assert( entityPtr );

				assert( !m_localMap[ x ][ y ] );
				m_localMap[ x ][ y ] = entityPtr;
			}
		}
	}

	void GameSession::RefreshLocalArea( const Vector2& offset )
	{
		DeleteLocalArea();
		CreateLocalArea( offset );
	}

	void GameSession::DeleteLocalArea()
	{
		for( int y = 0; y < LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 0; x < LOC_MAP_SIZE_X; ++x )
			{
				delete m_localMap[ x ][ y ];
				m_localMap[ x ][ y ] = nullptr;
			}
		}
	}

	void GameSession::HandleMovement()
	{
		Dir dir = Engine::Get().GetInputManager().GetMovementDir();
		if( dir != Dir::Undefined )
		{
			if( IsMovementPossible( dir ) )
			{
				if( ShouldMoveLocalArea( dir ) )
				{
					MoveLocalArea( dir );
				}
				else
				{
					MovePlayer( dir );
				}
			}
		}	
	}

	bool GameSession::ShouldMoveLocalArea( Dir dir ) const
	{
		assert( m_player );

		Vector2 testedPos;
		utility::DirToOffset( dir, testedPos );
		testedPos += m_localToWorld;

		if( testedPos.X < 0 || testedPos.X > ( WORLD_MAP_SIZE_X - LOC_MAP_SIZE_X ) )
		{
			return false;
		}

		if( testedPos.Y < 0 || testedPos.Y > ( WORLD_MAP_SIZE_Y - LOC_MAP_SIZE_Y ) )
		{
			return false;
		}

		// todo - magic numbers
		if( ( m_player->GetPos().X < 7 ) && ( dir == Dir::Right ) )
		{
			return false;
		}

		if( ( m_player->GetPos().X > ( LOC_MAP_SIZE_X - 8 ) ) && ( dir == Dir::Left ) )
		{
			return false;
		}

		if( ( m_player->GetPos().Y < 4 ) && ( dir == Dir::Down ) )
		{
			return false;
		}

		if( ( m_player->GetPos().Y > ( LOC_MAP_SIZE_Y - 5 ) ) && ( dir == Dir::Up ) )
		{
			return false;
		}

		return true;
	}

	void GameSession::MoveLocalArea( Dir dir )
	{
		Vector2 offset;
		utility::DirToOffset( dir, offset );
		m_localToWorld += offset;
	}

	void GameSession::MovePlayer( Dir dir )
	{
		m_player->MoveInDir( dir );
	}

	bool GameSession::IsMovementPossible( Dir dir ) const
	{
		assert( m_player );

		Vector2 testedPos;
		utility::DirToOffset( dir, testedPos );
		testedPos += m_player->GetPos();

		if( IsWithinLocalBounds( testedPos ) )
		{
			if( m_localMap[ testedPos.X ][ testedPos.Y ]->IsWalkable() )
			{
				return true;
			}
		}

		return false;
	}

	bool GameSession::IsWithinLocalBounds( const Vector2& pos ) const
	{
		if( pos.X < 0 || pos.X >= LOC_MAP_SIZE_X )
		{
			return false;
		}

		if( pos.Y < 0 || pos.Y >= LOC_MAP_SIZE_Y )
		{
			return false;
		}

		return true;
	}

	void GameSession::Draw()
	{
		assert( m_player );

		Vector2 playerPos = m_player->GetPos();

		for( int y = 0; y < LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 0; x < LOC_MAP_SIZE_X; ++x )
			{
				if( playerPos.X == x && playerPos.Y == y )
				{
					std::cout << m_player->GetIcon();
				}
				else
				{
					std::cout << m_localMap[ x ][ y ]->GetIcon(); 
				}
			}

			std::cout << std::endl;
		}

		std::cout << "\n\n";
	}

	void GameSession::DrawDebug() const
	{
		assert( m_player );
		Vector2 playerLocalPos = m_player->GetPos();
		Vector2 playerWorldPos = Vector2( playerLocalPos.X + m_localToWorld.X, playerLocalPos.Y + m_localToWorld.Y );

		std::cout << "GameSession Debug:\n";
		std::cout << "--------------------------------\n";
		std::cout << "Player local pos: [ " << playerLocalPos.X << ", " << playerLocalPos.Y << " ]\n";
		std::cout << "Player world pos: [ " << playerWorldPos.X << ", " << playerWorldPos.Y << " ]\n";
		std::cout << "LocalToWorld: [ " << m_localToWorld.X << ", " << m_localToWorld.Y << " ]\n";
		std::cout << "\n\n";
	}
}