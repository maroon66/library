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

//[x^{n-1}] a*b^i (0<=i<n)
//b[0]=0 を仮定
vc<mint> power_projection(vc<mint> a,vc<mint> b){
	int n=si(a);
	assert(si(b)==n);
	assert(b[0]==0);
	int s=minp2(n);
	a<<=(s-n);
	b.resize(s);
	
	vc<mint> p(4*s),q(4*s),r(4*s);
	rep(i,s)p[i*2]=a[i];
	rep(i,s)q[i*2]=-b[i];
	q[1]=1;
	for(int k=1;k<s;k*=2){
		rep(i,2*s)r[i]=q[i];
		rng(i,2*s,4*s)r[i]=0;
		for(int i=1;i<s/k;i+=2)
			rep(j,k)r[i*(2*k)+j]*=-1;
		inplace_fmt(r,false);
		inplace_fmt(p,false);
		rep(i,4*s)p[i]*=r[i];
		inplace_fmt(p,true);
		rng(i,2*s,4*s)p[i]=0;
		for(int i=0;i<s/k;i+=2){
			rep(j,2*k){
				p[i*(2*k)+j]=p[(i+1)*(2*k)+j];
				p[(i+1)*(2*k)+j]=0;
			}
		}
		if(2*k<s){
			inplace_fmt(q,false);
			rep(i,4*s)q[i]*=r[i];
			inplace_fmt(q,true);
			rng(i,2*s,4*s)q[i]=0;
		}
	}
	
	p.resize(s);
	rein(p);
	p.resize(n);
	return p;
}

//https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large
//N=2^17 で 1000ms くらい
//a[0]==0
//a[1]!=0
vc<mint> compositional_inverse(vc<mint> a){
	int n=si(a);
	assert(n>=2);
	assert(a[0]==0);
	assert(a[1]!=0);
	mint a1inv=a[1].inv();
	a*=a1inv;
	
	vc<mint> tmp(n);
	tmp[0]=1;
	auto pp=power_projection(tmp,a);
	rng(k,1,n)pp[k]*=invs[k]*(n-1);
	rein(pp);
	pp.resize(n-1);
	auto b=((Poly<mint>)pp).pow_const1(-invs[n-1],invs);
	b<<=1;
	
	mint cur=1;
	rep(i,n){
		b[i]*=cur;
		cur*=a1inv;
	}
	return b;
}

//https://judge.yosupo.jp/problem/composition_of_formal_power_series_large
//N=2^17 で 900ms くらい
//n=si(a)=si(b)
//returns a(b(x)) of size n
//b[0]=0 を仮定
vc<mint> composition(vc<mint> a,vc<mint> b){
	int n=si(a);
	assert(si(b)==n);
	assert(b[0]==0);
	int s=minp2(n);
	a.resize(s);
	rein(a);
	b.resize(s);
	
	int L=__lg(s);
	vvc<mint> qbuf(L,vc<mint>(4*s));
	vc<mint> q(4*s);
	rep(i,s)q[i*2]=-b[i];
	q[1]=1;
	for(int k=1,lv=0;k<s;k*=2,lv++){
		rep(i,2*s)qbuf[lv][i]=q[i];
		for(int i=1;i<s/k;i+=2)
			rep(j,k)qbuf[lv][i*(2*k)+j]*=-1;
		inplace_fmt(qbuf[lv],false);
		if(2*k<s){
			inplace_fmt(q,false);
			rep(i,4*s)q[i]*=qbuf[lv][i];
			inplace_fmt(q,true);
			rng(i,2*s,4*s)q[i]=0;
		}
	}
	
	auto p=a;p.resize(2*s);
	rein(p);
	p.resize(4*s);
	for(int k=s/2,lv=L-1;k>=1;k/=2,lv--){
		for(int i=0;i<s/k;i+=2){
			rep(j,2*k){
				p[i*(2*k)+j]=p[(i+1)*(2*k)+j];
				p[(i+1)*(2*k)+j]=0;
			}
		}
		inplace_fmt(p,false);
		rep(i,4*s)p[i]*=qbuf[lv][i];
		inplace_fmt(p,true);
		rng(i,2*s,4*s)p[i]=0;
	}
	p.resize(2*s);
	rein(p);
	rep(i,s)p[i]=p[i*2];
	p.resize(s);
	rein(p);
	p.resize(n);
	return p;
}

