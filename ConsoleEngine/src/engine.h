#pragma once

#include "inputManager.h"

extern unsigned int bytesAllocated;

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

	private:
		Engine();
		Engine( const Engine& ) {}
		void operator=( const Engine& ) {}

		void HandleGameSession();
		void CreateGameSession();
		void DestroyGameSession();

		void DrawDebug() const;

	private:
		InputManager m_inputManager;
		GameSession* m_gameSession;
	};
}