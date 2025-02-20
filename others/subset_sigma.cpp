
// size k
int c=(1<<k)-1;
while(c<1<<n){
	// hoge
	if(c==0)break;
	int x=c&-c,y=c+x;
	c=(((c&~y)/x)>>1)|y;
}

//subset of b(descending)
int a=b;
do{
	//
	a=(a-1)&b;
}while(a!=b);

template<class F>
void subset_for(int b,F f){
	//subset of b(descending)
	int a=b;
	do{
		f(a);
		a=(a-1)&b;
	}while(a!=b);
}

// popcount=k
template<class F>
void kbit_for(int n,int k,F f){
	int c=(1<<k)-1;
	while(c<(1<<n)){
		f(c);
		if(c==0)break;
		int x=c&-c,y=c+x;
		c=(((c&~y)/x)>>1)|y;
	}
}
