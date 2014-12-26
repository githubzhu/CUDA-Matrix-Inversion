#include <bits/stdc++.h>

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

bool inverse_cpu(int *A, int *B, int n){
	
	cuInit(0);
    
    CUdevice cuDevice;
    CUresult res = cuDeviceGet(&cuDevice, 0);

    CUcontext cuContext;
    res = cuCtxCreate(&cuContext, 0, cuDevice);

    CUmodule cuModule = (CUmodule)0;
    res = cuModuleLoad(&cuModule, "inverse_gpu.ptx");

    CUfunction init, fixRow, fixColumn;
	res = cuModuleGetFunction(&init, cuModule, "init");
    res = cuModuleGetFunction(&fixRow, cuModule, "fixRow");
	res = cuModuleGetFunction(&fixColumn, cuModule, "fixColumn");
	
 	int row_id, column_id;
 	CUdeviceptr devA, devB, dev_to_process;
 	void* argsInit[] = {&devB, &n};
 	void* argsRow[] = {&dev_to_process, &devA, &n, &row_id};
	void* argsColumn[] = {&dev_to_process, &devA, &n, &column_id};
	
	int tX_oneD = 1024;
	int bX_oneD = (n + tX_oneD-1)/tX_oneD;
	
	int tX_twoD = 32, tY_twoD = 32;
	int bX_twoD = (n + tX_twoD-1)/tX_twoD;
	int bY_twoD = (n + tY_twoD-1)/tY_twoD;
			
    cuMemHostRegister((void *) A, n*4, CU_MEMHOSTREGISTER_PORTABLE);
    cuMemAlloc(&devA, n*4);
    cuMemcpyHtoD(&devA, A, n*4); 

	cuMemAlloc(&devB, n*4);
    res = cuLaunchKernel(init, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, argsInit, 0);
    cuCtxSynchronize(); 
	/*	
	FWD(i,0,n)
		FWD(j,0,n)
			B[i*n+j] = (i == j);
	*/	

	FWD(i,0,n){

		// finding row with 1 on i-th column
		if(A[i*n+i] == Value(0)){
			int k = -1;
			FWD(j,i+1,n)
				if(A[j*n+i]){
					k = j;
					break;
				}
			if(k == -1) return 0;
			FWD(j,0,n) std::swap(A[i*n+j], A[k*n+j]);
			FWD(j,0,n) std::swap(B[i*n+j], B[k*n+j]);
		}
		if(A[i*n+i] != Value(1)) {
			row_id = i;
			
			dev_to_process = devA;
    		res = cuLaunchKernel(fixRow, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, argsRow, 0);
    		cuCtxSynchronize(); 
		
			dev_to_process = devB;
    		res = cuLaunchKernel(fixRow, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, argsRow, 0);
    		cuCtxSynchronize(); 
		}		
		// removing 1's in this column from other rows
		column_id = i;
			
		dev_to_process = devA;
    	res = cuLaunchKernel(fixColumn, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, argsColumn, 0);
    	cuCtxSynchronize(); 
		
		dev_to_process = devB;
    	res = cuLaunchKernel(fixRow, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, argsColumn, 0);
    	cuCtxSynchronize(); 
		/*
		FWD(k,0,n)
			if(i != k && A[k*n+i]){
				FWD(j,0,n) A[k*n+j] ^= A[i*n+j];
				FWD(j,0,n) B[k*n+j] ^= B[i*n+j];	
			}
		*/
	}

    return 1;
}
