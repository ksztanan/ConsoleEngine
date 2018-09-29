#pragma once

#include "types.h"
#include "config.h"
#include "map.h"

class Player;

namespace engine
{
	class GameSession
	{
	public:
		GameSession();
		~GameSession();

		void Update();
		void DrawDebug() const;

	private:
		void HandleInteractions();
		void HandleMovement();
		bool IsMovementPossible( Dir dir ) const;
		bool IsWithinLocalBounds( const Vector2& pos ) const;
		bool ShouldMoveLocalMap( Dir dir ) const;

		void SaveSessionData();
		void LoadSessionData();

		void Draw();	

	private:
		Map m_map;
		Player* m_player;
	};
}