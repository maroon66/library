//SRM791 Med
//getbits not verified
const int L=14;
constexpr int pow3(int n){
	return n==0?1:pow3(n-1)*3;
}
const int B=1<<L;
const int T=pow3(L);
int ternary[B];
pi getbits[T];
//assume b \in a
int getidx(int a,int b){
	return ternary[a]+ternary[b];
}
//bit の昇順，bit の subset の昇順
template<class F>
void subset_subset(int n,F f){
	rep(bit,1<<n){
		int sub=bit;
		do{
			f(bit,bit-sub);
			getbits[getidx(bit,sub)]=pi(bit,sub);
			sub=(sub-1)&bit;
		}while(sub!=bit);
	}
}
void initternary(){
	zero(ternary);
	rep(bit,B){
		int w=1;
		rep(i,L){
			if(bit&1<<i)ternary[bit]+=w;
			w*=3;
		}
	}
	subset_subset(L,[&](int a,int b){
		getbits[getidx(a,b)]=pi(a,b);
	});
}
