
template<class t=ll>
t extgcd(t a,t b,t&x,t&y){
	if(b==0){
		x=1;
		y=0;
		return a;
	}else{
		t g=extgcd(b,a%b,y,x);
		y-=a/b*x;
		return g;
	}
}

//https://codeforces.com/blog/entry/98376
//https://codeforces.com/gym/361606/problem/A
struct linear_basis{
	int n;
	vvc<mint> a;
	linear_basis(int nn):n(nn),a(n,vc<mint>(n)){}
	//x+=y*z
	//first off elements of y are zero
	void add_prod(vc<mint>&x,const vc<mint>&y,mint z,int off){
		rng(i,off,n)x[i]+=y[i]*z;
	}
	void add(vc<mint> v){
		rep(i,n)if(v[i]){
			ll z=a[i][i]?a[i][i].v:mint::mod;
			ll s,t;
			ll g=extgcd<ll>(v[i].v,z,s,t);
			mint x=z/g,y=-mint(v[i].v/g);
			rng(j,i,n)tie(a[i][j],v[j])=mp(v[j]*s+a[i][j]*t,v[j]*x+a[i][j]*y);
		}
	}
	bool spanned(vc<mint> v){
		rep(i,n)if(v[i]){
			if(a[i][i]==0||v[i].v%a[i][i].v)return false;
			add_prod(v,a[i],-mint(v[i].v/a[i][i].v),i);
		}
		return true;
	}
	vc<mint> lexmax(){
		vc<mint> v(n);
		rep(i,n)if(a[i][i])add_prod(v,a[i],(mint::mod-1-v[i].v)/a[i][i].v,i);
		return v;
	}
	mint107 count_spanned(){
		mint107 ans=1;
		rep(i,n)if(a[i][i])ans*=mint::mod/a[i][i].v;
		return ans;
	}
};
