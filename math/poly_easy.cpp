//CF459D
mint eval(const vc<mint>&f,mint x){
	mint a=0;
	per(i,si(f)){
		a*=x;
		a+=f[i];
	}
	return a;
}

//CF459D
vc<mint> interpolate_f(const vc<mint>&x,const vc<mint>&y){
	int n=si(x);
	assert(si(y)==n);
	vc<mint> res(n);
	vc<mint> cur(n+1);
	cur[0]=1;
	rep(i,n){
		per(j,i+1){
			cur[j+1]+=cur[j];
			cur[j]*=-x[i];
		}
	}
	vc<mint> tmp(n);
	rep(i,n){
		mint pre=0;
		per(j,n)pre=tmp[j]=cur[j+1]+pre*x[i];
		mint w=y[i]/eval(tmp,x[i]);
		rep(j,n)res[j]+=tmp[j]*w;
	}
	return res;
}

vc<mint> mult_poly(const vc<mint>&a,const vc<mint>&b){
	int n=si(a),m=si(b);
	vc<mint> c(n+m-1);
	rep(i,n)rep(j,m)c[i+j]+=a[i]*b[j];
	return c;
}

vc<mint> pow_poly(vc<mint> a,int n){
	vc<mint> res{1};
	while(n){
		if(n&1)res=mult_poly(res,a);
		n>>=1;
		if(n)a=mult_poly(a,a);
	}
	return res;
}

vc<mint> integrate(const vc<mint>&a){
	int n=si(a);
	vc<mint> c(n+1);
	rep(i,n)c[i+1]=invs[i+1]*a[i];
	return c;
}

vvc<mint> interpolate_2d(const vvc<mint>&a){
	int n=si(a),m=si(a[0]);
	vvc<mint> b(n);
	vc<mint> xs(m);
	rep(i,m)xs[i]=i;
	rep(i,n)b[i]=interpolate_f(xs,a[i]);
	xs.resize(n);
	rep(i,n)xs[i]=i;
	vc<mint> ys(n);
	rep(j,m){
		rep(i,n)ys[i]=b[i][j];
		auto c=interpolate_f(xs,ys);
		rep(i,n)b[i][j]=c[i];
	}
	return b;
}

//initfact
mint interpolate(const vc<mint>&a,mint x){
	int n=si(a);
	if(inc(0,x.v,n-1))return a[x.v];
	vc<mint> z(n+1);
	z[n]=1;
	per(i,n)z[i]=z[i+1]*(x-i);
	mint res,w=1;
	rep(i,n){
		res+=w*z[i+1]*a[i]*finv[i]*finv[n-1-i]*((n-1-i)%2?-1:1);
		w*=x-i;
	}
	return res;
}

vc<mint> mod_f(vc<mint> x,const vc<mint>&f){
	int k=si(f);
	gnr(i,k-1,si(x)){
		mint w=x[i]/f[k-1];
		rep(p,k-1)x[i-(k-1)+p]-=w*f[p];
	}
	x.resize(k-1);
	return x;
}

vc<mint> mult_mod_f(const vc<mint>&x,const vc<mint>&y,const vc<mint>&f){
	int n=si(x),m=si(y);
	vc<mint> res(n+m-1);
	rep(i,n)rep(j,m)res[i+j]+=x[i]*y[j];
	return mod_f(res,f);
}

//assume si(f)>=2 (degree(f)>=1)
vc<mint> pow_mod_f(vc<mint> x,int n,const vc<mint>&f){
	vc<mint> res{1};
	while(n){
		if(n&1)res=mult_mod_f(res,x,f);
		x=mult_mod_f(x,x,f);
		n>>=1;
	}
	return res;
}

int cutlow0(vc<mint>&a){
	rep(z,si(a))if(a[z]){
		rng(i,z,si(a))a[i-z]=a[i];
		a.resize(si(a)-z);
		return z;
	}
	int res=si(a);
	a.clear();
	return res;
}

void cuthigh0(vc<mint>&a){
	while(si(a)&&a.back()==0)a.pop_back();
}

vc<mint> monic(vc<mint> a){
	if(si(a)==0)return {};
	mint w=a.back().inv();
	for(auto&v:a)v*=w;
	return a;
}

vc<mint> gcd_poly(vc<mint> a,vc<mint> b){
	int z=min(cutlow0(a),cutlow0(b));
	cuthigh0(a);
	cuthigh0(b);
	while(si(a)&&si(b)){
		if(si(a)<si(b))swap(a,b);
		mint w=a.back()/b.back();
		rep(i,si(b))a[si(a)-si(b)+i]-=w*b[i];
		cutlow0(a);
		cuthigh0(a);
	}
	vc<mint> res=monic(si(a)==0?b:a);
	if(si(res))res.insert(res.bg,z,0);
	return res;
}

