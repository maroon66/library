template<class t>
t pow_mod(t x,t n,t m){
	t r=1;
	while(n){
		if(n&1)r=(r*x)%m;
		x=(x*x)%m;
		n>>=1;
	}
    return r;
}

ll norm_mod_64(int128 a,ll p){
	a%=p;
	if(a<0)a+=p;
	return a;
}

//p: odd (not necessarily prime)
//10^18 サイズの入力で動くはず（未検証）
ll jacobi(ll a,ll p){
	a=norm_mod_64(a,p);
	auto neg=[](ll x){return x%2?-1:1;};
	if(a==0) return p==1;
	else if(a%2) return neg(((p-1)&(a-1))>>1)*jacobi(p%a,a);
	else return neg(((p&15)*(p&15)-1)/8)*jacobi(a/2,p);
}

//mod が 64bit でも動くぜ
//yosupo sqrt mod (work without define int ll)
//LOJ 150
//p: prime
//0<=a<p
//returns minimum solution
ll sqrt_mod_64(ll a,ll p){
	if(p==2)return a%2;
	if(a==0)return 0;
	if(jacobi(a,p)==-1)return -1;
	ll b,c;
	do{
		b=rand_int(0,p-1);
		c=norm_mod_64(sq<int128>(b)-a,p);
	}while(jacobi(c,p)!=-1);
	auto mul=[&](pair<ll,ll> x,pair<ll,ll> y){
		return pi(
			norm_mod_64(int128(x.a)*y.a+int128(x.b)*y.b%p*c,p),
			norm_mod_64(int128(x.a)*y.b+int128(x.b)*y.a,p));
	};
	pair<ll,ll> x(b,1),cur(1,0);
	ll n=(p+1)/2;
	while(n){
		if(n&1)cur=mul(cur,x);
		x=mul(x,x);
		n>>=1;
	}
	assert(cur.b==0);
	return min(cur.a,p-cur.a);
}

//mod が 64bit でも動くぜ
//CF453E
//NERC2020 H
//ax^2+bx+c=0
//a!=0
pair<ll,ll> quadratic_equation_64(ll a,ll b,ll c,ll p){
	a=norm_mod_64(a,p);
	b=norm_mod_64(b,p);
	c=norm_mod_64(c,p);
	if(p==2){
		if(a==b){
			if(c==0)return mp(0,1);
			else return mp(-1,-1);
		}else{
			return mp(c,-1);
		}
	}
	ll d=norm_mod_64(sq<int128>(b)-int128(4)*a*c,p);
	ll z=sqrt_mod_64(d,p);
	if(z==-1)return mp(-1,-1);
	int128 den=pow_mod<int128>(2*a,p-2,p);
	if(z==0)return mp(norm_mod_64(-b*den,p),-1);
	else return mp(
		norm_mod_64((-b+z)*den,p),
		norm_mod_64((-b-z)*den,p));
}
