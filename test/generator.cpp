#include <bits/stdc++.h>

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

using namespace std;

int main(){
	char type[12];
	assert(scanf("%s", type));

	if(strcmp(type, "random") == 0){
		int seed;
		assert(scanf("%d", &seed) == 1);
		
		int mod;
		assert(scanf("%d", &mod) == 1);

		int n;
		assert(scanf("%d", &n) == 1);

		mt19937 eng(seed);
		uniform_int_distribution<> dist(0, mod-1);

		printf("%d\n%d\n", mod, n);
		FWD(i,0,n)
			FWD(j,0,n)
				printf("%d%c", dist(eng), " \n"[j==n-1]);
	}else if(strcmp(type, "direct") == 0){
		int mod;
		assert(scanf("%d", &mod) == 1);

		int n;
		assert(scanf("%d", &n) == 1);

		printf("%d\n%d\n", mod, n);
		FWD(i,0,n)
			FWD(j,0,n){
				int a;
				scanf("%d", &a);
				printf("%d%c", a, " \n"[j==n-1]);
			}
	}else{
		fprintf(stderr, "Test type %s has not been implemented yet.\n", type);
		return 1;
	}

	return 0;
}