#define USE_GOOD_MOD

//size of input must be a power of 2
//output of forward fmt is bit-reversed
//output elements are in the range [0,mod*4)
//input of inverse fmt should be bit-reversed
template<class mint>
void inplace_fmt(const int n,mint*const f,bool inv){
	static constexpr uint mod=mint::mod;
	static constexpr uint mod2=mod*2;
	static constexpr int L=30;
	static mint g[L],ig[L],p2[L];
	if(g[0].v==0){
		rep(i,L){
			mint w=-mint::root().pow(((mod-1)>>(i+2))*3);
			g[i]=w;
			ig[i]=w.inv();
			p2[i]=mint(1<<i).inv();
		}
	}
	if(!inv){
		int b=n;
		if(b>>=1){//input:[0,mod)
			rep(i,b){
				uint x=f[i+b].v;
				f[i+b].v=f[i].v+mod-x;
				f[i].v+=x;
			}
		}
		if(b>>=1){//input:[0,mod*2)
			mint p=1;
			for(int i=0,k=0;i<n;i+=b*2){
				rng(j,i,i+b){
					uint x=(f[j+b]*p).v;
					f[j+b].v=f[j].v+mod-x;
					f[j].v+=x;
				}
				p*=g[__builtin_ctz(++k)];
			}
		}
		while(b){
			if(b>>=1){//input:[0,mod*3)
				mint p=1;
				for(int i=0,k=0;i<n;i+=b*2){
					rng(j,i,i+b){
						uint x=(f[j+b]*p).v;
						f[j+b].v=f[j].v+mod-x;
						f[j].v+=x;
					}
					p*=g[__builtin_ctz(++k)];
				}
			}
			if(b>>=1){//input:[0,mod*4)
				mint p=1;
				for(int i=0,k=0;i<n;i+=b*2){
					rng(j,i,i+b){
						uint x=(f[j+b]*p).v;
						f[j].v=(f[j].v<mod2?f[j].v:f[j].v-mod2);
						f[j+b].v=f[j].v+mod-x;
						f[j].v+=x;
					}
					p*=g[__builtin_ctz(++k)];
				}
			}
		}
	}else{
		int b=1;
		if(b<n/2){//input:[0,mod)
			mint p=1;
			for(int i=0,k=0;i<n;i+=b*2){
				rng(j,i,i+b){
					ull x=f[j].v+mod-f[j+b].v;
					f[j].v+=f[j+b].v;
					f[j+b].v=x*p.v%mod;
				}
				p*=ig[__builtin_ctz(++k)];
			}
			b<<=1;
		}
		for(;b<n/2;b<<=1){
			mint p=1;
			for(int i=0,k=0;i<n;i+=b*2){
				rng(j,i,i+b/2){//input:[0,mod*2)
					ull x=f[j].v+mod2-f[j+b].v;
					f[j].v+=f[j+b].v;
					f[j].v=(f[j].v)<mod2?f[j].v:f[j].v-mod2;
					f[j+b].v=x*p.v%mod;
				}
				rng(j,i+b/2,i+b){//input:[0,mod)
					ull x=f[j].v+mod-f[j+b].v;
					f[j].v+=f[j+b].v;
					f[j+b].v=x*p.v%mod;
				}
				p*=ig[__builtin_ctz(++k)];
			}
		}
		if(b<n){//input:[0,mod*2)
			rep(i,b){
				uint x=f[i+b].v;
				f[i+b].v=f[i].v+mod2-x;
				f[i].v+=x;
			}
		}
		mint z=p2[__lg(n)];
		rep(i,n)f[i]*=z;
	}
}

template<class mint>
void inplace_fmt(vector<mint>&f,bool inv){
	inplace_fmt(si(f),f.data(),inv);
}

//size of input must be a power of 2
//output elements are in the range [0,mod*4)
template<class mint>
void half_fmt(const int n,mint*const f){
	static constexpr uint mod=mint::mod;
	static constexpr uint mod2=mod*2;
	static const int L=30;
	static mint g[L],h[L];
	if(g[0].v==0){
		rep(i,L){
			g[i]=-mint::root().pow(((mod-1)>>(i+2))*3);
			h[i]=mint::root().pow((mod-1)>>(i+2));
		}
	}
	int b=n;
	int lv=0;
	if(b>>=1){//input:[0,mod)
		mint p=h[lv++];
		for(int i=0,k=0;i<n;i+=b*2){
			rng(j,i,i+b){
				uint x=(f[j+b]*p).v;
				f[j+b].v=f[j].v+mod-x;
				f[j].v+=x;
			}
			p*=g[__builtin_ctz(++k)];
		}
	}
	if(b>>=1){//input:[0,mod*2)
		mint p=h[lv++];
		for(int i=0,k=0;i<n;i+=b*2){
			rng(j,i,i+b){
				uint x=(f[j+b]*p).v;
				f[j+b].v=f[j].v+mod-x;
				f[j].v+=x;
			}
			p*=g[__builtin_ctz(++k)];
		}
	}
	while(b){
		if(b>>=1){//input:[0,mod*3)
			mint p=h[lv++];
			for(int i=0,k=0;i<n;i+=b*2){
				rng(j,i,i+b){
					uint x=(f[j+b]*p).v;
					f[j+b].v=f[j].v+mod-x;
					f[j].v+=x;
				}
				p*=g[__builtin_ctz(++k)];
			}
		}
		if(b>>=1){//input:[0,mod*4)
			mint p=h[lv++];
			for(int i=0,k=0;i<n;i+=b*2){
				rng(j,i,i+b){
					uint x=(f[j+b]*p).v;
					f[j].v=(f[j].v<mod2?f[j].v:f[j].v-mod2);
					f[j+b].v=f[j].v+mod-x;
					f[j].v+=x;
				}
				p*=g[__builtin_ctz(++k)];
			}
		}
	}
}

