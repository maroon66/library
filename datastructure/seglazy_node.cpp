//empty という概念を導入したら必ず clone を作ること

//Range add,max
struct N{
	int lz,mx;
	N(int v=-inf):lz(0),mx(v){}
	void add(int v){
		mx+=v;
		lz+=v;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static N merge(N x,N y){
		return N(max(x.getm(),y.getm()));
	}
	int getm(){return mx;}
	bool ok(int v){
		return mx<=v;
	}
};

//Range add,min
struct AddMin{
	int lz,mn;
	AddMin(int v=inf):lz(0),mn(v){}
	void add(int v){
		mn+=v;
		lz+=v;
	}
	void push(AddMin&x,AddMin&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static AddMin merge(AddMin x,AddMin y){
		return AddMin(min(x.getm(),y.getm()));
	}
	int getm(){return mn;}
	bool ok(int v){
		return mn>v;
	}
};

//CGR20 I
//Range add,min,idx
struct N{
	int lz;
	pi mn;
	N(pi v=pi(inf,-1)):lz(0),mn(v){}
	void add(int v){
		mn.a+=v;
		lz+=v;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static N merge(N x,N y){
		return N(min(x.mn,y.mn));
	}
};

//Range add,sum
struct AddSum{
	int len,sum,lz;
	AddSum(pi z=pi(0,0)):len(z.a),sum(z.b),lz(0){}
	void add(int v){
		sum+=v*len;
		lz+=v;
		return;
	}
	void push(AddSum&x,AddSum&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static AddSum merge(AddSum x,AddSum y){
		return AddSum(pi(x.len+y.len,x.sum+y.sum));
	}
};

//Range add,sum (mint)
struct N{
	int len;
	mint sum,lz;
	N(int l,mint v):len(l),sum(v),lz(0){}
	N(pair<int,mint> z=pair<int,mint>(0,0)):len(z.a),sum(z.b),lz(0){}
	void add(mint v){
		sum+=v*len;
		lz+=v;
		return;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static N merge(N x,N y){
		return N(mp(x.len+y.len,x.sum+y.sum));
	}
};

using A=array<int,2>;
//Range add,sum (size K)
//CF 789 E
struct N{
	int len;
	A sum,lz;
	N(int z=0):len(z),sum{},lz{}{}
	void add(const A&v){
		rep(k,2){
			sum[k]+=v[k]*len;
			lz[k]+=v[k];
		}
		return;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		rep(k,2)lz[k]=0;
	}
	static N merge(const N&x,const N&y){
		N res(x.len+y.len);
		rep(k,2)res.sum[k]=x.sum[k]+y.sum[k];
		return res;
	}
};

//Range Add,Cap,GetMax
struct N{
	int mx,cap,ad;
	N(int v=-inf):mx(v),cap(inf),ad(0){}
	void setcap(int c){
		chmin(mx,c);
		chmin(cap,c-ad);
	}
	void add(int a){
		mx+=a;
		ad+=a;
	}
	void addcap(int a,int c){
		add(a);
		setcap(c);
	}
	void pushsub(N&x){
		x.setcap(cap);
		x.add(ad);
	}
	void push(N&x,N&y){
		pushsub(x);
		pushsub(y);
		cap=inf;
		ad=0;
	}
	static N merge(const N&x,const N&y){
		return N(max(x.mx,y.mx));
	}
	bool small(int v){
		return mx<v;
	}
};

//0 の部分に対応する値の総和
//UTPC2020E
//CF Global 24 H
template<class t>
struct Count0{
	using P=pair<int,t>;
	int mn,lz;
	t val;
	Count0():mn(inf),lz(0),val(0){}
	//Count0(P rw=P(inf,0)):mn(rw.a),lz(0),val(rw.b){}
	Count0(int m,mint v):mn(m),lz(0),val(v){}
	void add(int v){
		mn+=v;
		lz+=v;
	}
	void push(Count0&x,Count0&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static Count0 merge(const Count0&a,const Count0&b){
		Count0 res(min(a.mn,b.mn),0);
		if(res.mn==a.mn)res.val+=a.val;
		if(res.mn==b.mn)res.val+=b.val;
		return res;
	}
	t getzeros(){
		assert(0<=mn);
		return mn==0?val:0;
	}
};

//1st UCUP 13 L

//CF516F
//Range chmax,max
struct N{
	int mx,lz;
	N(int v=-inf):mx(v),lz(-inf){}
	void upd(int v){
		chmax(mx,v);
		chmax(lz,v);
	}
	void push(N&x,N&y){
		x.upd(lz);
		y.upd(lz);
		lz=-inf;
	}
	static N merge(N x,N y){
		return N(max(x.mx,y.mx));
	}
};

//KUPC2021 E
//Range chmin(and min but not verified)
struct N{
	int mn,lz;
	N(int v=inf):mn(v),lz(inf){}
	void upd(int v){
		chmin(mn,v);
		chmin(lz,v);
	}
	void push(N&x,N&y){
		x.upd(lz);
		y.upd(lz);
		lz=inf;
	}
	static N merge(N x,N y){
		return N(min(x.mn,y.mn));
	}
};

//range chmin,getmax
//CF Global 24 H
struct N{
	int mx,lz;
	N(int v=-inf):mx(v),lz(inf){}
	void upd(int v){
		chmin(mx,v);
		chmin(lz,v);
	}
	void push(N&x,N&y){
		x.upd(lz);
		y.upd(lz);
		lz=inf;
	}
	static N merge(N x,N y){
		return N(max(x.mx,y.mx));
	}
};

//2種類の値を flip で入れかえる
//IOI2022 2A
struct N{
	mint v[2];
	bool lz;
	N():v{},lz(false){}
	N(pair<mint,mint> ab):v{ab.a,ab.b},lz(false){}
	void flip(){
		swap(v[0],v[1]);
		lz^=true;
	}
	void push(N&x,N&y){
		if(lz){
			x.flip();
			y.flip();
		}
		lz=false;
	}
	static N merge(const N&x,const N&y){
		N res;
		rep(k,2)res.v[k]=x.v[k]+y.v[k];
		return res;
	}
};

//UCUP 2-24-D
//Range mult,sum
struct N{
	mint sum,lz;
	N(mint v=0):sum(v),lz(1){}
	void mult(mint v){
		sum*=v;
		lz*=v;
	}
	void push(N&x,N&y){
		x.mult(lz);
		y.mult(lz);
		lz=1;
	}
	static N merge(N x,N y){
		return N(x.sum+y.sum);
	}
};

//range add,set
//get min,max,sum
//1要素の値域は全部int
//UCUP 3-26-L
const int none=-inf;
struct N{
	int minval,maxval,lzset,lzadd,len;
	ll sum;
	N():minval(inf),maxval(-inf),lzset(none),lzadd(0),len(0),sum(0){}
	N(int v):minval(v),maxval(v),lzset(none),lzadd(0),len(1),sum(v){}
	void updset(int v){
		minval=maxval=lzset=v;
		lzadd=0;
		sum=(ll)len*v;
	}
	void updadd(int v){
		if(lzset==none){
			minval+=v;
			maxval+=v;
			lzadd+=v;
			sum+=(ll)len*v;
		}else{
			updset(lzset+v);
		}
	}
	void push(N&x,N&y){
		if(lzset!=none){
			x.updset(lzset);
			y.updset(lzset);
		}else{
			x.updadd(lzadd);
			y.updadd(lzadd);
		}
		lzset=none;
		lzadd=0;
	}
	static N merge(const N&x,const N&y){
		N res;
		res.minval=min(x.minval,y.minval);
		res.maxval=max(x.maxval,y.maxval);
		res.len=x.len+y.len;
		res.sum=x.sum+y.sum;
		return res;
	}
};
