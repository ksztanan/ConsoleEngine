#include "gameSession.h"
#include "engine.h"
#include "entity.h"
#include "utility.h"
#include "map.h"

#include <iostream>
#include <assert.h>

namespace engine
{
	GameSession::GameSession()
		: m_player( nullptr )
	{
		Initialize();
	}

	GameSession::~GameSession()
	{
		if( m_player )
		{
			delete m_player;
		}
	}

	void GameSession::Update()
	{
		HandleMovement();
		Draw();
		DrawDebug();
	}

	void GameSession::Initialize()
	{
		m_map.Initialize();
		m_player = new Player();
	}

	void GameSession::HandleMovement()
	{
		Dir dir = Engine::Get().GetInputManager().GetMovementDir();
		if( dir != Dir::Undefined )
		{
			if( IsMovementPossible( dir ) )
			{
				if( ShouldMoveLocalMap( dir ) )
				{
					m_map.MoveLocalMap( dir );
				}
				else
				{
					m_player->MoveInDir( dir );
				}
			}
		}	
	}

	bool GameSession::IsMovementPossible( Dir dir ) const
	{
		assert( m_player );

		Vector2 testedPos;
		utility::DirToOffset( dir, testedPos );
		testedPos += m_player->GetPos();

		if( IsWithinLocalBounds( testedPos ) )
		{
			if( m_map.GetEntityAt( testedPos )->IsWalkable() )
			{
				return true;
			}
		}

		return false;
	}

	bool GameSession::IsWithinLocalBounds( const Vector2& pos ) const
	{
		if( pos.X < 0 || pos.X >= config::LOC_MAP_SIZE_X )
		{
			return false;
		}

		if( pos.Y < 0 || pos.Y >= config::LOC_MAP_SIZE_Y )
		{
			return false;
		}

		return true;
	}

	bool GameSession::ShouldMoveLocalMap( Dir dir ) const
	{
		assert( m_player );

		Vector2 testedPos;
		utility::DirToOffset( dir, testedPos );
		testedPos += m_map.GetLocalToWorld();

		if( testedPos.X < 0 || testedPos.X > ( config::WORLD_MAP_SIZE_X - config::LOC_MAP_SIZE_X ) )
		{
			return false;
		}

		if( testedPos.Y < 0 || testedPos.Y > ( config::WORLD_MAP_SIZE_Y - config::LOC_MAP_SIZE_Y ) )
		{
			return false;
		}

		// todo - magic numbers
		if( ( m_player->GetPos().X < 7 ) && ( dir == Dir::Right ) )
		{
			return false;
		}

		if( ( m_player->GetPos().X > ( config::LOC_MAP_SIZE_X - 8 ) ) && ( dir == Dir::Left ) )
		{
			return false;
		}

		if( ( m_player->GetPos().Y < 4 ) && ( dir == Dir::Down ) )
		{
			return false;
		}

		if( ( m_player->GetPos().Y > ( config::LOC_MAP_SIZE_Y - 5 ) ) && ( dir == Dir::Up ) )
		{
			return false;
		}

		return true;
	}

	void GameSession::Draw()
	{
		assert( m_player );

		Vector2 playerPos = m_player->GetPos();

		for( int y = 0; y < config::LOC_MAP_SIZE_Y; ++y )
		{
			for( int x = 0; x < config::LOC_MAP_SIZE_X; ++x )
			{
				if( playerPos.X == x && playerPos.Y == y )
				{
					std::cout << m_player->GetIcon();
				}
				else
				{
					std::cout << m_map.GetEntityAt( Vector2( x, y ) )->GetIcon(); 
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
		Vector2 localToWorld = m_map.GetLocalToWorld();
		Vector2 playerWorldPos = Vector2( playerLocalPos.X + localToWorld.X, playerLocalPos.Y + localToWorld.Y );

		std::cout << "GameSession Debug:\n";
		std::cout << "--------------------------------------\n";
		std::cout << "Player local pos: [ " << playerLocalPos.X << ", " << playerLocalPos.Y << " ]\n";
		std::cout << "Player world pos: [ " << playerWorldPos.X << ", " << playerWorldPos.Y << " ]\n";
		std::cout << "LocalToWorld: [ " << localToWorld.X << ", " << localToWorld.Y << " ]\n";
		std::cout << "\n\n";
	}
}