//1 to n inclusive

mint sump0(int n){
	return n;
}

mint sump1(int n){
	int a=n,b=n+1;
	if(a%2==0)a/=2;
	else b/=2;
	return mint(a)*b;
}

mint sump2(int n){
	int a=n,b=n+1,c=n*2+1;
	if(a%2==0)a/=2;
	else b/=2;
	if(a%3==0)a/=3;
	else if(b%3==0)b/=3;
	else c/=3;
	return mint(a)*b*c;
}

//1-n までの k 乗和
mint sump(int k,int n){
	if(k==0)return sump0(n);
	if(k==1)return sump1(n);
	if(k==2)return sump2(n);
	assert(0);
}

//inclusive
mint sump(int k,int l,int r){
	return sump(k,r)-sump(k,l-1);
}

//initfact
//res[i]=1^i+2^i+...n^i
//si(res)=k
vc<mint> getlist(int n,int k){
	Poly<mint> g(k);
	mint cur=1;
	rep(i,k){
		cur*=n+1;
		g[i]=cur*finv[i+1];
	}
	Poly<mint> h(k);
	rep(i,k){
		h[i]=finv[i+1];
	}
	auto f=g*h.inv(k);
	f.resize(k);
	rep(i,k)f[i]*=fact[i];
	f[0]-=1;
	return f;
}
