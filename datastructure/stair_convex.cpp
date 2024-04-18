//XXII Opencup GP of Daejeon H,D
//lower right
struct LR{
	static const int none=inf;
	int mn,vis;
	LR(int v=none):mn(v),vis(v&1?v:none){}
	static LR merge(const LR&x,const LR&y){
		LR res(min(x.mn,y.mn));
		if(x.vis<y.mn)res.vis=x.vis;
		else res.vis=y.vis;
		return res;
	}
};
//CGR20 I
//upper left
struct UL{
	static const int none=-inf;
	int mx,vis;
	UL(int v=none):mx(v),vis(v&1?v:none){}
	static UL merge(const UL&x,const UL&y){
		UL res(max(x.mx,y.mx));
		if(x.mx<y.vis)res.vis=y.vis;
		else res.vis=x.vis;
		return res;
	}
};
ARC165F 2023 09 17
//lower left
struct LL{
	static const int none=inf;
	int mn,vis;
	LL(int v=none):mn(v),vis(v&1?v:none){}
	static LL merge(const LL&x,const LL&y){
		LL res(min(x.mn,y.mn));
		if(x.mn>y.vis)res.vis=y.vis;
		else res.vis=x.vis;
		return res;
	}
};

//同じx,y座標のときのタイブレークに注意したほうがいい
//自然な問題では無視できるケースが大半だとは思うが…
pi revxy(const pi&a){return pi(a.b,a.a);}
template<class Active>
struct stair_convex{
	const int n;
	vc<pi> xy,yx;
	vi hei;
	segtree<Active> act;
	set<pi> xi,yi;
	//rw must be all distnct
	stair_convex(const vc<pi>&rw):n(si(rw)),xy(rw),yx(n){
		sort(all(xy));
		rep(i,n)yx[i]=revxy(xy[i]);
		sort(all(yx));
		hei.resize(n);
		rep(i,n)hei[i]=2*lwb(yx,revxy(xy[i]))+1;
		act=segtree<Active>(hei);
		xi.emplace(inf,n);
		yi.emplace(inf,n);
	}
	void destroy(const pi&a){
		int i=lwb(xy,a);
		act.point_set(i,Active::none);
		xi.erase(pi(a.a,i));
		yi.erase(pi(a.b,i));
	}
	//sc 内部での index を返す
	int xcut(int x){
		auto itr=xi.lower_bound(pi(x,-inf));
		return itr->b;
	}
	//これ多分 LowerRight(階段上の点が x,y 昇順) でないと意味ない
	int ycut(int y){
		auto itr=yi.lower_bound(pi(y,-inf));
		return itr->b;
	}
	vc<pi> enumerate_active(){
		vc<pi> res;
		while(1){
			int w=act.getall().vis;
			if(w==Active::none)break;
			pi a=revxy(yx[w/2]);
			res.pb(a);
			int i=lwb(xy,a);
			act.point_set(i,hei[i]-1);
			xi.emplace(a.a,i);
			yi.emplace(a.b,i);
		}
		return res;
	}
	int getid(const pi&a){
		return lwb(xy,a);
	}
	int size()const{return n;}
};
