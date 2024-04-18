void* dbg_ptr;

//JOISC2013 day4 Spaceships
//TTPC2019 M
//AOJ2450
//XX Open Cup GP of Kazan J

template<class N>
struct toptree{
	struct Node{
		static Node* dbg_buf;
		using np=Node*;
		struct {np p,l,r;} r,c;
		//c.p==0 のときのみ r,rake が意味を持つ．
		np par,mid;
		bool rev;
		int a,b,ra,rb;
		N rake,cmp,raw;
		bool is_root(){
			return c.p==0&&r.p==0&&par==0;
		}
		template<class...Args>
		void init(int aa,int bb,Args&&...args){
			r={0,0,0};
			c={0,0,0};
			par=0;
			mid=0;
			rev=0;
			a=ra=aa;
			b=rb=bb;
			raw.init(a,b,forward<Args>(args)...);
			rake=cmp=raw;
		}
		void reverse(){
			rev^=1;
			cmp.reverse(a,b);
			swap(a,b);
			raw.reverse(ra,rb);
			swap(ra,rb);
			swap(c.l,c.r);
		}
		void push_rake(np x){
			assert(c.p==0);
			if(x){
				assert(x->c.p==0);
				assert(x->r.p==this);
				assert(a==x->a);
				rake.push_rake(a,b,x->b,x->rake);
			}
		}
		void push_cmp(np x){
			if(x){
				assert(x->c.p==this);
				if(rev)
					x->reverse();
				cmp.push_cmp(a,b,x->a,x->b,x->cmp);
			}
		}
		void propagate(){
			//rake
			if(c.p==0){
				push_rake(r.l);
				push_rake(r.r);
				rake.push_cmp(a,b,a,b,cmp);
				rake.clear();
			}
			//cmp
			push_cmp(c.l);
			push_cmp(c.r);
			cmp.push_cmp(a,b,ra,rb,raw);
			if(mid){
				assert(ra==rb);
				assert(mid->c.p==0);
				assert(mid->r.p==0);
				assert(mid->par==this);
				cmp.push_mid(a,b,mid->a,mid->b,mid->rake);
			}
			rev=0;
			cmp.clear();
		}
		//current node doesn't have any rake links
		//involved nodes are NOT updated
		void swap_rake(np x){
			assert(x->c.p==0);
			swap(r,x->r);
			if(r.p){
				if(r.p->r.l==x)r.p->r.l=this;
				if(r.p->r.r==x)r.p->r.r=this;
			}
			if(r.l)
				r.l->r.p=this;
			if(r.r)
				r.r->r.p=this;
			if(r.p==0){
				swap(par,x->par);
				if(par)par->mid=this;
			}
		}
		void update_cmp(){
			cmp=raw.clone();
			
			a=ra,b=rb;
			if(mid){
				assert(mid->c.p==0);
				assert(mid->r.p==0);
				assert(mid->a==a);
				assert(mid->par=this);
				cmp=N::rake(a,b,mid->b,cmp,mid->rake);
			}
			if(c.r){
				assert(c.r->a==b);
				assert(c.r->c.p==this);
				cmp=N::compress(a,b,c.r->b,cmp,c.r->cmp);
				b=c.r->b;
			}
			if(c.l){
				assert(c.l->b==a);
				assert(c.l->c.p==this);
				cmp=N::compress(c.l->a,a,b,c.l->cmp,cmp);
				a=c.l->a;
			}
		}
		int pos_cmp(){
			if(c.p&&c.p->c.l==this)return -1;
			if(c.p&&c.p->c.r==this)return 1;
			return 0;
		}
		void rotate_cmp(){
			np q=c.p,w;
			if(q->c.p==0)
				swap_rake(q);
			if(pos_cmp()==1){
				w=c.l;
				c.l=c.p;
				c.p->c.r=w;
			}else{
				w=c.r;
				c.r=c.p;
				c.p->c.l=w;
			}
			if(w) w->c.p=c.p;
			c.p=c.p->c.p;
			q->c.p=this;
			if(c.p&&c.p->c.l==q)c.p->c.l=this;
			if(c.p&&c.p->c.r==q)c.p->c.r=this;
			q->update_cmp();
		}
		void splay_cmp(){
			while(c.p){
				int x=pos_cmp(),y=c.p->pos_cmp();
				if(y&&x==y)c.p->rotate_cmp();
				if(y&&x!=y)rotate_cmp();
				rotate_cmp();
			}
			update_cmp();
		}
		void update_rake(){
			rake=cmp.clone();
			auto sub=[&](np x){
				if(x==0)return;
				assert(x->c.p==0);
				assert(x->a==a);
				assert(x->r.p==this);
				rake=N::rake(a,b,x->b,rake,x->rake);
			};
			sub(r.l);
			sub(r.r);
		}
		int pos_rake(){
			assert(c.p==0);
			if(r.p&&r.p->r.l==this) return -1;
			if(r.p&&r.p->r.r==this) return 1;
			return 0;
		}
		void rotate_rake(){
			np q=r.p,w;
			if(q->r.p==0){
				swap(par,q->par);
				if(par)par->mid=this;
			}
			if(pos_rake()==1){
				w=r.l;
				r.l=r.p;
				r.p->r.r=w;
			}else{
				w=r.r;
				r.r=r.p;
				r.p->r.l=w;
			}
			if(w) w->r.p=r.p;
			r.p=r.p->r.p;
			q->r.p=this;
			if(r.p&&r.p->r.l==q)r.p->r.l=this;
			if(r.p&&r.p->r.r==q)r.p->r.r=this;
			q->update_rake();
		}
		void splay_rake(){
			while(r.p){
				int x=pos_rake(),y=r.p->pos_rake();
				if(y&&x==y)r.p->rotate_rake();
				if(y&&x!=y)rotate_rake();
				rotate_rake();
			}
			update_rake();
		}
		void prepare(){
			if(c.p){
				assert(r.p==0);
				assert(par==0);
				c.p->prepare();
			}
			else if(r.p){
				assert(par==0);
				r.p->prepare();
			}
			else if(par){
				par->prepare();
			}
			propagate();
		}
		static np merge_rake(np x,np y){
			if(x==0)return y;
			if(y==0)return x;
			while(y->r.l){
				y->propagate();
				y=y->r.l;
			}
			y->propagate();
			y->splay_rake();
			y->r.l=x;
			x->r.p=y;
			y->update_rake();
			return y;
		}
		void expose(){
			prepare();
			for(np x=this;x;x=x->par){
				x->splay_cmp();
				x->splay_rake();
			}
			for(np x=this;x->par;){
				np p=x->par;
				x->par=0;
				assert(p->mid=x);
				np y=p->c.r;
				if(y){
					y->c.p=0;
					y->update_rake();
				}
				np z=x->r.l,w=x->r.r;
				if(z){
					z->r.p=0;
					x->r.l=0;
				}
				if(w){
					w->r.p=0;
					x->r.r=0;
				}
				z=merge_rake(z,w);
				z=merge_rake(z,y);
				p->mid=z;
				if(z)z->par=p;
				p->c.r=x;
				x->c.p=p;
				x=p;
			}
			splay_cmp();
			/*if(!is_root()){
				Node* dbg_buf=(Node*)dbg_ptr;
				cerr<<this-dbg_buf<<" "<<c.p-dbg_buf<<" "<<r.p-dbg_buf<<" "<<par-dbg_buf<<endl;
			}*/
			assert(is_root());
		}
		void merge_mid(np x){
			if(mid)
				mid->par=0;
			mid=merge_rake(mid,x);
			if(mid)
				mid->par=this;
			update_cmp();
		}
		void evert(){
			expose();
			np x=c.l;
			if(x==0)return;
			x->c.p=0;
			c.l=0;
			x->reverse();
			x->update_rake();
			merge_mid(x);
		}
		void link_sub(np x,np y){
			assert(is_root());
			assert(x->is_root());
			assert(c.l==0);
			assert(y);
			y->c.p=this;
			c.l=y;
			update_cmp();
			update_rake();
			x->merge_mid(this);
		}
		//this should be an edge node!!
		void cut_sub(){
			expose();
			assert(ra!=rb);
			assert(c.l);
			assert(c.r);
			assert(mid==0);
			c.l->c.p=0;
			c.r->c.p=0;
		}
	} *x;
	using np=Node*;
	int n;
	vc<np> buf;
	toptree(int nn):n(nn){
		x=new Node[2*n];
		rep(i,n)x[i].init(i,i);
		rep(i,n)buf.pb(x+n+i);
		
		dbg_ptr=(void*)x;
	}
	~toptree(){delete[] x;}
	np evert(int a){
		x[a].evert();
		return x+a;
	}
	np expose_path(int aa,int bb){
		np a=x+aa,b=x+bb;
		a->evert();
		if(a!=b){
			b->expose();
			if(a->is_root())return 0;
		}
		np y=b->c.r;
		if(y){
			y->c.p=0;
			b->c.r=0;
			y->update_rake();
			b->merge_mid(y);
		}
		return b;
	}
	//a<-b
	template<class...Args>
	np link(int aa,int bb,Args&&...args){
		np a=x+aa,b=x+bb;
		a->expose();
		b->evert();
		if(a->is_root()){
			np y=buf.back();buf.pop_back();
			y->init(aa,bb,forward<Args>(args)...);
			b->link_sub(a,y);
			return y;
		}else
			return 0;
	}
	void cut(np y){
		assert(y-x>=n);
		assert(y-x<2*n);
		y->cut_sub();
		buf.pb(y);
	}
	void cutpar(int aa){
		assert(0<=aa&&aa<n);
		np a=x+aa;
		a->expose();
		a=a->c.l;
		assert(a);
		while(a->c.r)a=a->c.r;
		assert(a-x>=n);
		cut(a);
	}
	int lca(int aa,int bb){
		np a=x+aa,b=x+bb;
		a->expose();
		b->expose();
		if(a->is_root())
			return -1;
		int c=-2;
		np d=a;
		while(a!=b){
			if(a->c.p){
				assert(a->r.p==0);
				assert(a->par==0);
				c=a->pos_cmp();
				a=a->c.p;
			}else if(a->r.p){
				assert(a->par==0);
				a=a->r.p;
			}else if(a->par){
				a=a->par;
				d=a;
			}else{
				//cerr<<a-x<<" "<<b-x<<endl;
				assert(false);
			}
		}
		a->expose();
		if(c==1)return b-x;
		return d-x;
	}
};

