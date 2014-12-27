#include <bits/stdc++.h>
#include "field.hpp"
#include "inverse_cpu.hpp"

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

using namespace std;

int main(){
	int mod;
	scanf("%d", &mod);
	field_init(mod);
	int n;
	assert(scanf("%d", &n) == 1);

	field_element* A = new field_element[n*n];
	field_element* B = new field_element[n*n];

	FWD(i,0,n)
		FWD(j,0,n){
			int a;
			scanf("%d", &a);
			A[i*n+j] = a;
		}

	int res = inverse_cpu(A, B, n);

	printf("%d\n", res);
	if(res)
		FWD(i,0,n)
			FWD(j,0,n)
				printf("%d%c", B[i*n+j].get_value(), " \n"[j==n-1]);

	delete[] A;
	delete[] B;

	return 0;
}