//http://lupus.is.kochi-u.ac.jp/shiota/misc/field/FiniteField.html
//Xmas2011 C
vc<mint> get_irreducible_poly(int n){
	const int p=mint::mod;
	if(n==1)return {0,1};
	while(1){
		vc<mint> f(n+1);
		f[n]=1;
		rep(i,n)f[i]=rand_int(0,p-1);
		vc<mint> g(n);
		g[1]=1;
		bool ok=true;
		rep(_,n/2){
			g=pow_mod_f(g,p,f);
			auto h=g;h[1]-=1;
			if(gcd_poly(h,f)!=vc<mint>{1}){
				ok=false;
				break;
			}
		}
		if(ok)return f;
	}
}

vc<mint> add_vec(const vc<mint>&x,const vc<mint>&y){
	int n=si(x),m=si(y),s=max(n,m);
	vc<mint> res(s);
	rep(i,s){
		if(i<n)res[i]+=x[i];
		if(i<m)res[i]+=y[i];
	}
	return res;
}

vc<mint> mult_cyc(const vc<mint>&a,const vc<mint>&b){
	int n=si(a);
	assert(si(b)==n);
	vc<mint> c(n);
	rep(i,n)rep(j,n)c[(i+j)%n]+=a[i]*b[j];
	return c;
}

vc<mint> pow_cyc(vc<mint> x,int n){
	vc<mint> res(si(x));res[0]=1;
	while(n){
		if(n&1)res=mult_cyc(res,x);
		x=mult_cyc(x,x);
		n>>=1;
	}
	return res;
}

//f を掛け算の最後に持ってくると定数倍改善するかも〜
//MITIT 2024 Spring Invitational Finals D
vc<mint> pow_easy(int n,const vc<mint>&g,mint k){
	assert(g[0]==1);
	int d=si(g);
	vc<mint> f(n);
	f[0]=1;
	rep(i,n-1){
		int u=min(i+1,d-1);
		mint val=0;
		rep(j,u)
			val+=f[i-j]*g[j+1]*(j+1);
		val*=k;
		rep(j,u)
			val-=f[i-j]*(i-j)*g[j+1];
		f[i+1]=val*invs[i+1];
	}
	return f;
}

//HDU 6997
vc<mint> pow_lowdegree2(int n,mint a,mint b,mint k){
	vc<mint> f(n);
	f[0]=1;
	f[1]=a*k;
	rng(i,1,n-1){
		f[i+1]=(f[i]*a*(k-i)+f[i-1]*b*(k*2-(i-1)))*invs[i+1];
	}
	return f;
}

//f^k をサイズ n で求める
//need invs
//fの定数項は 1 で固定
//引数では省略
vc<mint> pow_lowdegree(int n,const vc<pair<int,mint>>&f,mint k){
	for(auto [i,v]:f)assert(0<i);
	vc<mint> g(n);
	g[0]=1;
	rng(a,1,n){
		mint sum=0;
		for(auto [i,v]:f)if(i<=a){
			sum+=g[a-i]*v*(k*i-(a-i));
		}
		g[a]=sum*invs[a];
	}
	return g;
}

//f^k をサイズ n*m で求める
//need invs
//fの定数項は 1 で固定
//引数では省略
vvc<mint> pow_2d_lowdegree(int n,int m,const vc<tuple<int,int,mint>>&f,mint k){
	for(auto [i,j,v]:f){
		assert(0<=i&&0<=j);
		assert(0<i||0<j);
	}
	vvc<mint> g(n,vc<mint>(m));
	g[0][0]=1;
	rep(a,n)rep(b,m)if(a||b){
		mint sum=0;
		for(auto [i,j,v]:f)if(i<=a&&j<=b){
			sum+=g[a-i][b-j]*v*(k*(i+j)-(a-i+b-j));
		}
		g[a][b]=sum*invs[a+b];
	}
	return g;
}

vc<mint> inv_easy(const vc<mint>&f){
	int n=si(f);
	assert(n>=1);
	assert(f[0]==1);
	vc<mint> g(n);
	g[0]=1;
	rep(i,n)rng(j,i+1,n){
		g[j]-=g[i]*f[j-i];
	}
	return g;
}

vc<mint> log_easy(const vc<mint>&f){
	int n=si(f);
	assert(n>=1);
	assert(f[0]==1);
	auto g=f;
	rep(i,n)g[i]*=i;
	rep(i,n)rng(j,i+1,n){
		g[j]-=g[i]*f[j-i];
	}
	rng(i,1,n)g[i]*=invs[i];
	return g;
}

vc<mint> exp_easy(const vc<mint>&f){
	int n=si(f);
	assert(n>=1);
	assert(f[0]==0);
	auto a=f;
	rep(i,n)a[i]*=i;
	vc<mint> g(n);
	g[0]=1;
	rng(i,1,n){
		rng(j,1,i+1){
			g[i]+=a[j]*g[i-j];
		}
		g[i]*=invs[i];
	}
	return g;
}