//次数 d の頂点の重みが w[d] のとき，n 頂点の木の重みの総和はいくつか
//w のサイズが n+1 だとすると，サイズ n+2 の配列を返す
//w[0]=0 を仮定する
//そのため res[0]=res[1]=0 になることに注意
vc<mint> degree_weighted_tree(vc<mint> w){
	assert(w[0]==0);
	int n=si(w)-1;
	if(n==0)return {0,0};
	assert(w[1]!=0);
	Poly<mint> f(n);
	rep(i,n)f[i]=w[i+1]*finv[i];
	Poly<mint> g=f.inv(n)<<1;
	Poly<mint> h=compositional_inverse(g);
	Poly<mint> res=h.square().low(n+2)/2;
	rng(i,2,n+2)res[i]*=i*fact[i-2];
	return res;
}


//si(a)==si(b)==n
//for each 0<=x<n
//[x^i] a*b^i
vc<mint> pseudo_power_projection(Poly<mint> a,Poly<mint> b){
	int n=si(a);
	assert(si(b)==n);
	auto c=b.pow(n,n-1,invs);
	c*=a;c.resize(n);
	auto d=b.inv(n);
	d<<=1;
	d.resize(n);
	return reout(power_projection(c,d));
}

//mod任意
/*
//[x^{n-1}] a*b^i (0<=i<n)
//b[0]=0 を仮定
vc<mint> power_projection(vc<mint> a,vc<mint> b){
	int n=si(a);
	assert(si(b)==n);
	assert(b[0]==0);
	int s=minp2(n);
	a<<=(s-n);
	b.resize(s);
	
	vc<mint> p(2*s),q(2*s);
	rep(i,s)p[i*2]=a[i];
	rep(i,s)q[i*2]=-b[i];
	q[1]=1;
	for(int k=1;k<s;k*=2){
		auto r=q;
		for(int i=1;i<s/k;i+=2)
			rep(j,k)r[i*(2*k)+j]*=-1;
		chmult(p,r,2*s);
		for(int i=0;i<s/k;i+=2){
			rep(j,2*k){
				p[i*(2*k)+j]=p[(i+1)*(2*k)+j];
				p[(i+1)*(2*k)+j]=0;
			}
		}
		if(2*k<s)chmult(q,r,2*s);
	}
	
	p.resize(s);
	rein(p);
	p.resize(n);
	return p;
}

//https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large
//N=2^17 で 1000ms くらい
//a[0]==0
//a[1]!=0
vc<mint> compositional_inverse(vc<mint> a){
	int n=si(a);
	assert(n>=2);
	assert(a[0]==0);
	assert(a[1]!=0);
	mint a1inv=a[1].inv();
	a*=a1inv;
	
	vc<mint> tmp(n);
	tmp[0]=1;
	auto pp=power_projection(tmp,a);
	rng(k,1,n)pp[k]*=invs[k]*(n-1);
	rein(pp);
	pp.resize(n-1);
	auto b=((Poly<mint>)pp).pow_const1(-invs[n-1],invs);
	b<<=1;
	
	mint cur=1;
	rep(i,n){
		b[i]*=cur;
		cur*=a1inv;
	}
	return b;
}

//https://judge.yosupo.jp/problem/composition_of_formal_power_series_large
//N=2^17 で 900ms くらい
//n=si(a)=si(b)
//returns a(b(x)) of size n
//b[0]=0 を仮定
vc<mint> composition(vc<mint> a,vc<mint> b){
	int n=si(a);
	assert(si(b)==n);
	assert(b[0]==0);
	int s=minp2(n);
	a.resize(s);
	rein(a);
	b.resize(s);
	
	int L=__lg(s);
	vvc<mint> qbuf(L);
	vc<mint> q(2*s);
	rep(i,s)q[i*2]=-b[i];
	q[1]=1;
	for(int k=1,lv=0;k<s;k*=2,lv++){
		auto r=q;
		for(int i=1;i<s/k;i+=2)
			rep(j,k)r[i*(2*k)+j]*=-1;
		qbuf[lv]=r;
		if(2*k<s)chmult(q,r,2*s);
	}
	
	auto p=a;p.resize(2*s);
	rein(p);
	for(int k=s/2,lv=L-1;k>=1;k/=2,lv--){
		for(int i=0;i<s/k;i+=2){
			rep(j,2*k){
				p[i*(2*k)+j]=p[(i+1)*(2*k)+j];
				p[(i+1)*(2*k)+j]=0;
			}
		}
		chmult(p,qbuf[lv],2*s);
	}
	rein(p);
	rep(i,s)p[i]=p[i*2];
	p.resize(s);
	rein(p);
	p.resize(n);
	return p;
}*/
