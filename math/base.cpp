ll fld(ll a, ll b) { // floored division
	return a / b - ((a ^ b) < 0 && a % b); }
ll cld(ll a, ll b) { // ceiled division
	return a / b + ((a ^ b) > 0 && a % b); }
ll rem(ll a,ll b){
	return a-fld(a,b)*b;}

ll rem(ll a,ll b){
	ll res=a%b;
	return res<0?res+b:res;
}

ll cap(ll a,ll b){
	return b==0?0:a<=inf/b?a*b:inf;
}

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

//x*y=g mod m
//returns (g,y)
//x=0 -> (m,0) (未確認)
template<class t=ll>
pair<t,t> modinv(t x,t m){
	t a,b;
	t g=extgcd(x,m,a,b);
	if(a<0)a+=m/g;
	return {g,a};
}

//x = a mod b
//x = c mod d
// => x = p mod q
//returns (-1,-1) when infeasible
pi crt(int a,int b,int c,int d){
	if(b<d){
		swap(a,c);
		swap(b,d);
	}
	c%=d;
	int g,e;
	tie(g,e)=modinv(b,d);
	int k=(c-a);
	if(k%g)return pi(-1,-1);
	k/=g;
	int m=b/g*d;
	int x=(a+k*e%(d/g)*b)%m;
	if(x<0)x+=m;
	return pi(x,m);
}

//yukicoder no.187
int garner(vc<pi> z,int m){
	int ans=0,w=1;
	rep(i,si(z)){
		int x=z[i].a;
		ans=(ans+w*x)%m;
		rng(j,i+1,si(z))
			z[j].a=(z[j].a+z[j].b-x%z[j].b)*modinv(z[i].b,z[j].b).b%z[j].b;
		w=(w*z[i].b)%m;
	}
	return ans;
}

//AOJ2387
//find minimum x s.t. lw <= h*x mod w <=up
//return h*x mod w
//or -1 if impossible
int waf(int h,int w,int lw,int up){
	assert(0<=lw);
	assert(lw<=up);
	assert(up<=w);
	if(lw==0)return 0;
	if(h==0)return -1;
	int need=(lw+h-1)/h;
	int p=need*h;
	if(inc(lw,p,up)){
		return p;
	}
	int res=waf(w%h,h,p-up,p-lw);
	if(res==-1)return -1;
	return p-res;
}

//AOJ2387
//find minimum x s.t. lw <= h*x mod w <=up
//return 0<=x<w/gcd(h,w)
//or inf if impossible
int find_min(int h,int w,int lw,int up){
	int res=waf(h,w,lw,up);
	if(res!=-1){
		auto [g,y]=modinv(h,w);
		res=res/g*y%(w/g);
	}else{
		res=inf;
	}
	return res;
}

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

int mult_mod(int a,int b,int m){
	auto add=[&](int&x,int y){
		x+=y;
		if(x>=m)x-=m;
	};
	int res=0;
	while(b){
		if(b&1)add(res,a);
		add(a,a);
		b>>=1;
	}
	return res;
}

//https://hydro.ac/d/luogu/p/P8006
ll ab_eq_c(ll a,ll b,ll c){
	//0<=a'<=a
	//0<=b'<=b
	//a'+b'=c
	assert(0<=a);
	assert(0<=b);
	assert(0<=c);
	assert(c<=a+b);//TODO はずす
	return c+1-max<ll>(c-a,0)-max<ll>(c-b,0);
}
ll c2(ll n){
	return n*(n+1)/2;
}
ll ab_leq_c(ll a,ll b,ll c){
	//0<=a'<=a
	//0<=b'<=b
	//a'+b'<=c
	assert(0<=a);
	assert(0<=b);
	assert(0<=c);
	assert(c<=a+b);//TODO はずす
	return c2(c+1)-c2(max<ll>(c-a,0))-c2(max<ll>(c-b,0));
}
