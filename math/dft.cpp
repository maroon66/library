vc<mint> dft(const vc<mint>&a,const mint w){
	int n=si(a);
	vc<mint> b(n);
	mint z=1;
	rep(i,n){
		mint cur=1;
		rep(j,n){
			b[i]+=a[j]*cur;
			cur*=z;
		}
		z*=w;
	}
	return b;
}
