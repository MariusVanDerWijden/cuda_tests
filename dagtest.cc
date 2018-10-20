#include <iostream>

#include <cuda_runtime.h>

#include "dagtest.h"
#include "common.h"
#include "dagtestkernel.h"

void print_cuda_devices()
{
	int devCount;
	cudaGetDeviceCount(&devCount);
	printf("Found %d usable CUDA devices\n",devCount);
	for(int i = 0; i < devCount; i++)
	{
		cudaDeviceProp props;
		cudaGetDeviceProperties(&props, i);
		printf("%s\n",props.name);
		printf("\tCompute version: %d.%d \n",props.major,props.minor);
		printf("\tcudaDeviceProp::multiProcessorCount %d\n", props.multiProcessorCount);
		printf("\tcudaDeviceProp::clockRate: %f\n", (float)props.clockRate /1000000);
		printf("\tcudaDeviceProp::L2cacheSize: %d\n",props.l2CacheSize/1024);
	}
}

void perf_test()
{
	cudaSetDevice(0);
	cudaDeviceReset();
	
	cudaEvent_t evStart , evStop;
	float elapsedTime_ms;
	cudaEventCreate(&evStart);
	cudaEventCreate(&evStop);
	cudaEventRecord(evStart,0);
	perf_test_gpu();
	cudaEventRecord (evStop , 0);
	cudaEventSynchronize ( evStop );
	cudaEventElapsedTime (&elapsedTime_ms , evStart , evStop );
	printf("CUDA processing took: %f ms\n", elapsedTime_ms );
	cudaEventDestroy ( evStart );
	cudaEventDestroy ( evStop );
}