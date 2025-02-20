struct MaxNode{
	pi v;
	MaxNode(pi vv=pi(-inf,-1)):v(vv){}
	static MaxNode merge(const MaxNode&a,const MaxNode&b){
		return MaxNode(max(a.v,b.v));
	}
};

//区間の追加（ついでに index v をつける，v が distinct でないとだめ）
//区間でクエリ，共通部分が正の区間 があればいずれかの v を返す
//UCUP 2-20-O
struct range_set{
	vc<pi> lv;
	segtree<MaxNode> seg;
	range_set(){}
	void add_pre(int l,int,int v){
		lv.eb(l,v);
	}
	void init(){
		soin(lv);
		seg.resize(si(lv));
	}
	void add(int l,int r,int v){
		int i=lwb(lv,pi(l,v));
		seg.point_set(i,pi(r,i));
	}
	void del(int l,int,int v){
		int i=lwb(lv,pi(l,v));
		seg.point_set(i,MaxNode());
	}
	int collide(int b,int e){
		auto w=seg.composite(0,lwb(lv,pi(e,-1))).v;
		if(w.a>b)return lv[w.b].b;
		else return -1;
	};
};

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
	bool contain(const R&a)const{
		return xl<=a.xl&&a.xr<=xr&&yl<=a.yl&&a.yr<=yr;
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

//長方形でクエリ，共通部分が正の長方形があればいずれかの (R,id) を返す
//UCUP 2-20-O
//stress-tested
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
	vc<range_set> a,b;
	vc<R> buf;
	vc<bool> alive;
	//x 座標 [0,V)
	rect_set(int V,const vc<R>&ini):buf(ini),alive(si(ini),false){
		s=1;while(s<V)s*=2;
		a.resize(s*2);
		b.resize(s*2);
		
		rep(d,si(buf)){
			R v=buf[d];
			if(v.empty())continue;
			assert(inc(0,v.xl,V-1));
			assert(inc(0,v.xr,V-1));
			X(v.xl,v.xr,[&](int i){
				a[i].add_pre(v.yl,v.yr,d);
			});
			Y(v.xl,[&](int i){
				b[i].add_pre(v.yl,v.yr,d);
			});
		}
		
		rng(i,1,s*2){
			a[i].init();
			b[i].init();
		}
	}
	void add(int d){
		R v=buf[d];
		assert(v.valid());
		assert(!alive[d]);
		alive[d]=true;
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
					int j=a[i].collide(v.yl,v.yr);
					if(j!=-1){
						has=true;
						res=buf[j];
						idx=j;
					}
				}
			});
			X(v.xl,v.xr,[&](int i){
				if(!has){
					int j=b[i].collide(v.yl,v.yr);
					if(j!=-1){
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
