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
