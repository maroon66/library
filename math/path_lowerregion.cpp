//The 13th Chinese Northeast Collegiate Programming Contest I

vc<mint> hori(vc<mint>x,int h){
	int w=si(x)-1;
	vc<mint>y(w+1);
	rep(i,w+1)y[i]=binom(i,h);
	auto z=multiply(x,y);
	z.resize(w+1);
	return z;
}

vc<mint> vert(vc<mint>x,int h){
	int w=si(x)-1;
	rep(i,w+1)x[i]*=finv[w-i];
	vc<mint> y(fact,fact+(w+h+1));
	auto z=multiply(x,y);
	vc<mint> res(h+1);
	rep(i,h+1)res[i]=z[w+i]*finv[i];
	return res;
}

// 0<=a[0]<=a[1]...
// (i,a[i])->(i+1,a[i]) で上に蓋をされているような経路数を求める
// グリッドの高さが h で，長さ h+1 の数列を返す
vc<mint> advance(vi a,int h,vc<mint> up){
	{
		int cnt=0;
		rep(i,si(a))if(a[i]<0)cnt++;else break;
		a.erase(a.bg,a.bg+cnt);
		up.erase(up.bg,up.bg+cnt);
	}
	int n=si(a);
	assert(si(up)==n+1);
	if(n<=250||h<=250){
		vc<mint> res(h+1);
		rep(i,h+1){
			rep(j,n)if(a[j]>=i){
				up[j+1]+=up[j];
			}
			res[i]=up[n];
		}
		return res;
	}
	int t=(n+h)/2;
	int x=0,y=0;
	while(x+y<t){
		if(x==n||y<a[x])y++;
		else x++;
	}
	vc<mint> res(h+1);
	vc<mint> nx(n-x+1);
	{
		auto ue=hori(vc<mint>(x+all(up)),y);
		rep(i,n-x+1)nx[i]=ue[i];
	}
	{
		auto migi=vert(vc<mint>(x+all(up)),y);
		rep(i,y+1)res[i]+=migi[i];
	}
	if(x){
		auto hidari=advance(vi(a.bg,a.bg+x-1),a[x-1],vc<mint>(up.bg,up.bg+x));
		hidari.resize(y+1);
		{
			auto ue=vert(hidari,n-x);
			rep(i,n-x+1)nx[i]+=ue[i];
		}
		{
			auto migi=hori(hidari,n-x);
			rep(i,y+1)res[i]+=migi[i];
		}
	}
	if(y<h){
		vi b(x+all(a));
		for(auto&v:b)v-=y+1;
		auto migi=advance(b,h-y-1,nx);
		rep(i,h-y)res[y+1+i]=migi[i];
	}
	return res;
}

void slv(){
	int n;cin>>n;
	vi a(n);
	rep(i,n){
		cin>>a[i];
		a[i]--;
	}
	
	vc<mint> up(n+1);
	up[0]=1;
	auto ans=advance(a,a[n-1],up);
	cout<<ans[a[n-1]].v<<"\n";
}
