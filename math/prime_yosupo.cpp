ll gcd(ll a,ll b){return b?gcd(b,a%b):a;}

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

bool is_prime(ll n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    ll d = n - 1;
    while (d % 2 == 0) d /= 2;
    for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n <= a) break;
        ll t = d;
        ll y = pow_mod<__int128_t>(a, t, n);  // over
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = __int128_t(y) * y % n;  // flow
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}

ll pollard_single(ll n) {
    auto f = [&](ll x) { return (__int128_t(x) * x + 1) % n; };
    if (is_prime(n)) return n;
    if (n % 2 == 0) return 2;
    ll st = 0;
    while (true) {
        st++;
        ll x = st, y = f(x);
        while (true) {
            ll p = gcd((y - x + n), n);
            if (p == 0 || p == n) break;
            if (p != 1) return p;
            x = f(x);
            y = f(f(y));
        }
    }
}

vc<ll> pollard(ll n) {
    if (n == 1) return {};
    ll x = pollard_single(n);
    if (x == n) return {x};
    vc<ll> le = pollard(x);
    vc<ll> ri = pollard(n / x);
    le.insert(le.end(), ri.begin(), ri.end());
    return le;
}

ll primitive_root(ll p) {
    auto v = pollard(p - 1);
    while (true) {
        ll g = rand_int(1, p - 1);  //[1, p-1]
    //rng(g,4,p){
        bool ok = true;
        for (auto d : v) {
            ll f = (p - 1) / d;
            if (pow_mod<__int128_t>(g, f, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

vc<pi> to_freq(vi a){
	sort(all(a));
	vc<pi> res;
	for(auto v:a){
		if(res.empty()||res.back().a!=v)res.eb(v,0);
		res.back().b++;
	}
	return res;
}

vi enum_divs(int n){
	auto f=to_freq(pollard(n));
	dmp(f);
	
	vi res;
	auto rec=[&](auto self,int i,int d)->void{
		if(i==si(f))res.pb(d);
		else{
			rep(_,f[i].b+1){
				self(self,i+1,d);
				d*=f[i].a;
			}
		}
	};
	rec(rec,0,1);
	
	return res;
}
