#pragma once

#include "types.h"

class Entity;
class Player;

namespace engine
{
	class GameSession
	{
		using Uint8 = unsigned char;

		// todo - move to some config class
		static constexpr int WORLD_MAP_SIZE_X = 64;
		static constexpr int WORLD_MAP_SIZE_Y = 40;
		static constexpr int LOC_MAP_SIZE_X = 15;
		static constexpr int LOC_MAP_SIZE_Y = 9;

	public:
		GameSession();
		~GameSession();

		void Update();

	private:
		void Initialize();
		void LoadWorld( const char* fileName );
		void CreateLocalArea( const Vector2& offset );
		void RefreshLocalArea( const Vector2& offset );
		void DeleteLocalArea();
		
		void HandleMovement();
		bool IsMovementPossible( Dir dir ) const;
		bool IsWithinLocalBounds( const Vector2& pos ) const;
		bool ShouldMoveLocalArea( Dir dir ) const;
		void MoveLocalArea( Dir dir );
		void MovePlayer( Dir dir );

		void Draw();
		void DrawDebug() const;

	private:
		Uint8 m_worldMap[ WORLD_MAP_SIZE_X ][ WORLD_MAP_SIZE_Y ];
		Entity* m_localMap[ LOC_MAP_SIZE_X ][ LOC_MAP_SIZE_Y ];

		Vector2 m_localToWorld; // is the name correct?
		Player* m_player;
	};
}