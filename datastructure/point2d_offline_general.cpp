//x 座標 [-inf,x], y 座標 [l,r) の点を集める
template<class N>
struct point2d{
	struct point{
		int x,y;
		N v;
		bool operator<(const point&rhs)const{
			return x<rhs.x;
		}
	};
	struct query{
		int x,l,r,i;
		bool operator<(const query&rhs)const{
			return x<rhs.x;
		}
	};
	vc<point> ps;
	vc<query> qs;
	void ap(int x,int y,N v){
		ps.pb({x,y,v});
	}
	void aq(int x,int l,int r){
		int i=si(qs);
		qs.pb({x,l,r,i});
	}
	vc<N> calc(){
		sort(all(ps));
		sort(all(qs));
		vi ys;
		for(auto [x,y,v]:ps)ys.pb(y);
		mkuni(ys);
		segtree<N> seg(si(ys));
		int j=0;
		vc<N> ans(si(qs));
		for(auto [x,l,r,i]:qs){
			while(j<si(ps)&&ps[j].x<=x){
				int y=lwb(ys,ps[j].y);
				seg.point_merge(y,ps[j].v);
				j++;
			}
			l=lwb(ys,l);
			r=lwb(ys,r);
			ans[i]=seg.composite(l,r);
		}
		return ans;
	}
};
