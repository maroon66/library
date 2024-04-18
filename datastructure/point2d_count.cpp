//#define POINT_ADD_RANGE_GET
//Codechef 2022 March Cookoff PALQUE (range add)

template<class t>
struct Point1D{
	BIT<t> bit;
	vi pos;
	void addp(int p){
		pos.pb(p);
	}
	void init(){
		mkuni(pos);
		bit.init(pos.size());
	}
	int idx(int p){
		return lwb(pos,p);
	}
	#ifdef POINT_ADD_RANGE_GET
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
	#else
	//[b,e)
	void addrange(int b,int e,t v){
		bit.add(idx(b),v);
		bit.add(idx(e),-v);
	}
	t getpoint(int p){
		return bit.get(idx(p));
	}
	#endif
};

template<class t>
struct Point2D{
	vc<Point1D<t>> buf;
	vi pos,xs,ys;
	void addp(int x,int y){
		xs.pb(x);
		ys.pb(y);
	}
	int idx(int p){
		return lwb(pos,p);
	}
	void init(){
		pos=xs;
		mkuni(pos);
		buf.resize(pos.size());
		rep(i,xs.size())
			#ifdef POINT_ADD_RANGE_GET
			for(int j=idx(xs[i]);j<int(buf.size());j+=(j+1)&(-j-1))
			#else
			for(int j=idx(xs[i]);j>=0;j-=(j+1)&(-j-1))
			#endif
				buf[j].addp(ys[i]);
		for(auto&b:buf)b.init();
	}
	#ifdef POINT_ADD_RANGE_GET
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
	#else
	void addrect(int x1,int x2,int y1,int y2,t v){
		for(int j=idx(x1);j<int(buf.size());j+=(j+1)&(-j-1))
			buf[j].addrange(y1,y2,v);
		for(int j=idx(x2);j<int(buf.size());j+=(j+1)&(-j-1))
			buf[j].addrange(y1,y2,-v);
	}
	t getpoint(int x,int y){
		t res=0;
		for(int j=idx(x);j>=0;j-=(j+1)&(-j-1))
			res+=buf[j].getpoint(y);
		return res;
	}
	#endif
};
