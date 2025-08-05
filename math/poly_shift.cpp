//veryfi yosupo
//return a(x+c)
vc<mint> taylor_shift(vc<mint> a,mint c){
	int n=si(a);
	rep(i,n)a[i]*=fact[i];
	reverse(all(a));
	vc<mint> b(finv,finv+n);
	mint w=1;
	rep(i,n){
		b[i]*=w;
		w*=c;
	}
	a=multiply(a,b);
	a.resize(n);
	reverse(all(a));
	rep(i,n)a[i]*=finv[i];
	return a;
}

//verify yosupo
vc<mint> sampling_shift(vc<mint> a,mint c,int m){
	int n=si(a);
	rep(i,n)a[i]*=finv[i];
	vc<mint> b(finv,finv+n);
	rep(i,n)b[i]*=parity(i);
	chmult(a,b,n);
	rep(i,n)a[i]*=fact[i];
	reverse(all(a));
	mint w=1;
	rep(i,n){
		b[i]=finv[i]*w;
		w*=(c-i);
	}
	chmult(a,b,n);
	reverse(all(a));
	rep(i,n)a[i]*=finv[i];
	a.resize(m);
	b.resize(m);
	rep(i,m)b[i]=finv[i];
	chmult(a,b,m);
	rep(i,m)a[i]*=fact[i];
	return a;
}

void extend_poly(vc<mint>&a,int m){
	int n=si(a);
	rep(i,n)a[i]*=finv[i];
	vc<mint> b(finv,finv+n);
	rep(i,n)b[i]*=parity(i);
	chmult(a,b,n);
	a.resize(m);
	b.resize(m);
	rep(i,m)b[i]=finv[i];
	chmult(a,b,m);
	rep(i,m)a[i]*=fact[i];
}

//Yukicoder 2166
void extend_polys(vvc<mint>&as,int m){
	int n=si(as[0]);
	for(auto&a:as)assert(si(a)==n);
	for(auto&a:as)rep(i,n)a[i]*=finv[i];
	vc<mint> b(finv,finv+n);
	rep(i,n)b[i]*=parity(i);
	for(auto&a:as)chmult(a,b,n);
	for(auto&a:as)a.resize(m);
	b.resize(m);
	rep(i,m)b[i]=finv[i];
	for(auto&a:as)chmult(a,b,m);
	for(auto&a:as)rep(i,m)a[i]*=fact[i];
}

//multiuni 2025-1-D
struct large_factorial{
	vvc<mint> xs;
	large_factorial():xs{vc<mint>{1}}{
		while(sq<ull>(si(xs.back()))<mint::mod-1){
			auto x=xs.back();
			int s=si(x);
			extend_poly(x,4*s);
			rep(i,2*s)x[i]=x[i*2]*(s*(2*i+1)+1)*x[i*2+1];
			x.resize(s*=2);
			xs.pb(x);
		}
		for(auto&x:xs){
			int s=si(x);
			rep(i,s)x[i]*=i*s+1;
		}
	}
	mint getfact(int i){
		rep(k,si(xs)){
			const auto&x=xs[k];
			int s=si(x);
			int p=i/s;
			if(p>si(x))continue;
			mint res=1;
			rep(j,p)res*=x[j];
			rng(j,p*s,i)res*=j+1;
			return res;
		}
		assert(false);
	}
};
