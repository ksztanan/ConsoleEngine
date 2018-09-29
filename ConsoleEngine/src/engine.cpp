#include "engine.h"
#include "gameSession.h"
#include "windows.h"

#include <iostream>
#include <assert.h>

AllocationTracker allocationTracker;

void* operator new( size_t size )
{
	void* address = malloc( size );
	assert( address );
	allocationTracker.Allocate( address, size );

	return address;
}

void operator delete( void* address )
{
	assert( address );
	allocationTracker.Deallocate( address );
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
		allocationTracker.Allocate( this, sizeof( Engine ) );
	}

	Engine::~Engine()
	{
		if( m_gameSession )
		{
			delete m_gameSession;
		}

		allocationTracker.Deallocate( this );
	}

	void Engine::Run()
	{
		bool mainLoop = true;
		while( mainLoop )
		{
			CreateGameSession();

			while( true )
			{
				system( "cls" );
				UpdateGameSession();
				DrawDebug();

				m_inputManager.Update();
				if( m_inputManager.IsActionActive( InputAction::RestartGame ) )
				{
					break;
				}
				else if( m_inputManager.IsActionActive( InputAction::QuitGame ) )
				{
					mainLoop = false;
					break;
				}
				allocationTracker.ResetFrame();
			}

			DestroyGameSession();
		}	
	}

	InputManager& Engine::GetInputManager()
	{
		return m_inputManager;
	}

	SaveSystem& Engine::GetSaveSystem()
	{
		return m_saveSystem;
	}

	void Engine::UpdateGameSession()
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
		if( m_gameSession )
		{
			m_gameSession->DrawDebug();
		}

		allocationTracker.DrawDebug();
		m_saveSystem.DrawDebug();
	}
}