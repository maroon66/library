struct MinNode{
	int v;
	MinNode(int vv=inf):v(vv){}
	static MinNode merge(const MinNode&a,const MinNode&b){
		return MinNode(min(a.v,b.v));
	}
	bool ok(int x){return x<=v;}
};

struct MaxNode{
	int v;
	MaxNode(int vv=-inf):v(vv){}
	static MaxNode merge(const MaxNode&a,const MaxNode&b){
		return MaxNode(max(a.v,b.v));
	}
	bool ok(int x){return v<x;}
};

struct MaxNode{
	pi v;
	MaxNode(pi vv=pi(-inf,-1)):v(vv){}
	static MaxNode merge(const MaxNode&a,const MaxNode&b){
		return MaxNode(max(a.v,b.v));
	}
};

struct MinNode{
	pi v;
	MinNode(pi vv=pi(inf,-1)):v(vv){}
	MinNode(int val,int idx):v(val,idx){}
	static MinNode merge(const MinNode&a,const MinNode&b){
		return MinNode(min(a.v,b.v));
	}
};

template<class t>
struct SumNode{
	t v;
	SumNode(t vv=0):v(vv){}
	static SumNode merge(const SumNode&a,const SumNode&b){
		return SumNode(a.v+b.v);
	}
};

//prefix sum の min を取る
//composite(l,r) すると，[l,r) の prefix sum に関する情報がもらえる
//min の位置 pos, 値 val がもらえる
//pos は最小がもらえる
//Ptz 2022S 7-A
struct PreMin{
	int len,sum,pos,val;
	PreMin():len(0),sum(0),pos(0),val(0){}
	PreMin(int v):len(1),sum(v){
		single();
	}
	void single(){
		if(sum<0){
			pos=1;
			val=sum;
		}else{
			pos=0;
			val=0;
		}
	}
	static PreMin merge(const PreMin&a,const PreMin&b){
		PreMin res;
		res.len=a.len+b.len;
		res.sum=a.sum+b.sum;
		if(a.val<=a.sum+b.val){
			res.pos=a.pos;
			res.val=a.val;
		}else{
			res.pos=a.len+b.pos;
			res.val=a.sum+b.val;
		}
		return res;
	}
	void add(int v){
		assert(len==1);
		sum+=v;
		single();
	}
};
//max版
struct PreMax{
	int len,sum,pos,val;
	PreMax():len(0),sum(0),pos(0),val(0){}
	PreMax(int v):len(1),sum(v){
		single();
	}
	void single(){
		if(sum>0){
			pos=1;
			val=sum;
		}else{
			pos=0;
			val=0;
		}
	}
	static PreMax merge(const PreMax&a,const PreMax&b){
		PreMax res;
		res.len=a.len+b.len;
		res.sum=a.sum+b.sum;
		if(a.val>=a.sum+b.val){
			res.pos=a.pos;
			res.val=a.val;
		}else{
			res.pos=a.len+b.pos;
			res.val=a.sum+b.val;
		}
		return res;
	}
	void add(int v){
		assert(len==1);
		sum+=v;
		single();
	}
};

//seg.point_change(i,v)
//-> [0..i] に対して +v
struct MinStarry{
	int v,p;
	MinStarry(int vv=inf):v(vv),p(0){}
	static MinStarry merge(const MinStarry&a,const MinStarry&b){
		MinStarry res(min(a.v+b.p,b.v));
		res.p=a.p+b.p;
		return res;
	}
	void add(int a){
		v+=a;
		p+=a;
	}
};

//min を K 個保存
template<int K>
struct MinNode{
	int v[K];
	MinNode(){rep(i,K)v[i]=inf;}
	MinNode(int vv){rep(i,K)v[i]=inf;v[0]=vv;}
	static MinNode merge(const MinNode&a,const MinNode&b){
		MinNode res;
		int i=0,j=0;
		rep(k,K){
			if(a.v[i]<b.v[j])res.v[k]=a.v[i++];
			else res.v[k]=b.v[j++];
		}
		return res;
	}
};

const int K=40;
struct N{
	int buf[K];
	N(){
		rep(i,K)buf[i]=INT_MAX;
	}
	static N merge(const N&a,const N&b){
		N res;
		rep(i,K)res.buf[i]=min(a.buf[i],b.buf[i]);
		return res;
	}
	void gen_rand(){
		rep(i,K)buf[i]=rand_int(INT_MIN,INT_MAX);
	}
};

