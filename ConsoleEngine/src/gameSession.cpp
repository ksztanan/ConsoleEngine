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
		m_player = new Player();

		LoadSessionData();
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
		if( engine::Engine::Get().GetInputManager().IsActionActive( InputAction::SaveGame ) )
		{
			SaveSessionData();
		}

		HandleInteractions();
		HandleMovement();
		Draw();
	}

	void GameSession::HandleInteractions()
	{
		if( Engine::Get().GetInputManager().IsActionActive( InputAction::Interact ) )
		{
			Vector2 playerPos = m_player->GetPos();

			for( int i = -1; i <= 1; ++i )
			{
				Vector2 pos = Vector2( playerPos.X + i, playerPos.Y );
				if( IsWithinLocalBounds( pos ) )
				{
					if( m_player->Interact( m_map.GetEntityAt( pos ) ) == InteractionResult::TerrainDestroyed )
					{
						m_map.DestroyEntityAt( pos );
					}
				}

				pos = Vector2( playerPos.X, playerPos.Y + i );
				if( IsWithinLocalBounds( pos ) )
				{
					if( m_player->Interact( m_map.GetEntityAt( pos ) ) == InteractionResult::TerrainDestroyed )
					{
						m_map.DestroyEntityAt( pos );
					}
				}
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

		if( testedPos.X < 0 || testedPos.X > ( config::WORLD_MAP_SIZE_X - config::LOC_MAP_SIZE_X ) )						{ return false; }
		if( testedPos.Y < 0 || testedPos.Y > ( config::WORLD_MAP_SIZE_Y - config::LOC_MAP_SIZE_Y ) )						{ return false; }
		if( ( m_player->GetPos().X < config::PLAYER_SPAWN_X ) && ( dir == Dir::Right ) )									{ return false; }
		if( ( m_player->GetPos().X > ( config::LOC_MAP_SIZE_X - config::PLAYER_SPAWN_X + 1 ) ) && ( dir == Dir::Left ) )	{ return false; }
		if( ( m_player->GetPos().Y < config::PLAYER_SPAWN_Y ) && ( dir == Dir::Down ) )										{ return false; }
		if( ( m_player->GetPos().Y > ( config::LOC_MAP_SIZE_Y - config::PLAYER_SPAWN_Y + 1 ) ) && ( dir == Dir::Up ) )		{ return false; }

		return true;
	}

	void GameSession::SaveSessionData()
	{
		SaveData data;
		data.m_playerPos = m_player->GetPos();
		data.m_mapPos = m_map.GetLocalToWorld();
		data.m_destroyedEntities = m_map.GetDestroyedEntities();

		engine::Engine::Get().GetSaveSystem().Save( data );
	}

	void GameSession::LoadSessionData()
	{
		SaveData data;
		if( engine::Engine::Get().GetSaveSystem().Load( data ) )
		{
			m_player->SetPos( data.m_playerPos );
			m_map.SetLocalToWorld( data.m_mapPos );
			m_map.SetDestroyedEntities( data.m_destroyedEntities );
		}
		m_map.OnLoad();
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
		std::cout << "CURRENT DATA" << "\t\t\t" << "[Vec2]" << "\n";
		std::cout << "Player local pos:" << "\t\t" << "[" << playerLocalPos.X << "," << playerLocalPos.Y << "]\n";
		std::cout << "Player world pos:" << "\t\t" << "[" << playerWorldPos.X << "," << playerWorldPos.Y << "]\n";
		std::cout << "Map LocalToWorld:" << "\t\t" << "[" << localToWorld.X << "," << localToWorld.Y << "]\n";
		std::cout << "\n\n";
	}
}