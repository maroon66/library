//CF453E
//further optimization is possible
template<class mint>
struct bluestein{
	int n;
	bool inv;
	vc<mint> w;
	vc<mint> wfmt;
	bluestein(int nn,bool ii):n(nn),inv(ii),w(n){
		assert(mint::mod%(2*n)==1);
		mint z=mint::root().pow((mint::mod-1)/(2*n));
		if(inv)z=z.inv();
		rep(i,n)w[i]=z.pow(i*i);
		wfmt.resize(2*n-1);
		rep(i,n)wfmt[n-1+i]=w[i].inv();
		rng(i,1,n)wfmt[n-1-i]=w[i].inv();
	}
	vc<mint> transform(vc<mint> x){
		assert(si(x)==n);
		rep(i,n)x[i]*=w[i];
		auto y=arbitrary_convolution::multiply(x,wfmt);
		vc<mint> z(n);
		rep(i,n)z[i]=y[n-1+i]*w[i];
		if(inv){
			mint a=mint(1)/n;
			rep(i,n)z[i]*=a;
		}
		return z;
	}
};