//IOI2021 1A
//連続部分列の min,max 等
struct N{
	int a,mina,minl,minr,maxa,maxl,maxr;
	N(int v=0):a(v),mina(min(v,0LL)),minl(mina),minr(mina),maxa(max(v,0LL)),maxl(maxa),maxr(maxa){}
	static N merge(const N&a,const N&b){
		N res;
		res.a=a.a+b.a;
		res.mina=min({a.mina,b.mina,a.minr+b.minl});
		res.minl=min(a.minl,a.a+b.minl);
		res.minr=min(a.minr+b.a,b.minr);
		res.maxa=max({a.maxa,b.maxa,a.maxr+b.maxl});
		res.maxl=max(a.maxl,a.a+b.maxl);
		res.maxr=max(a.maxr+b.a,b.maxr);
		return res;
	}
	bool okmin(int v){
		return mina>v;
	}
	bool okmax(int v){
		return maxa<v;
	}
};

//ハッシュを取る
//UCUP 3-21-M
struct N{
	mint val,w;
	N():val(0),w(1){}
	N(int v):val(v),w(B){}
	static N merge(const N&a,const N&b){
		N res;
		res.val=a.val+b.val*a.w;
		res.w=a.w*b.w;
		return res;
	}
};

//1st Ucup 13 G
//maintain diameter
unique_ptr<etlca<E>>wd;
int wdist(int a,int b){
	return wd->wdist(a,b);
}
struct N{
	int v[2];
	N(int z=-1):v{z,z}{}
	static N merge(const N&x,const N&y){
		int a=-1,b=-1,c=-1;
		auto upd=[&](int d,int e){
			if(d!=-1&&e!=-1&&chmax(c,wdist(d,e))){
				a=d;
				b=e;
			}
		};
		upd(x.v[0],x.v[1]);
		upd(y.v[0],y.v[1]);
		rep(i,2)rep(j,2)upd(x.v[i],y.v[j]);
		N res;
		res.v[0]=a;
		res.v[1]=b;
		return res;
	}
};

template<class N>
struct Point1D{
	segtree<N> seg;
	vi pos;
	Point1D(){}
	void clear(){
		seg.clear();
		pos.clear();
	}
	void addp(int p){
		pos.pb(p);
	}
	//NOT Verified
	void init(){
		mkuni(pos);
		//seg=segtree<N>(vc<N>(si(pos)));
		seg.resize(si(pos));
	}
	int idx(int p){
		return lwb(pos,p);
	}
	void setv(int p,N v){
		seg.set(idx(p),v);
	}
	N get(int b,int e){
		return seg.composite(idx(b),idx(e));
	}
};

//N() が単位元
template<class N>
struct segtree{
	vc<N> x;
	int s;
	segtree(){}
	template<class t>
	segtree(vc<t> a){
		int n=a.size();
		s=1;
		while(s<n){s*=2;}
		x.resize(s*2);
		rep(i,n)
			x[s+i]=N(a[i]);
		gnr(i,1,s)
			N::merge(x[i*2],x[i*2+1],x[i]);
	}
	//NOT Verified
	segtree(int n){
		resize(n);
	}
	void resize(int n){
		s=1;
		while(s<n){s*=2;}
		x.assign(s*2,N());
		gnr(i,1,s)
			N::merge(x[i*2],x[i*2+1],x[i]);
	}
	void set(int i,const N&t){
		i+=s;
		x[i]=t;
		while(i>>=1)N::merge(x[i*2],x[i*2+1],x[i]);
	}
	N composite(int b,int e){
		assert(b<=e);
		N lf[2],rt[2];
		int lcur=0,rcur=0;
		for(int l=b+s,r=e+s;l<r;l>>=1,r>>=1){
			if (l&1){
				N::merge(lf[lcur],x[l],lf[lcur^1]);
				l++;
				lcur^=1;
			}
			if (r&1){
				r--;
				N::merge(x[r],rt[rcur],rt[rcur^1]);
				rcur^=1;
			}
		}
		N::merge(lf[lcur],rt[rcur],lf[lcur^1]);
		return lf[lcur^1];
	}
};
