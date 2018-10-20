#pragma once

typedef struct
{
    uint4 uint4s[128 / sizeof(uint4)];
} hash128_t;

typedef struct
{
    uint4 uint4s[32 / sizeof(uint4)];
} hash32_t;

__constant__ uint32_t d_dag_size;
__constant__ hash128_t* d_dag;
__constant__ hash32_t d_header;

void perf_test_gpu();
