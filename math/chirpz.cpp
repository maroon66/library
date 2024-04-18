//res: 長さ m
//res[i]= sum f[j] * w^(ij)
//ABC278H
vc<mint> chirpz(vc<mint> f,mint w,int m){
	int n=si(f);
	vc<mint> a(n+m-1),b(max(n,m));
	rep(i,si(a))a[i]=w.pow(i*(i-1)/2);
	rep(i,si(b))b[i]=a[i].inv();
	rep(i,n)f[i]*=b[i];
	reverse(all(f));
	int s=1;while(s<si(a))s*=2;
	f.resize(s);
	a.resize(s);
	f=multiply_givenlength(move(f),a);
	vc<mint> g(m);
	rep(i,m)g[i]=f[n-1+i]*b[i];
	return g;
}
