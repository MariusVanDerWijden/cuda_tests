#include <cuda_runtime.h>

#include <stdio.h>
#include "common.h"
#include "dagtestkernel.h"
#include "keccak.cuh"


__global__ void kernel()
{
	uint64_t nonce = 0;
	uint2 state[12];
	//vectorize nonce
    asm("mov.b64 {%0,%1},%2; \n\t" : "=r"(state[4].x), "=r"(state[4].y) : "l"(nonce));
    //execute keccak
	keccak_f1600_init(state);
	keccak_f1600_final(state);
	//dag access
	//d_dag[0].uint4s[0];
}

__global__ void genDag()
{
	//genDag
	for(int i = 0; i < d_dag_size; i++)
		for(int q = 0; q < 4; q++)
			d_dag[i].uint4s[q] = vectorize4(i*q, q*q);
	//genHeader
	for(int i = 0; i < 32 / sizeof(uint4); i++)
    	d_header.uint4s[i] = vectorize4(0, i);
}

void set_constants(
	hash128_t* _dag,
	uint32_t _dag_size,
	hash32_t _header
	)
{
	cudaMemcpyToSymbol(d_dag, &_dag, sizeof(hash128_t*));
	cudaMemcpyToSymbol(d_dag_size, &_dag_size, sizeof(uint32_t));
	cudaMemcpyToSymbol(d_header, &_header, sizeof(hash32_t));
}


void perf_test_gpu()
{	
	uint32_t dagSize = 128*1024*1024;
    hash128_t* dag;
    cudaMalloc(reinterpret_cast<void**>(&dag), dagSize);
    hash32_t header;
    cudaMalloc(reinterpret_cast<void**>(&header), dagSize);
    set_constants(dag,dagSize,header);
	genDag <<<1,1>>> ();
	cudaDeviceSynchronize();
	kernel <<<1,1>>> ();
	cudaThreadSynchronize();
}
