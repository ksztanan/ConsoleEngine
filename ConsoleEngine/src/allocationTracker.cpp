#include "allocationTracker.h"

#include <iostream>
#include <assert.h>

AllocationTracker::AllocationTracker()
	: m_totalBytesAllocated( 0 )
	, m_totalAllocations( 0 )
	, m_allocatedMemoryPct( 0.f )
	, m_showDetailedDebug( false )
{}

void AllocationTracker::Allocate( void* address, const Uint32 size )
{
	if( !address )
	{
		return;
	}

	for( int i = 0; i < ALLOCATIONS_CAP; ++i )
	{
		if( m_allocations[ i ].m_address == 0 )
		{
			assert( ( ( m_totalBytesAllocated + size ) / 1024 ) < MEMORY_KILOBYTES );

			m_allocations[ i ] = Allocation( address, size );
			m_totalBytesAllocated += size;
			++m_totalAllocations;
			m_allocatedMemoryPct = 100.f * (float)( m_totalBytesAllocated / 1024 ) / MEMORY_KILOBYTES;
			return;
		}
	}

	assert( false );
}

void AllocationTracker::Deallocate( void* address )
{
	if( !address )
	{
		return;
	}

	for( int i = 0; i < ALLOCATIONS_CAP; ++i )
	{
		if( m_allocations[ i ].m_address == address )
		{
			m_totalBytesAllocated -= m_allocations[ i ].m_size;
			--m_totalAllocations;
			m_allocatedMemoryPct = 100.f * (float)( m_totalBytesAllocated / 1024 ) / MEMORY_KILOBYTES;
			m_allocations[ i ] = Allocation();
			return;
		}
	}

	assert( false );
}

void AllocationTracker::DrawDebug() const
{
	const Uint32 allocatedKilobytes = (Uint32)m_totalBytesAllocated / 1024;
	const Uint32 remainingBytes = m_totalBytesAllocated % 1024;

	std::cout << "AllocationTracker Debug:\n";
	std::cout << "--------------------------------\n";
	std::cout << "Allocated memory: " << allocatedKilobytes <<"[kB] " << remainingBytes << "[B] - " << m_allocatedMemoryPct<< "% of " << MEMORY_KILOBYTES << "[kB]\n";
	std::cout << "Total allocations: " << m_totalAllocations << "\n";
	std::cout << "\n";

	if( m_showDetailedDebug )
	{
		std::cout << "Detailed allocations:\n\n";
		for( int i = 0; i < ALLOCATIONS_CAP; ++i )
		{
			if( m_allocations[ i ].m_address )
			{
				std::cout << "Address: " << m_allocations[ i ].m_address << " Size: " << m_allocations[ i ].m_size << "\n";
			}
		}

		std::cout << "\n";
	}
}

AllocationTracker::Allocation::Allocation()
	: m_address( nullptr )
	, m_size( 0 )
{}

AllocationTracker::Allocation::Allocation( void* address, Uint32 size )
	: m_address( address )
	, m_size( size )
{}
