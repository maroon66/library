//CF875F
//x,y の列があって，区間を選んで y+=x をして y の max　を返してもらう
//ついでに y の和 (ys) も計算 (UCUP1-21 D)
struct Kinetic{
	pi mx;
	int xs,ys,t,lz;
	Kinetic(pi v=none):mx(v),
		xs(v==none?0:v.a),ys(v==none?0:v.b),t(inf),lz(0){}
	bool add(int v){
		if(t<v)return false;
		if(mx!=none)mx.b+=mx.a*v;
		ys+=xs*v;
		t-=v;
		lz+=v;
		return true;
	}
	void push(Kinetic&x,Kinetic&y){
		bool tmp;
		tmp=x.add(lz);assert(tmp);
		tmp=y.add(lz);assert(tmp);
		lz=0;
	}
	static Kinetic merge(const Kinetic&x,const Kinetic&y){
		pi a=x.mx,b=y.mx;
		if(a.b<b.b)swap(a,b);
		Kinetic res(a);
		res.xs=x.xs+y.xs;
		res.ys=x.ys+y.ys;
		res.t=min(x.t,y.t);
		if(a.a<b.a)chmin(res.t,(a.b-b.b)/(b.a-a.a));
		return res;
	}
	bool okmax(int v){
		return mx.b<v;
	}
};
