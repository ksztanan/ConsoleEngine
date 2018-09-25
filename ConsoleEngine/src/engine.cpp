#include "engine.h"
#include "gameSession.h"
#include "windows.h"

#include <iostream>
#include <assert.h>

void* operator new( size_t size )
{
	void* address = malloc( size );
	assert( address );
	engine::Engine::Get().GetAllocationTracker().Allocate( address, size );

	return address;
}

void operator delete( void* address )
{
	assert( address );
	engine::Engine::Get().GetAllocationTracker().Deallocate( address );
	free( address );
}

namespace engine
{
	Engine& Engine::Get()
	{
		static Engine engine;

		return engine;
	}

	Engine::Engine()
		: m_gameSession( nullptr )
	{
		m_allocationTracker.Allocate( this, sizeof( Engine ) - sizeof( AllocationTracker ) );
	}

	Engine::~Engine()
	{
		if( m_gameSession )
		{
			delete m_gameSession;
		}

		m_allocationTracker.Deallocate( this );
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
			m_allocationTracker.ResetFrame();
		}

		DestroyGameSession();
	}

	InputManager& Engine::GetInputManager()
	{
		return m_inputManager;
	}

	AllocationTracker& Engine::GetAllocationTracker()
	{
		return m_allocationTracker;
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
		m_allocationTracker.DrawDebug();
	}
}