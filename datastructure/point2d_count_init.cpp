
template<class t>
struct Point1D{
	BIT<t> bit;
	vi pos;
	vc<t> raw;
	void reserve(int s){
		pos.reserve(s);
		raw.reserve(s);
	}
	void addpre(int p,t v){
		pos.pb(p);
		raw.pb(v);
	}
	void init(){
		assert(is_sorted(all(pos)));
		bit.init(raw);
	}
	int idx(int p){
		return lwb(pos,p);
	}
	void addv(int p,t v){
		bit.add(idx(p),v);
	}
	t sum(int b,int e){
		return bit.sum(idx(b),idx(e));
	}
	//[-inf,p)
	t getpref(int p){
		return bit.get(idx(p)-1);
	}
};

//point2d with init
//CF875E
template<class t>
struct Point2D{
	vc<Point1D<t>> buf;
	vi pos,xs,ys;
	vc<t> vs;
	void addpre(int x,int y,t v){
		xs.pb(x);
		ys.pb(y);
		vs.pb(v);
	}
	int idx(int p){
		return lwb(pos,p);
	}
	void init(){
		pos=xs;
		mkuni(pos);
		buf.resize(pos.size());
		vi ord=vid(si(xs));
		sort(all(ord),[&](int i,int j){return ys[i]<ys[j];});
		vi ss(si(buf));
		for(auto x:xs)
			for(int j=idx(x);j<si(buf);j+=(j+1)&(-j-1))
				ss[j]++;
		rep(i,si(buf))buf[i].reserve(ss[i]);
		for(auto i:ord)
			for(int j=idx(xs[i]);j<si(buf);j+=(j+1)&(-j-1))
				buf[j].addpre(ys[i],vs[i]);
		for(auto&b:buf)b.init();
	}
	void addv(int x,int y,t v){
		for(int j=idx(x);j<int(buf.size());j+=(j+1)&(-j-1))
			buf[j].addv(y,v);
	}
	//[x1,x2)*[y1,y2)
	t rect(int x1,int x2,int y1,int y2){
		t res=0;
		for(int j=idx(x1)-1;j>=0;j-=(j+1)&(-j-1))
			res-=buf[j].sum(y1,y2);
		for(int j=idx(x2)-1;j>=0;j-=(j+1)&(-j-1))
			res+=buf[j].sum(y1,y2);
		return res;
	}
	//NOT VERIFIED
	//[-inf,x)*[-inf,y)
	t getpref(int x,int y){
		t res=0;
		for(int j=idx(x)-1;j>=0;j-=(j+1)&(-j-1))
			res+=buf[j].getpref(y);
		return res;
	}
};
