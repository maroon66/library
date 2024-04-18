
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
