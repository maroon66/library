
//O(1) add, O(sqrt(N)) get
//Multiuni2023-5 A
//バッファサイズ，B を static にした
template<int S,int B>
struct ds1{
	int n,m;
	//int *a,*b;
	int a[S+10],b[S+10];
	void init(int nn){
		n=nn;
		//B=max(int(sqrtl(n)),int(1));
		m=n/B+1;
		//a=getbuf(n);
		//b=getbuf(m);
		rep(i,n)a[i]=0;
		rep(i,m)b[i]=0;
	}
	void add(int i,int v){
		assert(inc(0,i,n-1));
		a[i]+=v;
		b[i/B]+=v;
	}
	void addrng(int c,int d,int v){
		if(c==d)return;
		assert(c<d);
		add(c,v);
		if(d<n)add(d,-v);
	}
	int get(int i){
		int x=i/B;
		int res=0;
		rep(j,x)res+=b[j];
		rng(j,x*B,i+1)res+=a[j];
		return res;
	}
	/*
	not verified
	int sum(int c,int d){
		int x=c/B,y=(d+B-1)/B;
		int res=0;
		rng(i,x,y){
			int l=max(c,i*B);
			int r=min(d,(i+1)*B);
			if(l==i*B&&r==(i+1)*B) res+=b[i];
			else rng(j,l,r) res+=a[j];
		}
		return res;
	}
	*/
};
ds1<5*ten(5),1024> ds;

int buf[ten(6)];

int bufhead;
int*getbuf(int n){
	int*res=buf+bufhead;
	rep(i,n)res[i]=0;
	bufhead+=n;
	return res;
}

//O(1) add, O(sqrt(N)) get
struct ds1{
	int n,B,m;
	int *a,*b;
	void init(int nn){
		n=nn;
		B=max(int(sqrtl(n)),int(1));
		m=n/B+1;
		a=getbuf(n);
		b=getbuf(m);
	}
	void add(int i,int v){
		assert(inc(0,i,n-1));
		a[i]+=v;
		b[i/B]+=v;
	}
	void addrng(int c,int d,int v){
		if(c==d)return;
		assert(c<d);
		add(c,v);
		if(d<n)add(d,-v);
	}
	int get(int i){
		int x=i/B;
		int res=0;
		rep(j,x)res+=b[j];
		rng(j,x*B,i+1)res+=a[j];
		return res;
	}
	/*
	not verified
	int sum(int c,int d){
		int x=c/B,y=(d+B-1)/B;
		int res=0;
		rng(i,x,y){
			int l=max(c,i*B);
			int r=min(d,(i+1)*B);
			if(l==i*B&&r==(i+1)*B) res+=b[i];
			else rng(j,l,r) res+=a[j];
		}
		return res;
	}
	*/
};

//O(sqrt(N)) add, O(1) get
struct ds2{
	int n,B,m;
	int *a,*b;
	void init(int nn){
		n=nn;
		B=max(int(sqrtl(n)),int(1));
		m=n/B+1;
		a=getbuf(n);
		b=getbuf(m);
	}
	void add(int i,int v){
		assert(inc(0,i,n-1));
		int x=i/B;
		rng(j,i,min(n,(x+1)*B))a[j]+=v;
		rng(j,x,m)b[j]+=v;
	}
	void addrng(int c,int d,int v){
		if(c==d)return;
		assert(c<d);
		add(c,v);
		if(d<n)add(d,-v);
	}
	int get(int i){
		if(i<0)return 0;
		int x=i/B;
		int res=a[i];
		if(x)res+=b[x-1];
		return res;
	}
	int sum(int c,int d){
		return get(d-1)-get(c-1);
	}
};

//O(1) add, O(sqrt(N)) get
struct ds1{
	int n,B,m;
	vc<ll> a,b;
	ds1(int nn){
		n=nn;
		B=max(int(sqrtl(n)),int(1));
		m=n/B+1;
		a.resize(n);
		b.resize(m);
	}
	void add(int i,ll v){
		assert(inc(0,i,n-1));
		a[i]+=v;
		b[i/B]+=v;
	}
	void addrng(int c,int d,ll v){
		if(c==d)return;
		assert(c<d);
		add(c,v);
		if(d<n)add(d,-v);
	}
	ll get(int i){
		int x=i/B;
		ll res=0;
		rep(j,x)res+=b[j];
		rng(j,x*B,i+1)res+=a[j];
		return res;
	}
	/*
	not verified
	int sum(int c,int d){
		int x=c/B,y=(d+B-1)/B;
		int res=0;
		rng(i,x,y){
			int l=max(c,i*B);
			int r=min(d,(i+1)*B);
			if(l==i*B&&r==(i+1)*B) res+=b[i];
			else rng(j,l,r) res+=a[j];
		}
		return res;
	}
	*/
};


//6th Turing Cup B
//https://contest.xinyoudui.com/contest/237/problem/1276
//stress-tested
/*
//B分木
//range add
//point get
template<class t,int B>
struct addsum{
	int n,L;
	vvc<t> x;
	addsum(int nn):n(nn){
		vi lens;
		while(nn){
			lens.pb(nn);
			nn=nn/B;
		}
		L=si(lens);
		x.resize(L);
		rep(i,L)x[i].resize(lens[i]);
	}
	void add(int l,int r,t v){
		assert(0<=l&&l<=r&&r<=n);
		rep(lv,L){
			int lp=l/B;
			int rp=r/B;
			if(lp==rp){
				rng(i,l,r)x[lv][i]+=v;
				break;
			}
			lp++;
			rng(i,l,lp*B)x[lv][i]+=v;
			rng(i,rp*B,r)x[lv][i]+=v;
			l=lp;
			r=rp;
		}
	}
	t get(int i){
		assert(inc(0,i,n-1));
		t res=0;
		rep(lv,L){
			if(i>=si(x[lv]))break;
			res+=x[lv][i];
			i/=B;
		}
		return res;
	}
};
*/
//2^L分木
//3段確定
//range add
//point get
template<class t,int L,int S>
struct addsum{
	t x[3][S];
	void add(int l0,int r0,t v){
		int l1=(l0+(1<<L)-1)>>L,r1=r0>>L;
		if(l1<=r1){
			rng(i,l0,l1<<L)x[0][i]+=v;
			rng(i,r1<<L,r0)x[0][i]+=v;
			
			int l2=(l1+(1<<L)-1)>>L,r2=r1>>L;
			if(l2<=r2){
				rng(i,l1,l2<<L)x[1][i]+=v;
				rng(i,r2<<L,r1)x[1][i]+=v;
				rng(i,l2,r2)x[2][i]+=v;
			}else{
				rng(i,l1,r1)x[1][i]+=v;
			}
		}else{
			rng(i,l0,r0)x[0][i]+=v;
		}
	}
	t get(int i){
		return x[0][i]+x[1][i>>L]+x[2][i>>(L*2)];
	}
};
