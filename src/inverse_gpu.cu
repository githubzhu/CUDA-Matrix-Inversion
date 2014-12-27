#include<cstdio>

extern "C" {
__global__
void init(int* B, int n){
    int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
    int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
   
}

//P := dev_to_process
__global__
void swap(int* P, int n, int row_id, int row_to_swap){
    int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
   
}

__global__
void fixRow(int* P, int* A, int n, int row_id, Value inv){
    int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
   
}

__global__
void fixColumn(int* P, int* A, int n, int column_id, Value inv){
    int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
    int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
   
}
}
