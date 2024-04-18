//f(x)=(ax+b)/c という直線を考える
//(0,0) からスタートする．
//まず (0,floor(f(0))) まで移動し，移動するたびに B を append
//その後以下の操作を N 回繰り返す
//(x,y) から (x+1,y) へ移動．A をappend
//(x,y) から (x,floor(f(x))) まで移動．移動するたびに B をappend
//A,B 列ができるので，これを monoid の列だと思って composite
//最終的なmonoidを返す
//多分 O(log(w+h)) 回くらいの行列乗算だと思う
//N() は単位元
//stress-tested
//https://yukicoder.me/problems/no/2362
template<class N>
N gauss_sum_monoid(int a,int b,int c,int n,N A,N B){
	assert(0<=a);
	assert(0<=b);
	assert(0<c);
	assert(0<=n);
	
	auto x_pow=[&](N&x,N v,int k){
		while(k){
			if(k&1)x=N::merge(x,v);
			if(k>>=1)v=N::merge(v,v);
		}
	};
	auto pow_x=[&](N v,int k,N&x){
		while(k){
			if(k&1)x=N::merge(v,x);
			if(k>>=1)v=N::merge(v,v);
		}
	};
	N pre,suf;
	
	while(1){
		x_pow(pre,B,b/c);
		b%=c;
		x_pow(A,B,a/c);
		a%=c;
		
		int y=(a*n+b)/c;
		if(y==0){
			x_pow(pre,A,n);
			break;
		}
		int x=(y*c-b+a-1)/a;
		pow_x(A,n-x,suf);
		suf=N::merge(B,suf);
		y--;
		b=a+c-b-1;
		swap(a,c);
		n=y;
		swap(A,B);
	}
	
	return N::merge(pre,suf);
}

struct N{
	M m;
	N():m(M::getid()){}
	N(const M&mm):m(mm){}
	static N merge(const N&a,const N&b){
		return b.m*a.m;
	}
};
