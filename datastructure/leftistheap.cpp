//ATC002 C
//F(a,b) -> b を選択
//普通の priority_queue と同じ (less<int> で max が取れる)
template<class T,class F>
struct leftistheap{
	struct N{
		T t;
		N *l,*r;
		int s;
		N():l(nullptr),r(nullptr),s(1){}
	};
	using np=N*;
	np buf;
	int bufused;
	F f;
	leftistheap(int n,F ff=F()):buf(new N[n]),bufused(0),f(ff){}
	~leftistheap(){delete[] buf;}
	np nn(T t){
		np res=buf+bufused++;
		res->t=t;
		return res;
	}
	int depth(np a){
		if(!a)return 0;
		return a->s;
	}
	np merge(np a,np b){
		if(!a)return b;
		if(!b)return a;
		if(f(a->t,b->t))swap(a,b);
		a->r=merge(a->r,b);
		if(depth(a->l)<depth(a->r))
			swap(a->l,a->r);
		a->s=depth(a->r)+1;
		return a;
	}
	np pop(np a){
		return merge(a->l,a->r);
	}
	pair<T,T> gettwo(np a,T none){
		T x=none,y=none;
		if(a){
			x=a->t;
			if(a->l&&f(y,a->l->t))y=a->l->t;
			if(a->r&&f(y,a->r->t))y=a->r->t;
		}
		return mp(x,y);
	}
};

//ATC002 C
//深さは 0 からスタートするものとする
int hu_tucker(vi a){
	a.insert(a.bg,inf);
	a.pb(inf);
	int n=si(a);
	leftistheap<int,greater<int>> z(n*6);
	vi lf(n),rt(n);
	rep(i,n){
		lf[i]=i-1;
		rt[i]=i+1;
	}
	using np=decltype(z)::np;
	vc<np> ls(n);
	
	priority_queue<pi,vc<pi>,greater<pi>> pq;
	using A=array<int,4>;
	vc<A> buf(n);
	rep(i,n)rep(j,4)buf[i][j]=inf;
	auto upd=[&](int i,int j,int v){
		buf[i][j]=v;
		if(v<inf)pq.emplace(v,i*4+j);
	};
	rep(i,n-1)upd(i,0,a[i]+a[i+1]);
	
	//del from leaves
	auto del=[&](int i){
		assert(inc(1,i,n-2));
		int x=lf[i],y=rt[i];
		assert(0<=x);
		assert(y<n);
		rt[x]=y;
		lf[y]=x;
		rep(j,4)buf[i][j]=inf;
		ls[x]=z.merge(ls[x],ls[i]);
		return x;
	};
	auto updval=[&](int x){
		int y=rt[x];
		assert(0<=x);
		assert(y<n);
		upd(x,0,a[x]+a[y]);
		auto [p,q]=z.gettwo(ls[x],inf);
		upd(x,2,a[x]+p);
		upd(y,1,a[y]+p);
		upd(x,3,p+q);
	};
	
	int ans=0;
	
	rep(step,n-3){
		auto [val,ij]=pq.top();pq.pop();
		int i=ij/4,j=ij%4;
		if(buf[i][j]!=val){
			step--;
			continue;
		}
		ans+=val;
		int x;
		if(j==0){
			x=del(i);
			del(rt[x]);
		}else if(j==1){
			ls[lf[i]]=z.pop(ls[lf[i]]);
			x=del(i);
		}else if(j==2){
			ls[i]=z.pop(ls[i]);
			x=del(i);
		}else if(j==3){
			ls[i]=z.pop(ls[i]);
			ls[i]=z.pop(ls[i]);
			x=i;
		}else assert(false);
		ls[x]=z.merge(ls[x],z.nn(val));
		updval(x);
	}
	
	return ans;
}

//verify: yosupo k shortest path
//S_ bytes 消費
template<class T,class F,int S_>
struct persistent_heap{
	struct N{
		T v;
		N *l,*r;
		int s;
	};
	static constexpr int S=S_/sizeof(N);
	N w[S];
	F f=F();
	int used=0;
	using np=N*;
	int depth(np a){
		return a?a->s:0;
	}
	np nn(T v,np l,np r){
		assert(used<S);
		if(depth(l)<depth(r))swap(l,r);
		w[used]={v,l,r,depth(r)+1};
		return w+used++;
	}
	np merge(np a,np b){
		if(!a)return b;
		if(!b)return a;
		if(f(a->v,b->v))swap(a,b);
		return nn(a->v,a->l,merge(a->r,b));
	}
	np pop(np a){
		return merge(a->l,a->r);
	}
};

//verify https://judge.yosupo.jp/problem/directedmst
//F(a,b) -> b を選択
//普通の priority_queue と同じ (less<int> で max が取れる)
template<class T,class F,class U>
struct leftistheaplazy{
	struct N{
		T t;
		U u;
		N *l,*r;
		int s;
		N():t{},u{},l(nullptr),r(nullptr),s(1){}
	};
	using np=N*;
	const int n;
	unique_ptr<N[]> buf;
	int used;
	F f;
	leftistheaplazy(int nn,F ff=F()):n(nn),buf(new N[n]),used(0),f(ff){}
	np nn(T t){
		assert(used<n);
		np res=&buf[used++];
		res->t=t;
		return res;
	}
	int depth(np a){
		if(!a)return 0;
		return a->s;
	}
	np add(np a,const U&v){
		if(a){
			a->t+=v;
			a->u+=v;
		}
		return a;
	}
	np sub(np a,const U&v){
		if(a){
			a->t-=v;
			a->u-=v;
		}
		return a;
	}
	np merge(np a,np b){
		if(!a)return b;
		if(!b)return a;
		if(f(a->t,b->t))swap(a,b);
		a->r=merge(a->r,sub(b,a->u));
		if(depth(a->l)<depth(a->r))
			swap(a->l,a->r);
		a->s=depth(a->r)+1;
		return a;
	}
	void meld(np&a,np b){
		a=merge(a,b);
	}
	T pop(np&a){
		T res=a->t;
		a=merge(add(a->l,a->u),add(a->r,a->u));
		return res;
	}
};

//persistent で lazy な heap
//
template<class F,class U,int S_>
struct persistent_heap{
	struct N{
		U u;
		N *l,*r;
		int s;
	};
	static constexpr int S=S_/sizeof(N);
	N w[S];
	F f=F();
	int used=0;
	using np=N*;
	int depth(np a){
		return a?a->s:0;
	}
	np nn(U u,np l,np r){
		assert(used<S);
		if(depth(l)<depth(r))swap(l,r);
		w[used]={u,l,r,depth(r)+1};
		return w+used++;
	}
	np nn(){
		static np memo=nullptr;
		if(!memo)memo=nn(0,nullptr,nullptr);
		return memo;
	}
	np clone(np x){
		assert(used<S);
		w[used]=*x;
		return w+used++;
	}
	np add(np x,U u){
		if(!x||u==0)return x;
		x=clone(x);
		x->u+=u;
		return x;
	}
	np merge(np a,np b,U aoff=0,U boff=0){
		if(!a)return add(b,boff);
		if(!b)return add(a,aoff);
		if(f(a->u+aoff,b->u+boff)){
			swap(a,b);
			swap(aoff,boff);
		}
		return nn(a->u+aoff,a->l,merge(a->r,b,0,boff-(a->u+aoff)));
	}
	np pop(np a){
		return merge(a->l,a->r,a->u,a->u);
	}
};

persistent_heap<less<int>,int,900'000'000> ph;
using np=decltype(ph)::np;
