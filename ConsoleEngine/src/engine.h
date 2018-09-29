#pragma once

#include "inputManager.h"
#include "saveSystem.h"
#include "allocationTracker.h"

namespace engine
{
	class GameSession;

	class Engine
	{
	public:
		static Engine& Get();
		~Engine();

		void Run();

		InputManager& GetInputManager();
		SaveSystem& GetSaveSystem();

	private:
		Engine();
		Engine( const Engine& ) {}
		void operator=( const Engine& ) {}

		void UpdateGameSession();
		void CreateGameSession();
		void DestroyGameSession();

		void DrawDebug() const;

	private:
		InputManager m_inputManager;
		SaveSystem m_saveSystem;
		GameSession* m_gameSession;
	};
}