//UOJ 596
vc<mint> multivariable_mult(const vc<mint>&f,const vc<mint>&g,const vi&ns,const vi&hs){
	int n=si(f),k=si(ns);
	assert(si(g)==n);
	int mx=1;while(mx<(n*2-1))mx*=2;
	vc<mint> h(n);
	vvc<mint> fbuf(k,vc<mint>(mx));
	vvc<mint> gbuf(k,vc<mint>(mx));
	vvc<mint> hbuf(k,vc<mint>(mx));
	rep(i,n)fbuf[hs[i]][i]=f[i];
	rep(i,k)inplace_fmt(fbuf[i],false);
	rep(i,n)gbuf[hs[i]][i]=g[i];
	rep(i,k)inplace_fmt(gbuf[i],false);
	rep(a,k)rep(b,k){
		int c=(a+b)%k;
		rep(i,mx)hbuf[c][i]+=fbuf[a][i]*gbuf[b][i];
	}
	rep(i,k)inplace_fmt(hbuf[i],true);
	rep(i,n)h[i]=hbuf[hs[i]][i];
	return h;
}

//UOJ 596
vc<mint> multivariable_inv(const vc<mint>&f,const vi&ns,const vi&hs){
	int n=si(f),k=si(ns);
	int mx=1;while(mx<n)mx*=2;
	vc<mint> g(n);g[0]=f[0].inv();
	vvc<mint> fbuf(k,vc<mint>(mx));
	vvc<mint> gbuf(k,vc<mint>(mx));
	vvc<mint> hbuf(k,vc<mint>(mx));
	for(int s=1;s<n;s*=2){
		rep(i,k){
			fbuf[i].assign(s*2,0);
			gbuf[i].assign(s*2,0);
			hbuf[i].assign(s*2,0);
		}
		rep(i,min(n,s*2)){
			fbuf[hs[i]][i]=f[i];
		}
		rep(i,s){
			gbuf[hs[i]][i]=g[i];
		}
		rep(i,k){
			inplace_fmt(fbuf[i],false);
			inplace_fmt(gbuf[i],false);
		}
		rep(a,k)rep(b,k){
			int c=(a+b)%k;
			rep(i,s*2)hbuf[c][i]+=fbuf[a][i]*gbuf[b][i];
		}
		rep(i,k){
			inplace_fmt(hbuf[i],true);
		}
		rep(i,k){
			fbuf[i].assign(s*2,0);
		}
		rng(i,s,min(n,s*2)){
			fbuf[hs[i]][i]=hbuf[hs[i]][i];
		}
		rep(i,k){
			inplace_fmt(fbuf[i],false);
		}
		rep(i,k){
			hbuf[i].assign(s*2,0);
		}
		rep(a,k)rep(b,k){
			int c=(a+b)%k;
			rep(i,s*2)hbuf[c][i]+=fbuf[a][i]*gbuf[b][i];
		}
		rep(i,k){
			inplace_fmt(hbuf[i],true);
		}
		rng(i,s,min(n,s*2)){
			g[i]=-hbuf[hs[i]][i];
		}
	}
	return g;
}
