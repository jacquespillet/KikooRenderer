#include "ThreadingUtil.hpp"


namespace KikooRenderer 
{


namespace Util 
{
	
	
uint64_t ThreadPool::threadNumber = std::thread::hardware_concurrency(); 
ctpl::thread_pool ThreadPool::threadPool(threadNumber);
		
		
uint64_t ThreadPool::GetThreadNumber()
{
	return threadNumber; 
}

ThreadPool::ThreadPool(std::function<void(uint64_t, uint64_t)> func, uint64_t problemSize)
{
	uint64_t itemsPerThread = problemSize / threadNumber;
	uint64_t remainder = problemSize % threadNumber;
	std::vector<uint64_t> threadSize(threadNumber); 
	std::vector<uint64_t> threadOffset(threadNumber, 0); 
	for(uint64_t i = 0; i < threadNumber; i++)
		threadSize[i] = itemsPerThread;
	for(uint64_t i = 0; i < remainder; i++)
		threadSize[i]+=1;
	for(uint64_t i = 1; i < threadNumber; i++)
		threadOffset[i] = threadSize[i - 1] + threadOffset[i - 1];
	futures.resize(threadNumber); 
	
	for(uint64_t t = 0; t < threadNumber; t++)
	{
		uint64_t offset = threadOffset[t];
		uint64_t size = threadSize[t];
		futures[t] = threadPool.push([&, this, func](int id, uint64_t offset, uint64_t size, uint64_t threadInx)
		{
			for(uint64_t i = offset; i < offset + size; i++)
			{
				func(i, threadInx);
			}
		}, offset, size, t);	
	}
}

ThreadPool::ThreadPool(std::function<void(uint64_t)> func, uint64_t problemSize)
{
	//If problem size is 100, and num threads is 12
	uint64_t itemsPerThread = problemSize / threadNumber; //100/12 = 8
	uint64_t remainder = problemSize % threadNumber; //100%12=4
	std::vector<uint64_t> threadSize(threadNumber); //vector of size 12 : in each cell, number of elements per thread
	std::vector<uint64_t> threadOffset(threadNumber, 0); //vector of size 12 filled with 0 : offsets for each thread
	for(uint64_t i = 0; i < threadNumber; i++) //Each thread will have to compute 8 elements
		threadSize[i] = itemsPerThread;
	for(uint64_t i = 0; i < remainder; i++) //the first 4 threads will take 1 more element
		threadSize[i]+=1;
	for(uint64_t i = 1; i < threadNumber; i++) // Compute the offset of each thread to know the index of their first element
		threadOffset[i] = threadSize[i - 1] + threadOffset[i - 1];
	futures.resize(threadNumber); //resize the pool to the size of threads
	
	for(uint64_t t = 0; t < threadNumber; t++) //For each thread
	{
		uint64_t offset = threadOffset[t]; //offset to compute : first element in the array
		uint64_t size = threadSize[t]; //size of the chunk to compute
		futures[t] = threadPool.push([&, this, func](int id, uint64_t offset, uint64_t size) //add a thread to the threadpool
		{
			for(uint64_t i = offset; i < offset + size; i++) //This code is executed by each thread in the pool : from offset to offset + size, execute the func
			{
				func(i);
			}
		}, offset, size);	
	}
}

void ThreadPool::Block()
{
	for(uint64_t t = 0; t < threadNumber; t++) futures[t].wait();
}

//USAGE 
// KikooRenderer::Util::ThreadPool( std::function<void(uint64_t)>([this, &pointsObject, &pointCloud, &pointOffset](uint64_t i)
// {
// 	for(int j = 0; j < pointCloud[i].points.size(); j++)
// 	{
// 		uint64_t inx = pointOffset[i] + j; 
// 		pointsObject->points.points[inx].x = pointCloud[i].points[j].x;
// 		pointsObject->points.points[inx].y = pointCloud[i].points[j].y;
// 		pointsObject->points.points[inx].z = pointCloud[i].points[j].z;
// 		pointsObject->points.points[inx].r = pointCloud[i].points[j].r;
// 		pointsObject->points.points[inx].g = pointCloud[i].points[j].g;
// 		pointsObject->points.points[inx].b = pointCloud[i].points[j].b;
// 		pointsObject->points.points[inx].i = pointCloud[i].points[j].i;
// 	}
// }), pointCloud.size()).Block(); 

//OR

// KikooRenderer::Util::ThreadPool( std::function<void(uint64_t, uint64_t)>([this, &sum, &avVec](uint64_t i, uint64_t t)
// {
// 	avVec[t] += glm::dvec3(points.points[i].x, points.points[i].y, points.points[i].z);
// 	sum[t]++;
// }), points.points.size() ).Block();


}


} // end namespace ScanLABLibrary
