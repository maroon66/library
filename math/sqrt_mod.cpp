ll norm_mod(ll a,ll p){
	a%=p;
	if(a<0)a+=p;
	return a;
}

//p: odd (not necessarily prime)
//10^18 サイズの入力で動くはず（未検証）
ll jacobi(ll a,ll p){
	a=norm_mod(a,p);
	auto neg=[](ll x){return x%2?-1:1;};
	if(a==0) return p==1;
	else if(a%2) return neg(((p-1)&(a-1))>>1)*jacobi(p%a,a);
	else return neg(((p&15)*(p&15)-1)/8)*jacobi(a/2,p);
}

//yosupo sqrt mod (work without define int ll)
//LOJ 150
//p: prime
//0<=a<p
//returns minimum solution
ll sqrt_mod(ll a,ll p){
	if(p==2)return a%2;
	if(a==0)return 0;
	if(jacobi(a,p)==-1)return -1;
	ll b,c;
	do{
		b=rand_int(0,p-1);
		c=norm_mod(b*b-a,p);
	}while(jacobi(c,p)!=-1);
	auto mul=[&](pair<ll,ll> x,pair<ll,ll> y){
		return pi(norm_mod(x.a*y.a+x.b*y.b%p*c,p),norm_mod(x.a*y.b+x.b*y.a,p));
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

//TODO: p=2
//CF453E
//ax^2+bx+c=0
//a!=0
template<class mint>
vc<mint> quadratic_equation(mint a,mint b,mint c){
	mint d=sq(b)-4*a*c;
	/*if(p==2){
		if(a==b){
			if(c==0)return mp(0,1);
			else return mp(-1,-1);
		}else{
			return mp(c,-1);
		}
	}*/
	ll z=sqrt_mod(d.v,mint::mod);
	if(z==-1)return {};
	vc<mint> res;
	rep(k,2){
		res.pb((-b+z)/(2*a));
		if(z==0)break;
		z=mint::mod-z;
	}
	return res;
}

//CF586
//あまり変なケースはないっぽい？
vc<pair<mint,mint>> cross_circle(mint x1,mint y1,mint z1,mint x2,mint y2,mint z2){
	mint a=x2-x1,b=y2-y1;
	dmp2(a,b,z1,z2);
	bool rev=false;
	if(a==0){
		rev=true;
		swap(a,b);
	}
	if(a==0){
		assert(z1!=z2);
		return {};
	}
	//(p-a)^2+(q-b)^2=z2
	//p^2+q^2=z1
	//z1+a^2+b^2-z2=2(ap+bq)
	mint w=(z1+a*a+b*b-z2)/2;
	//(ap)^2+(aq)^2=(w-bq)^2+(aq)^2=a^2z1
	auto qs=quadratic_equation(a*a+b*b,-2*b*w,w*w-a*a*z1);
	vc<pair<mint,mint>> res;
	for(auto q:qs){
		mint p=(w-b*q)/a;
		if(rev)res.eb(q,p);
		else res.eb(p,q);
	}
	for(auto&t:res){
		t.a+=x1;
		t.b+=y1;
	}
	return res;
}

//yosupo sqrt_of_formal_power_series
//returns {} if the answer does not exist
//otherwise returns a one with smaller constant
template<class mint>
Poly<mint> sqrt_poly(Poly<mint> f){
	int n=si(f),z=0;
	for(;z<n&&f[z]==0;z++);
	if(z==n)
		return {0};
	if(z%2)
		return {};
	int w=sqrt_mod(f[z].v,mint::mod);
	if(w==-1)
		return {};
	int s=n-z;
	auto g=(Poly<mint>(f.bg+z,f.ed)/f[z]).sqrt(s)*w;
	z/=2;
	Poly<mint> h(z+s);
	rep(i,s)h[z+i]=g[i];
	return h;
}
