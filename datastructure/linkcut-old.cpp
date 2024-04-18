//find は lazy と組み合わせたときに何が起こるか未知数でございます(は？)

//cf16-tournament-round1-open A
//JOISC2013 day4 Spaceships
//AOJ2450
//CF Hello2020
template<class N>
struct linkcut{
	struct Node{
		using np=Node*;
		np l,r,p;
		bool rev;
		N x;
		Node():l(0),r(0),p(0),rev(false),x(){}
		void reverse(){
			rev^=true;
			x.reverse();
		}
		void propagate(){
			if(rev){
				swap(l,r);
				if(l)l->reverse();
				if(r)r->reverse();
				rev=false;
			}
			if(l)x.push(l->x);
			if(r)x.push(r->x);
			x.clear();
		}
		void update(){
			x.single();
			if(l)
				x.updatel(l->x);
			if(r)
				x.updater(r->x);
		}
		int pos(){
			if(p&&p->l==this) return -1;
			if(p&&p->r==this) return 1;
			return 0;
		}
		void prepare(){
			if(pos())
				p->prepare();
			propagate();
		}
		void rotate(){
			np q=p,c;
			if(pos()==1){
				c=l;
				l=p;
				p->r=c;
			}else{
				c=r;
				r=p;
				p->l=c;
			}
			if(c)c->p=p;
			p=p->p;
			q->p=this;
			if(p&&p->l==q)p->l=this;
			if(p&&p->r==q)p->r=this;
			q->update();
		}
		void splay(){
			prepare();
			while(pos()){
				int a=pos(),b=p->pos();
				if(b&&a==b)p->rotate();
				if(b&&a!=b)rotate();
				rotate();
			}
			update();
		}
		void expose(){
			for(np a=this;a;a=a->p)a->splay();
			for(np a=this;a->p;a=a->p){
				a->p->r=a;
				a->p->update();
			}
			splay();
		}
		void evert(){
			expose();
			if(l){
				l->reverse();
				l=0;
				update();
			}
		}
		void link(np a){
			evert();
			p=a;
		}
		template<class F,class...Args>
		np find(F f,Args&&...args){
			if(!(x.*f)(forward<Args>(args)...))return 0;
			propagate();
			x.single();
			np a=0;
			if(l)a=l->find(f,forward<Args>(args)...);
			if(a)return a;
			if((x.*f)(forward<Args>(args)...)){
				splay();
				return this;
			}
			return r->find(f,forward<Args>(args)...);
		}
		void cutchild(){
			expose();
			r=0;
			update();
		}
		void cutpar() {
			expose();
			assert(l);
			l->p=0;
			l=0;
			update();
		}
	}*x;
	linkcut(int n){
		x=new Node[n];
	}
	~linkcut(){delete[] x;}
	
	using np=typename Node::np;
	int lca(int aa,int bb){
		np a=x+aa,b=x+bb;
		a->expose();
		b->expose();
		if(!a->p)return -1;
		
		np d=a;
		while(a->p!=b){
			if(a->pos()==0)
				d=a->p;
			a=a->p;
		}
		if(a==b->l)return d-x;
		else return b-x;
	}
	//child->par
	void link(int a,int b){
		x[a].link(x+b);
	}
	void cutpar(int a){
		x[a].cutpar();
	}
	void expose(int a){
		x[a].expose();
	}
	void evert(int a){
		x[a].evert();
	}
	void cutchild(int a){
		x[a].cutchild();
	}
	void expose_path(int a,int b){
		x[a].evert();
		x[b].cutchild();
	}
	N&operator[](int i){
		return x[i].x;
	}
	template<class F,class...Args>
	int find(int a,F f,Args&&...args){
		np res=x[a].find(f,forward<Args>(args)...);
		if(res)return res-x;
		else return -1;
	}
};
//N::reverse
//N::push
//副作用なし,１個の子にpush
//N::clear
//lazy tagを消去
//N::single
//ノード単体の情報を元に部分木情報を初期化
//N::updatel,updater
//N::find

//link(child,par)
//expose_path(a,b) -> a が根になり，b に情報があつまる
//↑二つなんか順番の一貫性がなくね？
//lc.cutchild(a) をして a をみると a の根までのパス情報
//expose 等は必要ない

