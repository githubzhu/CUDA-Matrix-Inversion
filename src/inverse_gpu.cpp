#include <bits/stdc++.h>

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

bool inverse_gpu(int *A, int *B, int n){
	
	cuInit(0);
    
    CUdevice cuDevice;
    CUresult res = cuDeviceGet(&cuDevice, 0);

    CUcontext cuContext;
    res = cuCtxCreate(&cuContext, 0, cuDevice);

    CUmodule cuModule = (CUmodule)0;
    res = cuModuleLoad(&cuModule, "inverse_gpu.ptx");

    CUfunction init, swap, fixRow, fixColumn;
	res = cuModuleGetFunction(&init, cuModule, "init");
	res = cuModuleGetFunction(&swap, cuModule, "swap");
    res = cuModuleGetFunction(&fixRow, cuModule, "fixRow");
	res = cuModuleGetFunction(&fixColumn, cuModule, "fixColumn");
	
	Value inv;
 	int row_id, column_id, row_to_swap;
 	CUdeviceptr devA, devB, dev_to_process;
 	void* argsInit[] = {&devB, &n};
 	void* argsSwap[] = {&dev_to_process, &n, &row_id, &row_to_swap};
 	void* argsRow[] = {&dev_to_process, &devA, &n, &row_id, &inv};
	void* argsColumn[] = {&dev_to_process, &devA, &n, &column_id, &inv};
	
	int tX_oneD = 1024;
	int bX_oneD = (n + tX_oneD-1)/tX_oneD;
	
	int tX_twoD = 32, tY_twoD = 32;
	int bX_twoD = (n + tX_twoD-1)/tX_twoD;
	int bY_twoD = (n + tY_twoD-1)/tY_twoD;
			
    cuMemHostRegister((void *) A, n*n*sizeof(Value), CU_MEMHOSTREGISTER_PORTABLE);
    cuMemAlloc(&devA, n*n*sizeof(Value));
    cuMemcpyHtoD(&devA, A, n*n*sizeof(Value)); 

	cuMemAlloc(&devB, n*n*sizeof(Value));
    res = cuLaunchKernel(init, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, argsInit, 0);
    cuCtxSynchronize(); 

	FWD(i,0,n){

		row_id = i;

		// finding row with non-zero value on i-th column
		cuMemcpyDtoH(A+i*n+i, devA+i*n+i, sizeof(Value));
		if(A[i*n+i] == Value(0)){
			int k = -1;
			FWD(j,i+1,n)
				if(A[j*n+i]){
					k = j;
					break;
				}
			if(k == -1) return 0;
			row_to_swap = k;
			
			dev_to_process = devA;
    		res = cuLaunchKernel(swap, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, argsSwap, 0);
    		cuCtxSynchronize(); 
		
			dev_to_process = devB;
    		res = cuLaunchKernel(swap, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, argsSwap, 0);
    		cuCtxSynchronize(); 
		}

		cuMemcpyDtoH(A+i*n+i, devA+i*n+i, sizeof(Value));		
		inv = inverse(A[i*n+i], get_modulus());
		
		if(A[i*n+i] != Value(1)) {
			
			dev_to_process = devA;
    		res = cuLaunchKernel(fixRow, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, argsRow, 0);
    		cuCtxSynchronize(); 
		
			dev_to_process = devB;
    		res = cuLaunchKernel(fixRow, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, argsRow, 0);
    		cuCtxSynchronize(); 
		}		
		// reducing all the other elements of the i-th column to zero
		column_id = i;
			
		dev_to_process = devA;
    	res = cuLaunchKernel(fixColumn, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, argsColumn, 0);
    	cuCtxSynchronize(); 
		
		dev_to_process = devB;
    	res = cuLaunchKernel(fixColumn, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, argsColumn, 0);
    	cuCtxSynchronize(); 

	}

    return 1;
}
