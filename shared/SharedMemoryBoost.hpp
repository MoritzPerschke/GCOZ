#pragma once
#ifndef _SHARED_MEMORY_BOOST_IMPLEMENTATION
#define _SHARED_MEMORY_BOOST_IMPLEMENTATION

#include <Constants.h>
#include <scoped_allocator>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <boost/container/scoped_allocator.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <vector>
#include <set>

#define BOOST_DATE_TIME_NO_LIB 

using boost::interprocess::shared_memory_object;
using boost::interprocess::managed_shared_memory;
using boost::interprocess::create_only;
using boost::interprocess::open_only;
using boost::interprocess::named_mutex;
using boost::interprocess::scoped_lock;

namespace IPC {

	using std::pair;
	using std::scoped_allocator_adaptor;
	using namespace boost::interprocess;

	using segMan = managed_shared_memory::segment_manager;

/* ----------------[Stackoverflow answer based]-------------------- */
/* -------- https://stackoverflow.com/a/77467040/15005309 --------- */

	template <typename T> using Alloc = scoped_allocator_adaptor<allocator<T, segMan>>;
	template <typename T> using Vector = std::vector<T, Alloc<T>>;

	template <typename K, typename V, typename Cmp = std::less<K>, typename P = std::pair<K const, V>>
	using Map = boost::container::map<K, V, Cmp, Alloc<P>>;
	
	// Method Durations Map (int -> Vector<Duration>)
	using DurationVector_Map = Map<int, Vector<Nanoseconds>>;
	//Map for ThreadIDs (int -> Vector<idHash>)
	using ThreadIdVector_Map = Map<int, Vector<idHash>>;
	// Results Map (int -> float -> Vector<Duration>)
	using ResultsMap_Map     = Map<int, Map<int, Vector<Nanoseconds>>>;

} // namespace IPC
#endif //!_SHARED_MEMORY_BOOST_IMPLEMENTATION