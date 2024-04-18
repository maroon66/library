template<class val>
struct range_node{
	int l,r,mx;
	val v;
	range_node():mx(-inf){}
	range_node(int a,int b,val c):l(a),r(b),mx(r),v(c){}
	void reverse(){}
	void push(range_node&){}
	void clear(){}
	void single(){
		mx=r;
	}
	void updatel(const range_node&x){
		chmax(mx,x.mx);
	}
	void updater(const range_node&x){
		chmax(mx,x.mx);
	}
	bool operator<(const range_node&a)const{
		return l!=a.l?l<a.l:r!=a.r?r<a.r:v<a.v;
	}
	bool operator==(const range_node&a)const{
		return l==a.l&&r==a.r&&v==a.v;
	}
	bool ok(int a){
		return mx<=a;
	}
};

//区間の追加（ついでに適当な付加情報（t）をつける）
//区間でクエリ，共通部分が正の区間 があればいずれかの(l,r,t)を返す
//FHC2022 Final E/range_set/rect_set
template<class val>
struct range_set{
	using N=range_node<val>;
	using T=splaytree<N>;
	using np=typename T::np;
	static T t;
	np root;
	range_set():root(0){}
	~range_set(){t.freetree(root);}
	range_set(const range_set&)=delete;
	void operator=(const range_set&)=delete;
	range_set(range_set&&rhs){
		root=rhs.root;
		rhs.root=0;
	}
	range_set&operator=(range_set&&rhs){
		if(this!=&rhs){
			t.freetree(root);
			root=rhs.root;
			rhs.root=0;
		}
		return *this;
	}
	void add(int l,int r,val v){
		t.emplace(root,l,r,v);
	}
	void del(int l,int r,val v){
		bool tmp=t.erase(root,N(l,r,v));
		assert(tmp);
	}
	tuple<bool,int,int,val> collide(int b,int e){
		if(t.max_right_splay(root,&N::ok,b)&&root->x.l<e)
			return mt(true,root->x.l,root->x.r,root->x.v);
		return mt(false,int(),int(),val());
	};
};
template<class val>
typename range_set<val>::T range_set<val>::t;

struct R{
	int xl,xr,yl,yr;
	bool valid()const{
		return xl<=xr&&yl<=yr;
	}
	bool empty()const{
		return xl==xr||yl==yr;
	}
	int wid()const{
		return xr-xl;
	}
	int hei()const{
		return yr-yl;
	}
	//FHC 2021 Final F
	R translated(int x,int y){
		return {xl+x,xr+x,yl+y,yr+y};
	}
	//FHC 2021 Final F
	R shifted(int dir){
		static const int dxy[]{-1,0,1,0,-1};
		//LURD for 2d-axis
		int x=wid()*dxy[dir],y=hei()*dxy[dir+1];
		return translated(x,y);
	}
};
ostream& operator<<(ostream&os,R v){
	return os<<"["<<v.xl<<","<<v.xr<<")*["<<v.yl<<","<<v.yr<<")";
}
bool intersect(R a,R b){
	return max(a.xl,b.xl)<min(a.xr,b.xr)
		&& max(a.yl,b.yl)<min(a.yr,b.yr);
}

R bounding(R a,R b){
	return R{
		min(a.xl,b.xl),
		max(a.xr,b.xr),
		min(a.yl,b.yl),
		max(a.yr,b.yr),
	};
}

//長方形の追加（追加した順に id をつける）
//長方形でクエリ，共通部分が正の長方形があればいずれかの (R,id) を返す
//FHC2022 Final E/range_set/rect_set
struct rect_set{
	int s;
	template<class F>
	void X(int l,int r,F f){
		for(l+=s,r+=s;l<r;l>>=1,r>>=1){
			if(l&1)f(l);
			if(r&1)f(r-1);
			l++; //ceil l
		}
	}
	template<class F>
	void Y(int i,F f){
		for(i+=s;i;i>>=1)f(i);
	}
	vc<range_set<int>> a,b;
	vc<R> buf;
	vc<bool> alive;
	//x 座標 [0,n]
	rect_set(int n){
		init(n);
	}
	//FHC 2021 Final F
	void init(int n){
		s=1;while(s<n)s*=2;
		a.resize(s*2);
		b.resize(s*2);
	}
	//FHC 2021 Final F
	void clear(){
		a.clear();
		b.clear();
		buf.clear();
		alive.clear();
	}
	void add(R v){
		assert(v.valid());
		int d=si(buf);
		buf.pb(v);
		alive.pb(true);
		if(v.empty())return;
		X(v.xl,v.xr,[&](int i){
			a[i].add(v.yl,v.yr,d);
		});
		Y(v.xl,[&](int i){
			b[i].add(v.yl,v.yr,d);
		});
	}
	void del(int d){
		auto v=buf[d];
		assert(alive[d]);
		alive[d]=false;
		if(v.empty())return;
		X(v.xl,v.xr,[&](int i){
			a[i].del(v.yl,v.yr,d);
		});
		Y(v.xl,[&](int i){
			b[i].del(v.yl,v.yr,d);
		});
	}
	tuple<bool,R,int> collide(R v){
		assert(v.valid());
		bool has=false;
		R res;
		int idx;
		if(!v.empty()){
			Y(v.xl,[&](int i){
				if(!has){
					auto [f,l,r,j]=a[i].collide(v.yl,v.yr);
					if(f){
						has=true;
						res=buf[j];
						idx=j;
					}
				}
			});
			X(v.xl,v.xr,[&](int i){
				if(!has){
					auto [f,l,r,j]=b[i].collide(v.yl,v.yr);
					if(f){
						has=true;
						res=buf[j];
						idx=j;
					}
				}
			});
		}
		return mt(has,res,idx);
	}
};
