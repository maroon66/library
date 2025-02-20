//df/dx = a(x)*f(x)+b(x) を解く
//[x^0]f = z とする
//si(f)=n で返す
//stress-tested
Poly<mint> dfdx_linear(int n,const Poly<mint>&a,const Poly<mint>&b,mint z){
	assert(n>=1);
	if(n==1)return {z};
	auto [c,d]=a.low(n-1).inte(invs).exp2(n,invs);
	auto e=d*b.low(n-1);e.resize(n);
	gnr(i,1,n)e[i]=e[i-1]*invs[i];
	e[0]=z;
	auto f=c*e;f.resize(n);
	return f;
}

//df/dx = a(x)*f^2(x)+b(x)*f(x)+c(x) を解く
//[x^0]f = z とする
//si(f)=n で返す
//stress-tested
//N=250000 で 0.7sec 程度
Poly<mint> dfdx_quadratic(int n,const Poly<mint>&a,const Poly<mint>&b,const Poly<mint>&c,mint z){
	assert(n>=1);
	if(n==1)return {z};
	Poly<mint> f{z};
	while(si(f)<n){
		int s=si(f)*2;
		auto g=f*a.low(s-1);g.resize(s-1);
		Poly<mint> u(s-1),v(s-1);
		//u+=k*a*f^(k-1)
		rep(i,s-1)u[i]+=2*g[i];
		//u+=b
		rep(i,min(s-1,si(b)))u[i]+=b[i];
		//v+=(1-k)*a*f^k
		v+=(f*g).low(s-1)*(1-2);
		//v+=c
		rep(i,min(s-1,si(c)))v[i]+=c[i];
		f=dfdx_linear(s,u,v,z);
	}
	f.resize(n);
	return f;
}
