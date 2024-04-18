//JAGSUMMER2018Day2H
//CodeChef Persistent Oak
template<class N>
struct psegtree{
	using P=pair<N,N>;
	int curt;
	struct N2{
		N n;
		int l,r,t;
	};
	vc<N2> x;
	int s,root;
	template<class t>
	psegtree(const vc<t>&a){
		int n=a.size();
		s=1;
		while(s<n)s*=2;
		x.resize(s*2,{N(),0,0,0});
		rep(i,n)
			x[s+i].n=N(a[i]);
		gnr(i,1,s){
			x[i].n=N::merge(x[i*2].n,x[i*2+1].n);
			x[i].l=i*2;
			x[i].r=i*2+1;
		}
		curt=0;
		root=1;
	}
	int snapshot(){
		curt++;
		return root;
	}
	int modify(int i,int j,int k,N w){
		if(x[i].t!=curt){
			i=x.size();
			x.eb();
		}
		x[i]=N2{w,j,k,curt};
		return i;
	}
	P push(int i,N w){
		N a=x[x[i].l].n,b=x[x[i].r].n;
		w.push(a,b);
		return P(a,b);
	}
	int upd(int i,int j,int k){
		return modify(i,j,k,N::merge(x[j].n,x[k].n));
	}
	template<class F,class... Args>
	int chr(int i,N w,int l,int r,int b,int e,F f,Args... args){
		if(e<=l||r<=b)
			return modify(i,x[i].l,x[i].r,w);
		if(b<=l&&r<=e){
			(w.*f)(args...);
			return modify(i,x[i].l,x[i].r,w);
		}
		N ln,rn;
		tie(ln,rn)=push(i,w);
		int m=(l+r)/2;
		int c=chr(x[i].l,ln,l,m,b,e,f,args...);
		int d=chr(x[i].r,rn,m,r,b,e,f,args...);
		return upd(i,c,d);
	}
	template<class F,class... Args>
	int ch(int i,int b,int e,F f,Args... args){
		assert(b<e);
		return root=chr(i,x[i].n,0,s,b,e,f,args...);
	}
	template<class F,class G,class H>
	auto getr(int i,N w,int l,int r,int b,int e,F f,G g,H h){
		if(e<=l||r<=b)return h;
		if(b<=l&&r<=e)return (w.*f)();
		N ln,rn;
		tie(ln,rn)=push(i,w);
		int m=(l+r)/2;
		return g(getr(x[i].l,ln,l,m,b,e,f,g,h),getr(x[i].r,rn,m,r,b,e,f,g,h));
	}
	template<class F,class G,class H>
	auto get(int i,int b,int e,F f,G g,H h){
		assert(b<e);
		return getr(i,x[i].n,0,s,b,e,f,g,h);
	}
	int repr(int i,N w1,int j,N w2,int l,int r,int b,int e){
		if(e<=l||r<=b)return modify(i,x[i].l,x[i].r,w1);
		if(b<=l&&r<=e)return modify(i,x[j].l,x[j].r,w2);
		N ln1,rn1,ln2,rn2;
		tie(ln1,rn1)=push(i,w1);
		tie(ln2,rn2)=push(j,w2);
		int m=(l+r)/2;
		int c=repr(x[i].l,ln1,x[j].l,ln2,l,m,b,e);
		int d=repr(x[i].r,rn1,x[j].r,rn2,m,r,b,e);
		return upd(i,c,d);
	}
	int replace(int i,int j,int b,int e){
		assert(b<e);
		return root=repr(i,x[i].n,j,x[j].n,0,s,b,e);
	}
	//return maximum index
	template<class F,class...Args>
	pair<int,N> findr(int i,N w,int l,int r,int b,int e,F f,Args...args){
		if(e<=l||r<=b||!(w.*f)(args...))return {b-1,N()};
		if(b<=l&&r<=e&&r-l==1)
			return {l,w};
		N ln,rn;
		tie(ln,rn)=push(i,w);
		int m=(l+r)/2;
		auto a=findr(x[i].r,rn,m,r,b,e,f,args...);
		if(b<=a.a)return a;
		return findr(x[i].l,ln,l,m,b,e,f,args...);
	}
	template<class F,class...Args>
	pair<int,N> find(int i,int b,int e,F f,Args...args){
		assert(b<e);
		return findr(i,x[i].n,0,s,b,e,f,args...);
	}
};

struct N{
	int lz,mn;
	N(int v=0):lz(0),mn(v){}
	void add(int v){
		lz+=v;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		mn-=lz;
	}
	int getm(){
		return mn-lz;
	}
	int geta(){
		return lz;
	}
	static N merge(N x,N y){
		return N(min(x.getm(),y.getm()));
	}
	bool find(){
		return getm()<0;
	}
};


//XXII Opencup SEERC B
//push 自分の lz を書き換えているがいらない説ある
const int K=4;
const int V=5*ten(8);
using A=array<int,K>;
struct N{
	int mn[1<<K],lz[K];
	N(const A&a){
		zero(mn);
		rep(bit,1<<K){
			rep(i,K)if(bit&1<<i)mn[bit]+=a[i];
		}
		zero(lz);
	}
	N(bool empty=true){
		if(empty){
			rep(i,1<<K)mn[i]=inf;
		}else{
			zero(mn);
		}
		zero(lz);
	}
	void work(int i,int v){
		if(v<V){
			rep(bit,1<<K)if(bit&1<<i){
				mn[bit]+=v;
			}
			lz[i]+=v;
		}else{
			rep(bit,1<<K)if(bit&1<<i){
				mn[bit]=mn[bit^1<<i]+(v-2*V);
			}
			lz[i]=v;
		}
	}
	void push(N&a,N&b){
		rep(i,K)if(lz[i]){
			a.work(i,lz[i]);
			b.work(i,lz[i]);
			lz[i]=0;
		}
	}
	static N merge(const N&a,const N&b){
		N res;
		rep(i,1<<K)res.mn[i]=min(a.mn[i],b.mn[i]);
		return res;
	}
	int getm(){
		return mn[mask(K)];
	}
};
