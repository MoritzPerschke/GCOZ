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
	
	using DurationVector_Map = Map<int, Vector<SharedDuration>>; // Method Durations Map (int -> Vector<Duration>)
	using ThreadIdVector_Map = Map<int, Vector<idHash>>; //Map for ThreadIDs (int -> Vector<idHash>)
	using ResultsMap_Map     = Map<int, Map<int, Vector<SharedDuration>>>; // Results Map (int -> float -> Vector<Duration>) // does not work because allocator tries to alloc pair <int, Map<>> for pair <float, vec<llong>>

} // namespace IPC

	//template <typename K, typename V, typename Cmp = std::less<K>, typename PP = std::pair<K const, Map<KK const, V>>>
	//	typedef pair <const float, Duration_Vector>										DelayMap_Type;
	//	typedef scoped_allocator_adaptor <allocator<DelayMap_Type, segMan>>				DelayMapValue_Allocator;
	//	typedef map<float, Duration_Vector, std::less<float>, DelayMapValue_Allocator>	DelayResult_Map;
	
	//	typedef pair <const int, DelayResult_Map>	ResultsMap_Type;
	//	typedef scoped_allocator_adaptor <allocator<ResultsMap_Type, segMan>> ResultsMapValue_Allocator;
	//	/* This map is used to do method->delay->frametimes/rates */
	//	typedef map <int, DelayResult_Map, std::less<int>, ResultsMapValue_Allocator> Results_Map;


	//using testMap = Map<int, int>; //works
	//using IFDMap = Map<int, Map<float, Vector<int>>>;
	//using a = boost::container::multimap<int, int>;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		BASIC TYPES
//			- vector <int>
//			- vector <Duration>
//			- vector <threadID>
//			- map <int, Nanoseconds>
//			- map <int, vector<Duration>>
//			- map <float, vector<Duration>>
//			- map <int, vector<threadID>>
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	/* Vector of integers (nr. of method calls) */
//	typedef allocator <int, segMan>		 Int_Allocator;
//	typedef vector <int, Int_Allocator>	 Int_Vector;
//	
//	/* Vector of Durations (Frametimes, Durations of methods,...) */
//	typedef allocator <SharedDuration, segMan>			 Duration_Allocator;
//	typedef vector <SharedDuration, Duration_Allocator>	 Duration_Vector;
//
//	/* Vector of ThreadIDs */
//	typedef allocator <idHash, segMan>			IdHash_Allocator;
//	typedef vector <idHash, IdHash_Allocator>	IdHash_Vector;
//
//	/* Map of int -> Vector of Durations (MethodDurations,...) */
//	typedef pair  <const int, Duration_Vector>											DurationMap_Type;
//	typedef scoped_allocator_adaptor <allocator<DurationMap_Type, segMan>>				DurationMapValue_Allocator;
//	typedef map <int, Duration_Vector, std::less<int>, DurationMapValue_Allocator>		DurationVector_Map;
//
//	/* Map of float -> Vector of Durations */
//	typedef pair <const float, Duration_Vector>										DelayMap_Type;
//	typedef scoped_allocator_adaptor <allocator<DelayMap_Type, segMan>>				DelayMapValue_Allocator;
//	typedef map<float, Duration_Vector, std::less<float>, DelayMapValue_Allocator>	DelayResult_Map;
//
//	/* Map of int -> Vector of ThreadIDs */
//	typedef pair <const int, IdHash_Vector>											IdHashMap_Type;
//	typedef scoped_allocator_adaptor <allocator<IdHashMap_Type, segMan>>			IdHashMapValue_Allocator;
//	typedef map <int, IdHash_Vector, std::less<idHash>, IdHashMapValue_Allocator>	ThreadIdVector_Map;
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////		COMPLICATED TYPES
////			- map <int, map<float, vector<Duration>>>
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	/* Map of int -> float -> Vector of Durations (Profiling results, needs one per method) */ //this is a lot 
//	/* maps method -> delay -> results */
//	/* profilingResults->at(1)->at(.1)->push_back(); */
//
//	typedef pair <const int, DelayResult_Map>	ResultsMap_Type;
//	typedef scoped_allocator_adaptor <allocator<ResultsMap_Type, segMan>> ResultsMapValue_Allocator;
//	/* This map is used to do method->delay->frametimes/rates */
//	typedef map <int, DelayResult_Map, std::less<int>, ResultsMapValue_Allocator> Results_Map;
//

#endif //!_SHARED_MEMORY_BOOST_IMPLEMENTATION