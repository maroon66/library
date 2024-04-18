//n,m: powers of 2
//[0-m-1],[m,2*m-1]...[nm-m,nm-1]
void inplace_fmt_2d(vc<mint>&f,int n,int m,bool inv){
	rep(i,n)inplace_fmt(m,&f[i*m],inv);
	vc<mint> tmp(n);
	rep(j,m){
		rep(i,n)tmp[i]=mint(f[i*m+j].v);
		inplace_fmt(tmp,inv);
		rep(i,n)f[i*m+j]=mint(tmp[i].v);
	}
}
