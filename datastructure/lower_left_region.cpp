//(x,y) のペアを管理
//今までのやつが作る階段凸法の面積
//左下の面積ね
//(1,1) を入れると 1 が返ってくる
//非負整数以外を入れると何が起こるかわからない
struct lower_left_region{
	struct P{
		int x,y;
		bool operator<(const P&rhs)const{
			return x!=rhs.x?x<rhs.x:y>rhs.y;
		}
	};
	set<P> s;
	int area=0;
	void add(int x,int y){
		auto itr=s.lower_bound(P{x,y});
		P l{0,0},r{0,0};
		if(itr!=s.ed){
			if(y<=itr->y)return;
			r=*itr;
		}
		P pre=r;
		while(itr!=s.bg){
			auto p=prev(itr);
			area+=p->x*pre.y;
			if(p->y>y){
				l=*p;
				break;
			}
			area-=p->x*p->y;
			pre=*p;
			s.erase(p);
		}
		s.insert(itr,P{x,y});
		area+=x*y;
		area-=l.x*y;
		area-=x*r.y;
	}
};
