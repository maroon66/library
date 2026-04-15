//ARC163F
//res[i]=v^i (0<=i<=n)
vc<mint> pows(mint v,int n){
	vc<mint> res(n+1);
	res[0]=1;
	rep(i,n)res[i+1]=res[i]*v;
	return res;
}
//ARC163F
//res[i]=sum_{0<=v<m} v^i (0<=i<n)
vc<mint> powsum(int m,int n){
	Poly<mint> num=pows(m,n+1);
	rep(i,n)num[i]=-num[i+1]*finv[i+1];
	num.resize(n);
	Poly<mint> den(n);
	rep(i,n)den[i]=-finv[i+1];
	auto res=num*den.inv(n);
	res.resize(n);
	rep(i,n)res[i]*=fact[i];
	return res;
}
vc<mint> expVx(mint v,int n){
	auto res=pows(v,n);
	rep(i,n+1)res[i]*=finv[i];
	return res;
}
//[x^deg(a)] a*b^i を i=0,...,n で計算
vc<mint> power_projection_with_const(const vc<mint>&a,vc<mint> b,int n){
	assert(si(a)==si(b));
	mint c=b[0];
	b[0]=0;
	auto f=power_projection(a,b);
	rep(i,si(f))f[i]*=finv[i];
	chmult(f,expVx(c,n),n+1);
	rep(i,n+1)f[i]*=fact[i];
	return f;
}
//composition の結果
//deg(b) まで
vc<mint> composition_with_const(vc<mint> a,vc<mint> b){
	mint c=b[0];
	b[0]=0;
	int n=si(a)-1,m=si(b)-1;
	rep(i,n+1)a[i]*=fact[i];
	rein(a);
	chmult(a,expVx(c,n),n+1);
	rein(a);
	rep(i,n+1)a[i]*=finv[i];
	a.resize(m+1);
	return composition(a,b);
}
