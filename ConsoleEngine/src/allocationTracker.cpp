#include "allocationTracker.h"

#include <iostream>
#include <assert.h>

//#define DETAILED_DEBUG

AllocationTracker::AllocationTracker()
	: m_totalBytesAllocated( 0 )
	, m_totalAllocations( 0 )
	, m_frameBytesAllocated( 0 )
	, m_frameBytesFreed( 0 )
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
			m_frameBytesAllocated += size;
			++m_totalAllocations;
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
			m_frameBytesFreed += m_allocations[ i ].m_size;
			--m_totalAllocations;
			m_allocations[ i ] = Allocation();
			return;
		}
	}

	assert( false );
}

void AllocationTracker::ResetFrame()
{
	m_frameBytesAllocated = 0;
	m_frameBytesFreed = 0;
}

void AllocationTracker::DrawDebug() const
{
	const Uint32 allocatedKilobytes = m_totalBytesAllocated / 1024;
	const Uint32 allocatedBytesRemainder = m_totalBytesAllocated % 1024;
	const Uint32 frameAllocatedKilobytes = m_frameBytesAllocated / 1024;
	const Uint32 frameAllocatedBytesRemainder = m_frameBytesAllocated % 1024;
	const Uint32 frameFreedKilobytes = m_frameBytesFreed / 1024;
	const Uint32 frameFreedBytesRemainder = m_frameBytesFreed % 1024;
	const Uint32 freeBytes = ( MEMORY_KILOBYTES * 1024 ) - m_totalBytesAllocated;
	const Uint32 freeKilobytes = freeBytes / 1024;
	const Uint32 freeBytesRemainder = freeBytes % 1024;
		
	std::cout << "AllocationTracker Debug:\n";
	std::cout << "--------------------------------------\n";
	std::cout << "MEMORY"				<< "\t\t\t"		<< "[kB]"					<< "\t" << "[B]"						<< "\n";
	std::cout << "Allocated:"			<< "\t\t"		<< allocatedKilobytes		<< "\t" << allocatedBytesRemainder		<< "\n";
	std::cout << "Allocated (frame):"	<< "\t"			<< frameAllocatedKilobytes	<< "\t" << frameAllocatedBytesRemainder << "\n";
	std::cout << "Freed (frame):"		<< "\t\t"		<< frameFreedKilobytes		<< "\t" << frameFreedBytesRemainder		<< "\n";
	std::cout << "Free:"				<< "\t\t\t"		<< freeKilobytes			<< "\t" << freeBytesRemainder			<< "\n";	
	std::cout << "\n";
	std::cout << "ALLOCATIONS"			<< "\t\t\t"											<< "[-]"						<< "\n";
	std::cout << "Count:"				<< "\t\t\t\t"										<< m_totalAllocations			<< "\n";
	std::cout << "\n";

#ifdef DETAILED_DEBUG
	std::cout << "Detailed allocations:\n\n";
	for( int i = 0; i < ALLOCATIONS_CAP; ++i )
	{
		if( m_allocations[ i ].m_address )
		{
			std::cout << "Address: " << m_allocations[ i ].m_address << " Size: " << m_allocations[ i ].m_size << "\n";
		}
	}

	std::cout << "\n";
#endif // DETAILED_DEBUG
}

AllocationTracker::Allocation::Allocation()
	: m_address( nullptr )
	, m_size( 0 )
{}

AllocationTracker::Allocation::Allocation( void* address, Uint32 size )
	: m_address( address )
	, m_size( size )
{}
