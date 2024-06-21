//CF947G
int find_min_match(const string&str,const string&t){
	if(si(str)>si(t))return -1;
	
	int n=si(str);
	int m=si(t);
	
	vc<bool> ok(m-n+1,true);
	const int s=minp2(m);
	vc<mint> x(n);
	vc<mint> y(m);
	
	rep(phase,2){
		static mint rv[26];
		rep(k,26)rv[k]=rand_int(mint::mod);
		auto tomint=[&](char c)->mint{
			assert(islower(c)||c=='-');
			if(c=='-')return 0;
			else return rv[c-'a'];
		};
		
		rep(i,n)x[i]=tomint(str[n-1-i]);
		rep(i,m)y[i]=tomint(t[i]);
		
		vc<mint> d(m-n+1);
		vc<mint> a(s);
		vc<mint> b(s);
		vc<mint> c(s);
		{
			fila(a,0);
			fila(b,0);
			rep(i,n)a[i]=x[i]*x[i]*x[i];
			rep(i,m)b[i]=y[i];
			
			inplace_fmt(a,false);
			inplace_fmt(b,false);
			rep(i,s)c[i]+=a[i]*b[i];
		}
		{
			fila(a,0);
			fila(b,0);
			rep(i,n)a[i]=x[i]*x[i]*(-2);
			rep(i,m)b[i]=y[i]*y[i];
			
			inplace_fmt(a,false);
			inplace_fmt(b,false);
			rep(i,s)c[i]+=a[i]*b[i];
		}
		{
			fila(a,0);
			fila(b,0);
			rep(i,n)a[i]=x[i];
			rep(i,m)b[i]=y[i]*y[i]*y[i];
			
			inplace_fmt(a,false);
			inplace_fmt(b,false);
			rep(i,s)c[i]+=a[i]*b[i];
		}
		
		inplace_fmt(c,true);
		rep(i,m-n+1)if(c[n-1+i])ok[i]=false;
	}
	rep(i,m-n+1)if(ok[i])return i;
	return -1;
}
