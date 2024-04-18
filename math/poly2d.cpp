//multiply_givenlength を導入
//arbitrary だけ verify
//XXI Opencup GP of Belarus B

//UOJ Round18 B
//UOJ Long Round C
vvc<mint> convolute2d(const vvc<mint>&x,const vvc<mint>&y){
	int n1=si(x),m1=si(x[0]);
	int n2=si(y),m2=si(y[0]);
	int n=n1+n2-1,m=m1+m2-1;
	int s=1;
	while(s<n*m)s*=2;
	vc<mint> p(s),q(s);
	rep(i,n1)rep(j,m1)p[i*m+j]=x[i][j];
	rep(i,n2)rep(j,m2)q[i*m+j]=y[i][j];
	auto r=multiply_givenlength(p,q);
	vvc<mint> res(n,vc<mint>(m));
	rep(i,n)rep(j,m)res[i][j]=r[i*m+j];
	return res;
}

//UOJ Round18 B
vvc<mint> square2d(const vvc<mint>&x){
	int n1=si(x),m1=si(x[0]);
	int n=n1*2-1,m=m1*2-1,s=1;
	while(s<n*m)s*=2;
	vc<mint> p(s);
	rep(i,n1)rep(j,m1)p[i*m+j]=x[i][j];
	auto r=multiply_givenlength(p,p,true);
	vvc<mint> res(n,vc<mint>(m));
	rep(i,n)rep(j,m)res[i][j]=r[i*m+j];
	return res;
}

//UOJ Round18 B
vvc<mint> inv2d(const vvc<mint>&x){
	int n=si(x),m=si(x[0]);
	vvc<mint> r{{x[0][0].inv()}};
	for(int h=1,w=1,s=1;s<n+m-1;s*=2,h=min(s,n),w=min(s,m)){
		int ns=s*2,nh=min(ns,n),nw=min(ns,m);
		vvc<mint> nx(nh,vc<mint>(nw));
		rep(i,h)rep(j,w)if(i+j<s)nx[i][j]=r[i][j]*2;
		vvc<mint> tmp(nh,vc<mint>(nw));
		rep(i,nh)rep(j,nw)if(i+j<ns)tmp[i][j]=x[i][j];
		auto val=convolute2d(square2d(r),tmp);
		rep(i,nh)rep(j,nw)if(i+j<ns)nx[i][j]-=val[i][j];
		r.swap(nx);
	}
	return r;
}

//UOJ Round18 B
//縦横 sqrt(500000)で 1s 程度
vvc<mint> squareroot2d(const vvc<mint>&x){
	assert(x[0][0]==1);
	int n=si(x),m=si(x[0]);
	assert(n<=m);
	vvc<mint> r{{1}};
	const mint half=mint(1)/2;
	for(int h=1,w=1,s=1;s<n+m-1;s*=2,h=min(s,n),w=min(s,m)){
		int ns=s*2,nh=min(ns,n),nw=min(ns,m);
		vvc<mint> tmp(nh,vc<mint>(nw));
		rep(i,h)rep(j,w)if(i+j<s)tmp[i][j]=r[i][j];
		vvc<mint> nx=tmp;
		vvc<mint> low(nh,vc<mint>(nw));
		rep(i,nh)rep(j,nw)if(i+j<ns)low[i][j]=x[i][j];
		auto z=inv2d(tmp);
		z.resize(nh);
		rep(i,nh)z[i].resize(nw);
		auto val=convolute2d(z,low);
		rep(i,nh)rep(j,nw)if(i+j<ns){
			nx[i][j]+=val[i][j];
			nx[i][j]*=half;
		}
		r.swap(nx);
	}
	return r;
}

//一切 verify どころかコンパイルもしてない，驚いたか？
		/*
vvc<mint> exp2d(const vvc<mint>&x){
	assert(x[0][0]==0);
	int n=si(x),m=si(x[0]);
	assert(n<=m);
	vvc<mint> f{{1}},g{{1}};
	for(int h=1,w=1,s=1;s<n+m-1;s*=2,h=min(s,n),w=min(s,m)){
		int ns=s*2,nh=min(ns,n),nw=min(ns,m);
		
		if(s>1){
			vvc<mint> ng(h,vc<mint>(w));
			rep(i,si(g))rep(j,si(g[i]))if(i+j<s/2)ng[i][j]=g[i][j]*2;
			g=convolute2d(square2d(g),f);
			rep(i,h)rep(j,w)if(i+j<s)ng[i][j]-=g[i][j];
			g.swap(ng);
		}
		
	if(n>=s)break;
	g=g*2-(g.square()*f).low(n);
	//if(n>=s)break;
	Poly q=low(n).dif();
	q=q+g*(f.dif()-f*q).low(2*n-1);
	f=f+(f*(low(2*n)-q.inte(invs))).low(2*n);
		
		vvc<mint> q(h,vc<mint>(w));
		rep(i,h)rep(j,w)if(i+j<s)q[i][j]=x[i][j]*(i+j);
		vvc<mint> e=convolute2d(f,q);
		rep(i,si(e))rep(j,si(e[i]))e[i][j]=-e[i][j];
		rep(i,h)rep(j,w)e[i][j]+=f[i][j]*(i+j);
		e=convolute2d(g,e);
		q.resize(nh);
		rep(i,nh)q[i].resize(nw);
		rep(i,min(nh,si(e)))rep(j,min(nw,si(e[i])))q[i][j]+=e[i][j];
		rep(i,nh)rep(j,nw)if(i+j==0){
		}else if(i+j<ns){
			q[i][j]=x[i][j]-q[i][j]*invs[i+j];
		}else{
			q[i][j]=0;
		}
		vvc<mint> last=convolute2d(f,q);
		f.resize(nh);
		rep(i,nh){
			f[i].resize(nw);
			rep(j,nw)if(inc(s,i+j,ns-1))f[i][j]=last[i][j];
		}
	}
	return f;
}
		*/
