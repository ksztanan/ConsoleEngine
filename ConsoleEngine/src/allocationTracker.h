#pragma once

#include <array>

class AllocationTracker
{
	using Uint32 = unsigned int;
	static constexpr Uint32 ALLOCATIONS_CAP = 1000;

public:
	AllocationTracker();

	void Allocate( void* address, const Uint32 size );
	void Deallocate( void* address );
	void DrawDebug() const;

private:
	struct Allocation
	{
		Allocation();
		Allocation( void* address, Uint32 size );
		
		void* m_address;
		Uint32 m_size;
	};

	std::array< Allocation, ALLOCATIONS_CAP > m_allocations;
	Uint32 m_totalBytesAllocated;
	bool m_showDetailedDebug;
};