template<class mint>
void half_fmt(vector<mint>&f){
	half_fmt(si(f),f.data());
}

#ifdef USE_GOOD_MOD

template<class mint>
vc<mint> multiply(vc<mint> x,const vc<mint>&y,bool same=false){
	int n=si(x)+si(y)-1;
	int s=1;
	while(s<n)s*=2;
	x.resize(s);inplace_fmt(x,false);
	if(!same){
		static vc<mint> z;
		z.clear();z.resize(s);
		rep(i,si(y))z[i]=y[i];
		inplace_fmt(z,false);
		rep(i,s)x[i]*=z[i];
	}else{
		rep(i,s)x[i]*=x[i];
	}
	inplace_fmt(x,true);x.resize(n);
	return x;
}
template<class mint>
vc<mint> multiply_givenlength(vc<mint> x,const vc<mint>&y,bool same=false){
	int s=si(x);
	assert(ispow2(s));
	assert(si(y));
	x.resize(s);inplace_fmt(x,false);
	if(!same){
		static vc<mint> z;
		z.clear();z.resize(s);
		rep(i,si(y))z[i]=y[i];
		inplace_fmt(z,false);
		rep(i,s)x[i]*=z[i];
	}else{
		rep(i,s)x[i]*=x[i];
	}
	inplace_fmt(x,true);
	return x;
}

#else

//59501818244292734739283969-1=5.95*10^25 までの値を正しく計算
//最終的な列の大きさが 2^24 までなら動く
//最終的な列の大きさが 2^20 以下のときは，下の 3 つの素数を使ったほうが速い（は？）
//VERIFY: yosupo
//Yukicoder No980 (same=true)
namespace arbitrary_convolution{
	constexpr modinfo base0{167772161,3};//2^25 * 5 + 1
	constexpr modinfo base1{469762049,3};//2^26 * 7 + 1
	constexpr modinfo base2{754974721,11};//2^24 * 45 + 1
	//extern constexpr modinfo base0{1045430273,3};//2^20 * 997 + 1
	//extern constexpr modinfo base1{1051721729,6};//2^20 * 1003 + 1
	//extern constexpr modinfo base2{1053818881,7};//2^20 * 1005 + 1
	using mint0=modular<base0>;
	using mint1=modular<base1>;
	using mint2=modular<base2>;
	template<class t,class mint>
	vc<t> sub(const vc<mint>&x,const vc<mint>&y,bool same=false){
		int n=si(x)+si(y)-1;
		int s=1;
		while(s<n)s*=2;
		vc<t> z(s);rep(i,si(x))z[i]=x[i].v;
		inplace_fmt(z,false);
		if(!same){
			vc<t> w(s);rep(i,si(y))w[i]=y[i].v;
			inplace_fmt(w,false);
			rep(i,s)z[i]*=w[i];
		}else{
			rep(i,s)z[i]*=z[i];
		}
		inplace_fmt(z,true);z.resize(n);
		return z;
	}
	template<class mint>
	vc<mint> multiply(const vc<mint>&x,const vc<mint>&y,bool same=false){
		auto d0=sub<mint0>(x,y,same);
		auto d1=sub<mint1>(x,y,same);
		auto d2=sub<mint2>(x,y,same);
		int n=si(d0);
		vc<mint> res(n);
		static const mint1 r01=mint1(mint0::mod).inv();
		static const mint2 r02=mint2(mint0::mod).inv();
		static const mint2 r12=mint2(mint1::mod).inv();
		static const mint2 r02r12=r02*r12;
		static const mint w1=mint(mint0::mod);
		static const mint w2=w1*mint(mint1::mod);
		rep(i,n){
			ull a=d0[i].v;
			ull b=(d1[i].v+mint1::mod-a)*r01.v%mint1::mod;
			ull c=((d2[i].v+mint2::mod-a)*r02r12.v+(mint2::mod-b)*r12.v)%mint2::mod;
			res[i].v=(a+b*w1.v+c*w2.v)%mint::mod;
		}
		return res;
	}
	template<class t,class mint>
	vc<t>&sub_givenlength(const vc<mint>&x,const vc<mint>&y,bool same=false){
		int s=si(x);
		assert(ispow2(s));
		assert(si(y)==s);
		static vc<t> z;
		z.clear();z.resize(s);
		rep(i,si(x))z[i]=x[i].v;
		inplace_fmt(z,false);
		if(!same){
			static vc<t> w;
			w.clear();w.resize(s);
			rep(i,si(y))w[i]=y[i].v;
			inplace_fmt(w,false);
			rep(i,s)z[i]*=w[i];
		}else{
			rep(i,s)z[i]*=z[i];
		}
		inplace_fmt(z,true);
		return z;
	}
	template<class mint>
	vc<mint> multiply_givenlength(vc<mint> x,const vc<mint>&y,bool same=false){
		auto&d0=sub_givenlength<mint0>(x,y,same);
		auto&d1=sub_givenlength<mint1>(x,y,same);
		auto&d2=sub_givenlength<mint2>(x,y,same);
		int n=si(d0);
		x.resize(n);
		static const mint1 r01=mint1(mint0::mod).inv();
		static const mint2 r02=mint2(mint0::mod).inv();
		static const mint2 r12=mint2(mint1::mod).inv();
		static const mint2 r02r12=r02*r12;
		static const mint w1=mint(mint0::mod);
		static const mint w2=w1*mint(mint1::mod);
		rep(i,n){
			ull a=d0[i].v;
			ull b=(d1[i].v+mint1::mod-a)*r01.v%mint1::mod;
			ull c=((d2[i].v+mint2::mod-a)*r02r12.v+(mint2::mod-b)*r12.v)%mint2::mod;
			x[i].v=(a+b*w1.v+c*w2.v)%mint::mod;
		}
		return x;
	}
}
using arbitrary_convolution::multiply;
using arbitrary_convolution::multiply_givenlength;

