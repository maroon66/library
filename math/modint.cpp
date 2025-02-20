//mint107 は verify してねえ
//#define DYNAMIC_MOD

struct modinfo{uint mod,root;
#ifdef DYNAMIC_MOD
constexpr modinfo(uint m,uint r):mod(m),root(r),im(0){set_mod(m);}
ull im;
constexpr void set_mod(uint m){
	mod=m;
	im=ull(-1)/m+1;
}
uint product(uint a,uint b)const{
	ull z=ull(a)*b;
	uint x=((unsigned __int128)z*im)>>64;
	uint v=uint(z)-x*mod;
	return v<mod?v:v+mod;
}
#endif
};
template<modinfo const&ref>
struct modular{
	static constexpr uint const &mod=ref.mod;
	static modular root(){return modular(ref.root);}
	uint v;
	//modular(initializer_list<uint>ls):v(*ls.bg){}
	modular(ll vv=0){s(vv%mod+mod);}
	modular& s(uint vv){
		v=vv<mod?vv:vv-mod;
		return *this;
	}
	modular operator-()const{return modular()-*this;}
	modular& operator+=(const modular&rhs){return s(v+rhs.v);}
	modular&operator-=(const modular&rhs){return s(v+mod-rhs.v);}
	modular&operator*=(const modular&rhs){
		#ifndef DYNAMIC_MOD
		v=ull(v)*rhs.v%mod;
		#else
		v=ref.product(v,rhs.v);
		#endif
		return *this;
	}
	modular&operator/=(const modular&rhs){return *this*=rhs.inv();}
	modular operator+(const modular&rhs)const{return modular(*this)+=rhs;}
	modular operator-(const modular&rhs)const{return modular(*this)-=rhs;}
	modular operator*(const modular&rhs)const{return modular(*this)*=rhs;}
	modular operator/(const modular&rhs)const{return modular(*this)/=rhs;}
	modular pow(ll n)const{
		if(n<0)return inv().pow(-n);
		modular res(1),x(*this);
		while(n){
			if(n&1)res*=x;
			x*=x;
			n>>=1;
		}
		return res;
	}
	modular inv()const{return pow(mod-2);}
	/*modular inv()const{
		int x,y;
		int g=extgcd<ll>(v,mod,x,y);
		assert(g==1);
		if(x<0)x+=mod;
		return modular(x);
	}*/
	friend modular operator+(ll x,const modular&y){
		return modular(x)+y;
	}
	friend modular operator-(ll x,const modular&y){
		return modular(x)-y;
	}
	friend modular operator*(ll x,const modular&y){
		return modular(x)*y;
	}
	friend modular operator/(ll x,const modular&y){
		return modular(x)/y;
	}
	friend ostream& operator<<(ostream&os,const modular&m){
		return os<<m.v;
	}
	friend istream& operator>>(istream&is,modular&m){
		ll x;is>>x;
		m=modular(x);
		return is;
	}
	bool operator<(const modular&r)const{return v<r.v;}
	bool operator==(const modular&r)const{return v==r.v;}
	bool operator!=(const modular&r)const{return v!=r.v;}
	explicit operator bool()const{
		return v;
	}
};

#ifndef DYNAMIC_MOD
extern constexpr modinfo base{998244353,3};
//extern constexpr modinfo base{1000000007,0};
//extern constexpr modinfo base{2147483579,1689685080};//2^31 未満の最大の安全素数
//modinfo base{1,0};
#ifdef USE_GOOD_MOD
static_assert(base.mod==998244353);
#endif
#else
modinfo base(1,0);
extern constexpr modinfo base107(1000000007,0);
using mint107=modular<base107>;
#endif
using mint=modular<base>;

mint parity(int i){
	return i%2==0?1:-1;
}

#ifdef LOCAL
const int vmax=10010;
#else
const int vmax=(1<<21)+10;
#endif
mint fact[vmax],finv[vmax],invs[vmax];
void initfact(){
	const int s=min<int>(vmax,base.mod);
	fact[0]=1;
	rng(i,1,s){
		fact[i]=fact[i-1]*i;
	}
	finv[s-1]=fact[s-1].inv();
	for(int i=s-2;i>=0;i--){
		finv[i]=finv[i+1]*(i+1);
	}
	for(int i=s-1;i>=1;i--){
		invs[i]=finv[i]*fact[i-1];
	}
}
mint choose(int n,int k){
	return inc(0,k,n)?fact[n]*finv[n-k]*finv[k]:0;
}
mint binom(int a,int b){
	return 0<=a&&0<=b?fact[a+b]*finv[a]*finv[b]:0;
}
mint catalan(int n){
	return binom(n,n)-(n-1>=0?binom(n-1,n+1):0);
}
//対角線を超えず (x,y) に至る方法の数
mint catalan(int x,int y){
	assert(y<=x);
	return binom(x,y)-binom(x+1,y-1);
}
//y=x+c を超えず (x,y) に至る方法の数
mint catalan(int x,int y,int c){
	assert(y<=x+c);
	return binom(x,y)-binom(x+c+1,y-c-1);
}

/*
const int vmax=610;
mint fact[vmax+1],binbuf[vmax+1][vmax+1];
mint choose(int n,int k){
	return 0<=k&&k<=n?binbuf[n-k][k]:0;
}
mint binom(int a,int b){
	return 0<=a&&0<=b?binbuf[a][b]:0;
}
void initfact(int n){
	fact[0]=1;
	rep(i,n)fact[i+1]=fact[i]*(i+1);
	rep(i,n+1)rep(j,n+1){
		if(i==0&&j==0){
			binbuf[i][j]=1;
		}else{
			binbuf[i][j]=0;
			if(i)binbuf[i][j]+=binbuf[i-1][j];
			if(j)binbuf[i][j]+=binbuf[i][j-1];
		}
	}
}
*/

mint p2[vmax],p2inv[vmax];
void initp2(){
	p2[0]=1;
	rep(i,vmax-1)p2[i+1]=p2[i]*2;
	p2inv[vmax-1]=p2[vmax-1].inv();
	per(i,vmax-1)p2inv[i]=p2inv[i+1]*2;
}

int m2i(mint a){
	uint v=a.v;
	return v<mint::mod/2?v:int(v)-int(mint::mod);
}

pi m2f(mint x){
	pi res(inf,inf);
	auto upd=[&](int a,int b){
		if(abs(res.a)+res.b>abs(a)+b){
			res=pi(a,b);
		}
	};
	rng(b,1,1000){
		mint y=x*b;
		upd(y.v,b);
		upd(-int((-y).v),b);
	}
	return res;
}

ll m2l(mint a){
	return a.v<mint::mod/2?a.v:ll(a.v)-ll(mint::mod);
}

void showpoly(const vc<mint>&a){
	vi tmp(si(a));
	rep(i,si(a)){
		tmp[i]=m2l(a[i]);
	}
	cerr<<tmp<<endl;
}

struct initfactp2{initfactp2(){initfact();initp2();}}initfactp2_;
