#include <bits/stdc++.h>
#include "inverse_gpu.hpp"

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

using namespace std;

int main(){
	int n;
	assert(scanf("%d", &n) == 1);

	int *A = new int[n*n];
	int *B = new int[n*n];

	FWD(i,0,n)
		FWD(j,0,n)
			scanf("%d", &A[i*n+j]);

	int res = inverse_gpu(A, B, n);

	printf("%d\n", res);
	if(res)
		FWD(i,0,n)
			FWD(j,0,n)
				printf("%d%c", B[i*n+j], " \n"[j==n-1]);

	delete[] A;
	delete[] B;

	return 0;
}