#endif

//UTPC2021 C
namespace integer_convolution{
	extern constexpr modinfo base0{1045430273,3};//2^20 * 997 + 1
	extern constexpr modinfo base1{1051721729,6};//2^20 * 1003 + 1
	//extern constexpr modinfo base0{469762049,3};//2^26 * 7 + 1
	//extern constexpr modinfo base1{754974721,11};//2^24 * 45 + 1
	using mint0=modular<base0>;
	using mint1=modular<base1>;
	template<class t>
	vc<t> sub(const vi&x,const vi&y,bool same=false){
		int n=si(x)+si(y)-1;
		int s=1;
		while(s<n)s*=2;
		vc<t> z(s);rep(i,si(x))z[i]=x[i];
		inplace_fmt(z,false);
		if(!same){
			vc<t> w(s);rep(i,si(y))w[i]=y[i];
			inplace_fmt(w,false);
			rep(i,s)z[i]*=w[i];
		}else{
			rep(i,s)z[i]*=z[i];
		}
		inplace_fmt(z,true);z.resize(n);
		return z;
	}
	vi multiply(const vi&x,const vi&y,bool same=false){
		auto d0=sub<mint0>(x,y,same);
		auto d1=sub<mint1>(x,y,same);
		const mint1 r=mint1(mint0::mod).inv();
		const ll v=ll(mint0::mod)*mint1::mod;
		int n=si(d0);
		vi res(n);
		rep(i,n){
			res[i]=d0[i].v+(r*(d1[i]-d0[i].v)).v*(ull)mint0::mod;
			if(res[i]>v/2)res[i]-=v;
		}
		return res;
	}
}

//最大で 1<<mx のサイズの fft が登場！
template<class mint>
vc<mint> large_convolution(const vc<mint>&a,const vc<mint>&b,int mx){
	int n=si(a),m=si(b);
	vc<mint> c(n+m-1);
	int len=1<<(mx-1);
	for(int i=0;i<n;i+=len){
		for(int j=0;j<n;j+=len){
			int x=min(len,n-i),y=min(len,m-j);
			auto d=multiply(vc<mint>(a.bg+i,a.bg+i+x),vc<mint>(b.bg+j,b.bg+j+y));
			rep(k,si(d))
				c[i+j+k]+=d[k];
		}
	}
	return c;
}

//input A: N 次,B ?,M
//output D: M 次多項式
//C を M 次多項式として
//[x^N] A*B*C = [x^M] D*C
//となるような D を返す
//CF796F
template<class mint>
vc<mint> transpose_advance(const vc<mint>&a,const vc<mint>&b,int m){
	int n=si(a)-1;
	auto d=multiply(a,b);
	vc<mint> res(m+1);
	if(n>=m){
		rep(i,m+1)res[i]=d[i+n-m];
	}else{
		rng(i,m-n,m+1)res[i]=d[i+n-m];
	}
	return res;
}

//Yukicoder 2166
template<class mint>
void chmult(vc<mint>&x,const vc<mint>&y,int s){
	x=multiply(move(x),y);
	x.resize(s);
}
