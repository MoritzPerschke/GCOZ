#pragma once
#ifndef _SHARED_MEMORY_BOOST_IMPLEMENTATION
#define _SHARED_MEMORY_BOOST_IMPLEMENTATION

#include <Constants.h>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

namespace IPC {
	using namespace boost::interprocess;
	typedef managed_shared_memory::segment_manager segMan;
	typedef allocator<Duration, segMan> DurationAllocator;
	typedef vector<Duration, DurationAllocator> DurationVector;
	typedef std::pair<const Duration, DurationVector> DurationMapType;
	typedef allocator<DurationMapType, segMan> MapValueAllocator;
	typedef map<Duration, DurationVector, std::less<Duration>, MapValueAllocator> DurationMap;

} // namespace IPC

#endif //!_SHARED_MEMORY_BOOST_IMPLEMENTATION