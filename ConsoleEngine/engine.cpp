#include "engine.h"
#include "gameSession.h"
#include "windows.h"
#include <iostream>
#include <assert.h>

// temp
extern unsigned int bytesAllocated = 0;

void* operator new( size_t size )
{
	void* address = malloc( size );
	assert( address );
	bytesAllocated += size;

	return address;
}

void operator delete( void* address )
{
	// todo - how to count freed mem?
	free( address );
}
// temp end

namespace engine
{
	Engine& Engine::Get()
	{
		static Engine engine;

		return engine;
	}

	Engine::Engine()
		: m_gameSession( nullptr )
	{}

	Engine::~Engine()
	{
		if( m_gameSession )
		{
			delete m_gameSession;
		}
	}

	void Engine::Run()
	{
		CreateGameSession();

		while( true )
		{		
			system( "cls" );
			HandleGameSession();
			DrawDebug();
			m_inputManager.Update();
		}

		DestroyGameSession();
	}

	InputManager& Engine::GetInputManager()
	{
		return m_inputManager;
	}

	void Engine::HandleGameSession()
	{
		if( m_gameSession )
		{
			m_gameSession->Update();
		}
	}

	void Engine::CreateGameSession()
	{
		if( !m_gameSession )
		{
			m_gameSession = new GameSession;
		}

		return;
	}

	void Engine::DestroyGameSession()
	{
		if( m_gameSession )
		{
			delete m_gameSession;
			m_gameSession = nullptr;
		}
	}

	void Engine::DrawDebug() const
	{
		std::cout << "Engine Debug:\n";
		std::cout << "--------------------------------\n";
		std::cout << "Allocated bytes: " << bytesAllocated << "\n";
		std::cout << "\n";
	}
}