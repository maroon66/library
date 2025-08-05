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


//+a,-b を繰り返す
//-b はやれるだけやるみたいな感じ
//a,b の操作にそれぞれ重み x,y がついている
//重みの合計 n になるまでやる
//n を超えそうになる分は？
//type k(=0,1) の中で重み c 必要だとする
//return (res,k,c)
//↓これは愚直！高速な実装は work_rec
template<class N>
tuple<N,int,int> gauss_sum_monoid_real(ld a,ld b,int x,int y,int n,N A,N B){
	ld cur=0;
	N res;
	while(1){
		if(cur>=b){
			if(n-y<0)return mt(res,1,n);
			n-=y;
			res=N::merge(res,B);
			cur-=b;
		}else{
			if(n-x<0)return mt(res,0,n);
			n-=x;
			res=N::merge(res,A);
			cur+=a;
		}
	}
	assert(false);
}

template<class N>
N Npow(const N&v,int n){
	N res,w=v;
	while(n){
		if(n&1)res=N::merge(res,w);
		w=N::merge(w,w);
		n>>=1;
	}
	return res;
}

//nxに対して A^la*B^lb, A^ra*B^rb を渡す
//stress-tested (+1,-sqrt(2))
template<class N,class NX>
tuple<N,int,int> work_rec(int x,int y,int n,N A,N B,
	int la,int lb,int ra,int rb,NX nx){
	if(x>n)return mt(N(),0,n);
	
	int nxx=la*x+lb*y,nxy=ra*x+rb*y;
	N nxA=N::merge(Npow(A,la),Npow(B,lb));
	N nxB=N::merge(Npow(A,ra),Npow(B,rb));
	
	auto [res,k,c]=nx(nxx,nxy,n,nxA,nxB);
	if(c>0){
		if(k==0){
			if(la*x<=c){
				k=1;
				res=N::merge(res,Npow(A,la));
				c-=la*x;
				int p=c/y;
				res=N::merge(res,Npow(B,p));
				c-=p*y;
			}else{
				k=0;
				int p=c/x;
				res=N::merge(res,Npow(A,p));
				c-=p*x;
			}
		}else{
			if(ra*x<=c){
				k=1;
				res=N::merge(res,Npow(A,ra));
				c-=ra*x;
				int p=c/y;
				res=N::merge(res,Npow(B,p));
				c-=p*y;
			}else{
				k=0;
				int p=c/x;
				res=N::merge(res,Npow(A,p));
				c-=p*x;
			}
		}
	}
	return mt(res,k,c);
}

//+1,-sqrt(2)
template<class N> tuple<N,int,int> F(int x,int y,int n,N A,N B);
//+(sqrt(2)+1),-1
template<class N> tuple<N,int,int> G(int x,int y,int n,N A,N B);
//+1,-(sqrt(2)+1)
template<class N> tuple<N,int,int> H(int x,int y,int n,N A,N B);

template<class N>
tuple<N,int,int> F(int x,int y,int n,N A,N B){
	return work_rec(x,y,n,A,B,1,0,1,1,G<N>);
}
template<class N>
tuple<N,int,int> G(int x,int y,int n,N A,N B){
	return work_rec(x,y,n,A,B,1,2,0,1,H<N>);
}
template<class N>
tuple<N,int,int> H(int x,int y,int n,N A,N B){
	return work_rec(x,y,n,A,B,1,0,2,1,G<N>);
}