//range add,sum
struct N{
	int lz,len,val,sum;
	N(int v=0):lz(0),len(1),val(v),sum(v){}
	void add(int v){
		lz+=v;
		val+=v;
		sum+=len*v;
	}
	void reverse(){}
	void push(N&x){
		x.add(lz);
	}
	void clear(){
		lz=0;
	}
	void single(){
		assert(lz==0);
		len=1;
		sum=val;
	}
	void upd(const N&x){
		assert(lz==0);
		len+=x.len;
		sum+=x.sum;
	}
	void updatel(const N&x){
		upd(x);
	}
	void updater(const N&x){
		upd(x);
	}
};

//v が 1 以上のやつを見つけるやつ
struct N{
	int v,s;
	void init(int a){
		v=a;
		single();
	}
	void reverse(){
	}
	void push(N&){
	}
	void clear(){
	}
	void single(){
		s=v;
	}
	void updatel(const N&x){
		s+=x.s;
	}
	void updater(const N&x){
		s+=x.s;
	}
	bool find(){
		return s;
	}
	void add(int x){
		v+=x;
		s+=x;
	}
};

//Do use segment tree
struct M{
	int l,r,s,m,c;
	void init(int v,int len){
		assert(len>=0);
		s=v*len;
		l=r=m=max(s,v);
		c=len;
	}
	void lazy(int v){
		init(v,c);
	}
	void reverse(){
		swap(l,r);
	}
};

M merge(M a,M b){
	if(a.c==0)return b;
	if(b.c==0)return a;
	int l=max(a.l,a.s+b.l);
	int r=max(a.r+b.s,b.r);
	int s=a.s+b.s;
	int m=max({a.m,a.r+b.l,b.m});
	return {l,r,s,m,a.c+b.c};
}

const int none=INT_MIN;

struct N{
	int v,lz;
	M m;
	void init(int a){
		v=a;
		lz=none;
		single();
	}
	void reverse(){
		m.reverse();
	}
	void lazy(int a){
		v=a;
		lz=a;
		m.lazy(a);
	}
	void push(N&x){
		if(lz!=none)
			x.lazy(lz);
	}
	void clear(){
		lz=none;
	}
	void single(){
		assert(lz==none);
		m.init(v,1);
	}
	void updatel(const N&x){
		assert(lz==none);
		m=merge(x.m,m);
	}
	void updater(const N&x){
		assert(lz==none);
		m=merge(m,x.m);
	}
	int getans(){
		return m.m;
	}
};

//path-min
struct N{
	int val,mn;
	void init_vertex(int){
		val=inf;
		single();
	}
	void init_edge(int,int,int w){
		val=w;
		single();
	}
	void reverse(){
	}
	void push(N&){
	}
	void clear(){
	}
	void single(){
		mn=val;
	}
	void upd(const N&x){
		chmin(mn,x.mn);
	}
	void updatel(const N&x){upd(x);}
	void updater(const N&x){upd(x);}
};

//JOISP 2022 Reconstruction
template<class N>
struct lc_helper:linkcut<N>{
	using typename linkcut<N>::Node;
	using linkcut<N>::x;
	int n;
	vi ebuf;
	vc<pi> es;
	lc_helper(int nn):linkcut<N>(nn*2-1),n(nn),es(n-1){
		rep(i,n-1)ebuf.pb(n+i);
	}
	template<class...Args>
	void init_vertex(int v,Args&&...args){
		x[v]=Node();
		x[v].x.init_vertex(v,forward<Args>(args)...);
	}
	template<class...Args>
	int get_init_edge(int u,int v,Args&&...args){
		assert(si(ebuf));
		int e=ebuf.back();ebuf.pop_back();
		x[e]=Node();
		x[e].x.init_edge(u,v,forward<Args>(args)...);
		return e;
	}
	//辺の ID を [0,n-1) で返す
	template<class...Args>
	int link(int a,int b,Args&&...args){
		if(this->lca(a,b)==-1){
			int e=get_init_edge(a,b,forward<Args>(args)...);
			es[e-n]=pi(a,b);
			linkcut<N>::evert(a);linkcut<N>::link(a,e);
			linkcut<N>::evert(b);linkcut<N>::link(b,e);
			return e-n;
		}else{
			return -1;
		}
	}
	//辺の ID を [0,n-1) で渡す
	void cut(int e){
		auto [a,b]=es[e];
		linkcut<N>::evert(e+n);
		linkcut<N>::cutpar(a);
		linkcut<N>::cutpar(b);
		ebuf.pb(e+n);
	}
};
