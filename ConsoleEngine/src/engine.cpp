#include "engine.h"
#include "gameSession.h"
#include "windows.h"

#include <iostream>
#include <assert.h>
#include <array>

// temp
using Uint32 = unsigned int;

struct MemoryAlloc
{
	MemoryAlloc() : m_address( 0 ), m_size( 0 ) {}
	MemoryAlloc( Uint32 address, Uint32 size ) : m_address( address ), m_size( size ) {}

	Uint32 m_address;
	Uint32 m_size;
};

const Uint32 ARRAY_SIZE = 1000;
std::array< MemoryAlloc, ARRAY_SIZE > memoryRecords;
static Uint32 totalBytesAllocated = 0;

void* operator new( size_t size )
{
	void* address = malloc( size );
	assert( address );

	for( int i = 0; i < ARRAY_SIZE; ++i )
	{
		if( memoryRecords[ i ].m_address == 0 )
		{
			memoryRecords[ i ] = MemoryAlloc( (Uint32)address, size );
			totalBytesAllocated += size;
			break;
		}
	}
	
	return address;
}

void operator delete( void* address )
{
	for( int i = 0; i < ARRAY_SIZE; ++i )
	{
		if( memoryRecords[ i ].m_address == (Uint32)address )
		{
			totalBytesAllocated -= memoryRecords[ i ].m_size;
			memoryRecords[ i ] = MemoryAlloc();
			break;
		}
	}
	
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
		std::cout << "Allocated bytes: " << totalBytesAllocated << "\n";

		/*for( int i = 0; i < ARRAY_SIZE; ++i )
		{
			if( memoryRecords[ i ].m_address != 0 )
			{
				std::cout << "Allocation: Address: " << memoryRecords[ i ].m_address << " Size: " << memoryRecords[ i ].m_size << "\n";
			}
		}*/

		std::cout << "\n";
	}
}