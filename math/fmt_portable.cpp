//Multiuni 2022 Day2 E
//2019 Summer Petrozavodsk Camp, Day 8: XIX Open Cup Onsite A
const mint prim_root = 3;
const mint prim_root_inv=prim_root.inv();
void inplace_fmt(vc<mint>&f, const bool inv){
	const int n = f.size();
	const mint root = inv?prim_root_inv:prim_root;
	static vc<mint>g;
	g.clear();g.resize(n);
	
	for(int b=n/2;b>=1;b/=2){
		mint w=root.pow((mod-1)/(n/b)), p = 1;
		for(int i=0;i<n;i+=b*2){
			rep(j,b){
				f[i+j+b]*=p;
				g[i/2+j]=f[i+j]+f[i+b+j];
				g[n/2+i/2+j]=f[i+j]-f[i+b+j];
			}
			p*=w;
		}
		swap(f, g);
	}
	if(inv){
		mint z=mint(n).inv();
		rep(i,n)f[i]*=z;
	}
}

vector<mint> multiply(vector<mint> a,vector<mint> b){
	int n=a.size(),m=b.size();
	assert(n>0&&m>0);
	int s=1;
	while(s<n+m-1)s*=2;
	a.resize(s);
	inplace_fmt(a,false);
	b.resize(s);
	inplace_fmt(b,false);
	rep(i,s)a[i]*=b[i];
	inplace_fmt(a,true);
	a.resize(n+m-1);
	return a;
}
