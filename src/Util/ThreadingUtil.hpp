#pragma once 

#include "Util/Common.h"

namespace KikooRenderer 
{


namespace Util 
{
	
	
/**
 A class for running a function concurrently on mltiple threads in a SIMD manner across a large set of data
 */
 
class ThreadPool
{
	protected:
		static uint64_t threadNumber; 
		static ctpl::thread_pool threadPool;
		
		std::vector< std::future<void> > futures;
		
		
	public:
		
		/**
		 Gets the number of hardware concurrent threads on the machine
		 @return the number of hardware concurrent threads on the machine
		 */
		 
		static uint64_t GetThreadNumber();
		
		
		/**
		 Constructor
		 @param func The function to run in SIMD parralel way, the first parameter is the id of the SIMD data, and the second is the thread index, the constructor starts the process.
		 */
		 
		ThreadPool(std::function<void(uint64_t, uint64_t)> func, uint64_t size);
		
		
		/**
		 Constructor
		 @param func The function to run in SIMD parralel way, the parameter is the id of the SIMD data, the constructor starts the process.
		 */
		 
		ThreadPool(std::function<void(uint64_t)> func, uint64_t size);
		
		
		/**
		 This blocks execution until the SIMD process has completed. 
		 */
		 
		void Block();
}; 
	
	
}


} // end namespace ScanLABLibrary
