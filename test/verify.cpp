#include <bits/stdc++.h>
#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

using namespace std;

bool check(int *A, int *B, int n){
	int *C = new int[n*n];
	FWD(i,0,n*n) C[i] = 0;
	FWD(i,0,n)
		FWD(k,0,n)
			FWD(j,0,n)
				C[i*n+j] ^= A[i*n+k] * B[k*n+j];
	bool correct = 1;
	FWD(i,0,n)
		FWD(j,0,n)
			if(C[i*n+j] != (i == j))
				correct = 0;
	delete[] C;
	return correct;
}

int main(){
	int status = 0;

	int n;
	assert(scanf("%d", &n) == 1);

	int *in = new int[n*n];
	FWD(i,0,n)
		FWD(j,0,n)
			assert(scanf("%d", &in[i*n+j]) == 1);

	int cpu_res;
	int *cpu = new int[n*n];
	assert(scanf("%d", &cpu_res) == 1);
	if(cpu_res)
		FWD(i,0,n)
			FWD(j,0,n)
				assert(scanf("%d", &cpu[i*n+j]) == 1);

	int gpu_res;
	int *gpu = new int[n*n];
	assert(scanf("%d", &gpu_res) == 1);
	if(gpu_res)
		FWD(i,0,n)
			FWD(j,0,n)
				assert(scanf("%d", &gpu[i*n+j]) == 1);

	if(cpu_res != gpu_res){
		if(cpu_res){
			fprintf(stderr, "CPU found inversion, GPU has not.\n");
		}else{
			fprintf(stderr, "GPU found inversion, CPU has not.\n");
		}
		status = 1;
	}

	if(cpu_res && !check(in, cpu, n)){
		fprintf(stderr, "CPU inversion is not correct.\n");
		status = 1;
	}

	if(gpu_res && !check(in, gpu, n)){
		fprintf(stderr, "GPU inversion is not correct.\n");
		status = 1;
	}

	delete[] in;
	delete[] cpu;
	delete[] gpu;

	if(status){
		fprintf(stderr, "Problems detected\n");
	}

	return status;
}