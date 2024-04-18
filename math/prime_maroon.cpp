//モンゴメリ乗算
//S=2^64, I=S^-1 mod N, R*N=-1 mod S, T=2^64 mod N, E=2^128 mod N
//re(x) (x<S*N) return (x*I)%N (+0 or N)
//(x*I) = (x/S) = (x+(x*R)%S*N)/S mod N
//mult(x,y) (x*y<S*N) return (x*y*I)%N (+0 or N)
//en(x) (x<S) モンゴメリ空間に移す
//de(x) (x<S*N) 元の空間に戻す
struct m64{
	using u128=__uint128_t;
	ull n,n2,r,t,e;
	m64(ull nn){
		n=nn;
        assert(n<(1ull<<61));//2^61<=n<2^62 が必要なときは factor で add を使う
        assert(n%2==1);
        n2=n*2;
        r=n&3;
        rep(_,5)r*=2-n*r;
        r=-r;
        assert(r*n==-1ull);
        t=-ull(n)%n;
        e=-u128(n)%n;
	}
	ull re(u128 x){return (x+u128(ull(x)*r)*n)>>64;}
	ull mult(ull x,ull y){return re(u128(x)*y);}
	ull en(ull x){return mult(x,e);}
	ull de(ull x){x=re(x);return x<n?x:x-n;}
	ull pow(ull x,ull p){
		ull res=t;
		while(p){
			if(p&1)res=mult(res,x);
			x=mult(x,x);
			p>>=1;
		}
		return res;
	}
	
	bool pri(){
		assert(n>=3);
		assert(n&1);
		ull d=n-1;
		int s=__builtin_ctzll(d);
		d>>=s;
		auto check=[&](ull a)->int{
			a%=n;
			if(a==0)return 1;
			ull y=pow(en(a),d);
			if(de(y)==1||de(y)==n-1)return 0;
			rep(i,s){
				y=mult(y,y);
				if(de(y)==n-1)return 0;
			}
			return -1;
		};
		if(n<4759123141ull){
			for(ull a:{2, 7, 61}){
				int w=check(a);
				if(w)return w==1;
			}
		}else{
			for(ull a:{2, 325, 9375, 28178, 450775, 9780504, 1795265022}){
				int w=check(a);
				if(w)return w==1;
			}
		}
		return true;
	}
	
	ull c;
	ull f(ull x){return re(u128(x)*x+c);}
	ull add(ull x,ull y){x+=y;return x<n2?x:x-n2;}
	ull div(){
		assert(n>=3);
		assert(n&1);
		if(pri())return n;
		mt19937_64 gen;
		while(1){
			c=gen()%(n-1)+1;
			ull y=gen()%(n-1)+1;
			for(ull s=1;;s<<=1){
				ull x=n2-y;
				ull m=min(ull(1)<<(__lg(n)/6),s);
				rep(i,s/m){
					ull w=t,z=y;
					rep(j,m){
						y=f(y);
						w=mult(w,y+x);
						//w=mult(w,add(y,x)); (2^61<=n<2^62)
					}
					ull g=gcd(de(w),n);
					if(g!=1){
						if(g<n)return g;
						rep(j,m){
							z=f(z);
							if((g=gcd(de(z+x),n))!=1){
								if(g<n)return g;
								else goto fail;
							}
						}
						assert(false);
					}
				}
			}
			fail:;
		}
	}
};

bool isprime(ull n) {
	if(n<=1)return false;
	if(n==2)return true;
	if(n%2==0)return false;
	return m64(n).pri();
}

//素数なら自身を返す，合成数なら非自明な約数を返す
ull finddivisor(ull n){
	assert(n>1);
	if(n%2==0)return 2;
	return m64(n).div();
}

//Q=100 20ms on yosupo
vc<ull> factorize(ull n){
	vc<ull> res;
	auto dfs=[&](auto self,ull v)->void{
		if(v==1)return;
		ull x=finddivisor(v);
		if(x==v)return res.pb(x);
		self(self,x);
		self(self,v/x);
	};
	dfs(dfs,n);
	sort(all(res));
	return res;
}

vc<pi> to_freq(vc<ull> a){
	sort(all(a));
	vc<pi> res;
	for(auto v:a){
		if(res.empty()||res.back().a!=(int)v)res.eb((int)v,0);
		res.back().b++;
	}
	return res;
}

//https://codeforces.com/contest/1782/problem/D
vi enum_divs(int n){
	auto f=to_freq(factorize(n));
	
	vi res;
	auto rec=[&](auto self,int i,int d)->void{
		if(i==si(f))res.pb(d);
		else{
			rep(_,f[i].b+1){
				self(self,i+1,d);
				d*=f[i].a;
			}
		}
	};
	rec(rec,0,1);
	
	return res;
}

template<class t>
t pow_mod(t x,t n,t m){
	t r=1;
	while(n){
		if(n&1)r=(r*x)%m;
		x=(x*x)%m;
		n>>=1;
	}
    return r;
}

//UCUP 1-18 E
int order(int a,int p){
	assert(inc(1,a,p-1));
	int x=p-1;
	for(auto q:factorize(p-1))
		if(pow_mod<__int128>(a,x/q,p)==1)
			x/=q;
	return x;
}

//assume p is a prime
//not verified
bool is_primitiveroot(int r,int p){
	if(!r)return false;
	for(auto q:factorize(p-1))
		if(pow_mod<__int128>(r,(p-1)/q,p)==1)
			return false;
	return true;
}

//assume p is a prime
//yukicoder No.1025
int get_primitiveroot(int p){
	rng(w,1,p)if(is_primitiveroot(w,p))return w;
	assert(false);
}

const int vmax=ten(6)+10;
bool pri[vmax];
vi ps;
void initprime(){
	rng(i,2,vmax)
		pri[i]=1;
	ps.clear();
	rng(i,2,vmax)if(pri[i]){
		ps.pb(i);
		for(int j=i*2;j<vmax;j+=i)
			pri[j]=0;
	}
}
vi fs[vmax];
void initfactor(){
	rng(i,2,vmax)if(fs[i].empty())
		for(int j=i;j<vmax;j+=i)
			fs[j].pb(i);
}
vi ds[vmax];
void initdivisor(){
	rng(i,1,vmax)
		for(int j=i;j<vmax;j+=i)
			ds[j].pb(i);
}
int mu[vmax];
void initmu(){
	rng(i,1,vmax)mu[i]=1;
	rng(i,2,vmax)if(pri[i]){
		for(int j=i;j<vmax;j+=i)
			if(j%(i*i))
				mu[j]=-mu[j];
			else
				mu[j]=0;
	}
}

vi factors(int x){
	vi res;
	for(int i=2;i*i<=x;i+=(i==2?1:2)){
		if(x%i==0){
			res.pb(i);
			while(x%i==0)
				x/=i;
		}
	}
	if(x>1)res.pb(x);
	sort(all(res));
	return res;
}

vi divisors(int n){
	vi res;
	for(int i=1;i*i<=n;i++){
		if(n%i==0){
			res.pb(i);
			if(i<n/i)res.pb(n/i);
		}
	}
	sort(all(res));
	return res;
}