//push_rake(a,b,cb,c): propagate to a rake child c
//push_cmp(a,b,ca,cb,c): propagate to c, whose path (ca,cb) is a subsegment of (a,b)
//push_mid(a,b,ca,cb,c): propagate to a middle child c
//reverse(a,b): reverse the node
//clear(): clear the lazy tag
//clone(): return a cloned node
//compress(a,m,b,x,y): returns the compressed node xy
//rake(a,b,c,x,y): rake y(a,c) to x(a,b)
//nodes are edge-based, and vertex nodes are identitiy nodes
//note that there exists a non-identity node with 0-length path
//expose をして np->cmp を見るのが基本
//np->a,np->b を見る
//evert(a) を呼ぶと，np->a==a になっている．
//問題によっては np->b にも気を使う必要があったりする
//expose_path(a,b) で a が親になる，つまり，Node の端点が a,b となる．(自然)

//empty path に対応するノード，まあ単位元とかだと思ってください(意外とここを適当にやると壊れたり)
//lazy tag をくっつける時は，puttag みたいな関数を使うのが定石

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
	void show(){
		dmp(l);
		dmp(r);
		dmp(s);
		dmp(m);
		dmp(c);
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

const int nmax=200010;
int w[nmax];

struct N{
	int lz;
	static const int none=-123456;
	M m;
	//void init(int a,int b){
	void init(int,int){
		lz=none;
		m.init(0,0);
	}
	//void push_rake(int a,int b,int cb,N&x){
	void push_rake(int,int,int,N&x){
		//x.lz=lz;
		//x.m.lazy(lz);
	}
	//void push_cmp(int a,int b,int ca,int cb,N&x){
	void push_cmp(int a,int b,int ca,int cb,N&x){
		if(lz==none)return;
		if(a!=ca&&ca!=cb)w[ca]=lz;
		x.lz=lz;
		x.m.lazy(lz);
	}
	//void push_mid(int a,int b,int ca,int cb,N&x){
	void push_mid(int,int,int,int,N&x){
		//x.lz=lz;
		//x.m.lazy(lz);
	}
	//void reverse(int a,int b){
	void reverse(int,int){
		m.reverse();
	}
	void clear(){
		lz=none;
	}
	N clone()const{
		N res{none,m};
		return res;
	}
	static N compress(int a,int m,int b,const N&x,const N&y){
		assert(a!=b);
		if(a==m)return y;
		if(m==b)return x;
		M z;z.init(w[m],1);
		N res{none,merge(x.m,(merge(z,y.m)))};
		return res;
	}
	static N rake(int a,int b,int cb,const N&x,const N&y){
		assert(a!=cb);
		return x.clone();
	}
	void putlazy(int a,int b,int v){
		w[a]=v;
		w[b]=v;
		lz=v;
	}
	int getans(int a,int b){
		//dmp(a);
		//dmp(b);
		if(a==b){
			dmp(w[a]);
			M z;z.init(w[a],1);
			return merge(z,m).m;
		}else{
			M x,y;
			x.init(w[a],1);
			y.init(w[b],1);
			return merge(x,merge(m,y)).m;
		}
	}
};

//active な頂点の個数を数える,部分木全体に値を足す

const int nmax=100010;
int st[nmax];

struct N{
	ll v;
	int s;
	//void init(int a,int b){
	void init(int,int){
		v=0;
		s=0;
	}
	//void push_rake(int a,int b,int cb,N&x){
	void push_rake(int,int,int,N&x){
		x.v+=v;
	}
	//void push_cmp(int a,int b,int ca,int cb,N&x){
	void push_cmp(int,int,int,int,N&x){
		x.v+=v;
	}
	//void push_mid(int a,int b,int ca,int cb,N&x){
	void push_mid(int,int,int,int,N&x){
		x.v+=v;
	}
	//void reverse(int a,int b){
	void reverse(int,int){
	}
	void clear(){
		v=0;
	}
	N clone()const{
		N res{0,s};
		return res;
	}
	static N compress(int a,int m,int b,const N&x,const N&y){
		assert(a!=b);
		N res{0,x.s+y.s};
		if(a!=m&&b!=m)res.s+=st[m];
		return res;
	}
	static N rake(int a,int b,int cb,const N&x,const N&y){
		assert(a!=cb);
		N res{0,x.s+y.s};
		res.s+=st[cb];
		return res;
	}
	int gets(int a,int b){
		int res=s;
		res+=st[a];
		if(a!=b)res+=st[b];
		return res;
	}
};


//ARC048D
//lazy なし
const int nmax=100010;
bool f[nmax];

struct N{
	int len;
	int w[2][2];
	//void init(int a,int b){
	void init(int a,int b){
		if(a==b)len=0;
		else len=1;
		rep(k,2){
			if(f[a])w[k][0]=0;
			else if(f[b])w[k][0]=3;
			else w[k][0]=inf;
			if(f[a])w[k][1]=len;
			else if(f[b])w[k][1]=2;
			else w[k][1]=inf;
			swap(a,b);
		}
	}
	//void push_rake(int a,int b,int cb,N&x){
	void push_rake(int,int,int,N&){
	}
	//void push_cmp(int a,int b,int ca,int cb,N&x){
	void push_cmp(int,int,int,int,N&){
	}
	//void push_mid(int a,int b,int ca,int cb,N&x){
	void push_mid(int,int,int,int,N&){
	}
	//void reverse(int a,int b){
	void reverse(int,int){
		swap(w[0],w[1]);
	}
	void clear(){
	}
	N clone()const{
		return *this;
	}
	static N compress(int a,int m,int b,N x,N y){
		assert(a!=b);
		N res;
		res.len=x.len+y.len;
		
		rep(k,2){
			res.w[k][0]=min(x.w[k][0],x.len*3+y.w[k][0]);
			res.w[k][1]=min(x.w[k][1]+y.len,x.len*2+y.w[k][1]);
			swap(a,b);
			swap(x,y);
		}
		
		return res;
	}
	static N rake(int a,int b,int cb,const N&x,const N&y){
		assert(a!=cb);
		N res=x;
		chmin(res.w[0][0],y.w[0][0]);
		chmin(res.w[0][1],y.w[0][0]+x.len);
		chmin(res.w[1][0],x.len*3+y.w[0][0]);
		chmin(res.w[1][1],x.len*2+y.w[0][0]);
		return res;
	}
	int getans(){
		dmp(w[0][0]);
		dmp(w[0][1]);
		return min(len*2,w[0][1]);
	}
};


//直径を求める
struct N{
	int len,w[2],d;
	//void init(int a,int b){
	void init(int a,int b,int c=0){
		len=w[0]=w[1]=d=c;
	}
	//void push_rake(int a,int b,int cb,N&x){
	void push_rake(int,int,int,N&){
	}
	//void push_cmp(int a,int b,int ca,int cb,N&x){
	void push_cmp(int,int,int,int,N&){
	}
	//void push_mid(int a,int b,int ca,int cb,N&x){
	void push_mid(int,int,int,int,N&){
	}
	//void reverse(int a,int b){
	void reverse(int,int){
		swap(w[0],w[1]);
	}
	void clear(){
	}
	N clone()const{
		return *this;
	}
	static N compress(int a,int m,int b,N x,N y){
		assert(a!=b);
		N res;
		res.len=x.len+y.len;
		res.w[0]=max(x.w[0],x.len+y.w[0]);
		res.w[1]=max(y.w[1],y.len+x.w[1]);
		res.d=max({x.d,y.d,x.w[1]+y.w[0]});
		
		return res;
	}
	static N rake(int a,int b,int cb,const N&x,const N&y){
		assert(a!=cb);
		N res=x;
		chmax(res.w[0],y.w[0]);
		chmax(res.w[1],x.len+y.w[0]);
		chmax(res.d,y.d);
		chmax(res.d,x.w[0]+y.w[0]);
		
		return res;
	}
	int getans(){
		return d;
	}
};
