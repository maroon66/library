struct BS{
	int n;
	vc<ull> x;
	BS(int nn):n(nn),x((n+63)/64){}
	BS(const vi&ini):n(si(ini)),x((n+63)/64){
		rep(i,n)set(i,ini[i]);
	}
	BS(const string&ini):n(si(ini)),x((n+63)/64){
		rep(i,n){
			assert(ini[i]=='0'||ini[i]=='1');
			set(i,ini[i]-'0');
		}
	}
	//サイズ固定，全然速くない，なんで？
	/*array<ull,ten(5)/64+10> x;
	BS(int nn):n(nn),x{}{}
	BS(const vi&ini):n(si(ini)),x{}{
		rep(i,n)set(i,ini[i]);
	}
	BS(const string&ini):n(si(ini)),x{}{
		rep(i,n){
			assert(ini[i]=='0'||ini[i]=='1');
			set(i,ini[i]-'0');
		}
	}*/
	void set(int i){
		assert(inc(0,i,n-1));
		x[i/64]|=1ull<<(i&63);
	}
	void reset(int i){
		assert(inc(0,i,n-1));
		x[i/64]&=~(1ull<<(i&63));
	}
	void set(int i,bool v){
		assert(inc(0,i,n-1));
		if(v)set(i);
		else reset(i);
	}
	void flip(int i){
		assert(inc(0,i,n-1));
		x[i/64]^=1ull<<(i&63);
	}
	/* .x[i] と間違えやすいので封印
	bool operator[](int i)const{
		assert(inc(0,i,n-1));
		return x[i/64]&1ull<<(i&63);
	}*/
	bool get(int i)const{
		assert(inc(0,i,n-1));
		return x[i/64]&1ull<<(i&63);
	}
	int _Find_first()const{
		rep(i,si(x))if(si(x)){
			return i*64+__builtin_ctzll(x[i]);
		}
		return n;
	}
	BS& operator^=(const BS&rhs){
		assert(n==rhs.n);
		rep(i,si(x))x[i]^=rhs.x[i];
		return *this;
	}
	BS operator^(const BS&rhs)const{
		assert(n==rhs.n);
		return BS(*this)^=rhs;
	}
	bool operator==(const BS&rhs)const{
		assert(n==rhs.n);
		return x==rhs.x;
	}
	bool operator<(const BS&rhs)const{
		assert(n==rhs.n);
		return x<rhs.x;
	}
	bool any()const{
		rep(i,si(x))if(x[i])return true;
		return false;
	}
	//[l,r) を w[to,to+(r-l)) に xor する
	//stress-tested
	void window_xor(int l,int r,BS&w,int to)const{
		assert(0<=l&&l<=r&&r<=n);
		assert(to+(r-l)<=w.n);
		if(l==r)return;
		
		int L=to,R=to+(r-l);
		int dif=l-L;
		int q=dif&63,p=(dif-q)/64;
		
		auto single=[&](int i,int b,int e){
			assert(0<=b&&b<=e&&e<=64);
			if(b==e)return;
			ull t=0;
			if(b+q<64)t|=x[i+p]>>q;
			if(e+q>64)t|=x[i+p+1]<<(64-q);
			w.x[i]^=t&umask(b,e);
		};
		
		int u=(L+63)/64,v=R/64;
		if(u<=v){
			single(u-1,L-(u-1)*64,64);
			if(q==0){
				rng(i,u,v){
					w.x[i]^=x[i+p];
				}
			}else{
				rng(i,u,v){
					w.x[i]^=(x[i+p]>>q)|(x[i+p+1]<<(64-q));
				}
			}
			single(v,0,R-v*64);
		}else{
			assert(v==u-1);
			single(u-1,L-(u-1)*64,R-v*64);
		}
	}
	//[l,r) を w[to,to+(r-l)) に and する
	//stress-tested
	void window_and(int l,int r,BS&w,int to)const{
		assert(0<=l&&l<=r&&r<=n);
		assert(to+(r-l)<=w.n);
		if(l==r)return;
		
		int L=to,R=to+(r-l);
		int dif=l-L;
		int q=dif&63,p=(dif-q)/64;
		
		auto single=[&](int i,int b,int e){
			assert(0<=b&&b<=e&&e<=64);
			if(b==e)return;
			ull t=0;
			if(b+q<64)t|=x[i+p]>>q;
			if(e+q>64)t|=x[i+p+1]<<(64-q);
			w.x[i]&=t|~umask(b,e);
		};
		
		int u=(L+63)/64,v=R/64;
		if(u<=v){
			single(u-1,L-(u-1)*64,64);
			if(q==0){
				rng(i,u,v){
					w.x[i]&=x[i+p];
				}
			}else{
				rng(i,u,v){
					w.x[i]&=(x[i+p]>>q)|(x[i+p+1]<<(64-q));
				}
			}
			single(v,0,R-v*64);
		}else{
			assert(v==u-1);
			single(u-1,L-(u-1)*64,R-v*64);
		}
	}
	//[l,r) を w[to,to+(r-l)) に or する
	//stress-tested
	void window_or(int l,int r,BS&w,int to)const{
		assert(0<=l&&l<=r&&r<=n);
		assert(to+(r-l)<=w.n);
		if(l==r)return;
		
		int L=to,R=to+(r-l);
		int dif=l-L;
		int q=dif&63,p=(dif-q)/64;
		
		auto single=[&](int i,int b,int e){
			assert(0<=b&&b<=e&&e<=64);
			if(b==e)return;
			ull t=0;
			if(b+q<64)t|=x[i+p]>>q;
			if(e+q>64)t|=x[i+p+1]<<(64-q);
			w.x[i]|=t&umask(b,e);
		};
		
		int u=(L+63)/64,v=R/64;
		if(u<=v){
			single(u-1,L-(u-1)*64,64);
			if(q==0){
				rng(i,u,v){
					w.x[i]|=x[i+p];
				}
			}else{
				rng(i,u,v){
					w.x[i]|=(x[i+p]>>q)|(x[i+p+1]<<(64-q));
				}
			}
			single(v,0,R-v*64);
		}else{
			assert(v==u-1);
			single(u-1,L-(u-1)*64,R-v*64);
		}
	}
	//デバッグ用
	//N以降のbitは常に0を入れることを保証する
	void checktail0()const{
		rep(i,si(x))rep(j,64)if(i*64>=n){
			assert((x[i]&1ull<<j)==0);
		}
	}
};
//Stanford Local Programming Contest 2026 H
void imos(ull&a,int&carry){
	a^=carry;
	carry=__builtin_parityll(a);
	a^=a<<(1<<0);
	a^=a<<(1<<1);
	a^=a<<(1<<2);
	a^=a<<(1<<3);
	a^=a<<(1<<4);
	a^=a<<(1<<5);
}
