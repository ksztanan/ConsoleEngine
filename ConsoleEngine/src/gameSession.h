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

	private:
		void HandleInteractions();
		void HandleMovement();
		bool IsMovementPossible( Dir dir ) const;
		bool IsWithinLocalBounds( const Vector2& pos ) const;
		bool ShouldMoveLocalMap( Dir dir ) const;

		void OnSave();
		void OnLoad();

		void Draw();
		void DrawDebug() const;

	private:
		Map m_map;
		Player* m_player;
	};
}