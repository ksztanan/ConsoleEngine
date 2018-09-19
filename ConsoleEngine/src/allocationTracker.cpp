#include "allocationTracker.h"

#include <iostream>

AllocationTracker::AllocationTracker()
	: m_totalBytesAllocated( 0 )
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
			m_allocations[ i ] = Allocation( address, size );
			m_totalBytesAllocated += size;
			return;
		}
	}
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
			m_allocations[ i ] = Allocation();
			return;
		}
	}
}

void AllocationTracker::DrawDebug() const
{
	const Uint32 allocatedKilobytes = (Uint32)m_totalBytesAllocated / 1024;
	const Uint32 remainingBytes = m_totalBytesAllocated % 1024;

	std::cout << "AllocationTracker Debug:\n";
	std::cout << "--------------------------------\n";
	std::cout << "Allocated memory: " << allocatedKilobytes <<" kB, " << remainingBytes << " B" << "\n";
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
