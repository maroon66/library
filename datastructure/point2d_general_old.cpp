//PakenCamp2019 Day2 Jikka
template<class t,class F>
struct Point1D{
	SegTree<t,F> seg;
	vi pos;
	const t g;
	Point1D(F ff,t gg):seg(ff,gg),g(gg){}
	void addp(int p){
		pos.pb(p);
	}
	void init(){
		mkuni(pos);
		seg.init(vc<t>(pos.size(),g));
	}
	int idx(int p){
		return lwb(pos,p);
	}
	void updv(int p,t v){
		seg.upd(idx(p),v);
	}
	t get(int b,int e){
		return seg.get(idx(b),idx(e));
	}
};

//PakenCamp2019 Day2 Jikka
template<class t,class F>
struct Point2D{
	vc<Point1D<t,F>> buf;
	vi pos,xs,ys;
	const F f;
	const t g;
	int s;
	Point2D(F ff,t gg):f(ff),g(gg){}
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
		s=1;
		while(s<(int)pos.size())s*=2;
		rep(i,s*2)buf.emplace_back(f,g);
		rep(i,xs.size()){
			int j=lwb(pos,xs[i])+s;
			while(j>=1){
				buf[j].addp(ys[i]);
				j>>=1;
			}
		}
		for(auto&b:buf)b.init();
	}
	void updv(int x,int y,t v){
		int j=idx(x)+s;
		while(j>=1){
			buf[j].updv(y,v);
			j>>=1;
		}
	}
	t get(int b,int e,int p,int q,int l,int r,int i){
		if(e<=l||r<=b)return g;
		if(b<=l&&r<=e)return buf[i].get(p,q);
		int m=(l+r)/2;
		return f(get(b,e,p,q,l,m,i*2),get(b,e,p,q,m,r,i*2+1));
	}
	//[x1,x2)*[y1,y2)
	t get(int x1,int x2,int y1,int y2){
		return get(idx(x1),idx(x2),y1,y2,0,s,1);
	}
};


//以下定数倍高速化したやつ


//一番深いlogを非再帰にしたものだがそもそもコンパイルすら通るか知らない
/*
//VERIFY: yosupo
//CF Global3 H
//ARC073 F
//point update
template<class T,class F>
struct SegTree{
	vc<T> buf;
	int s;
	const F f;
	const T g;
	SegTree(F ff,T gg):f(ff),g(gg){}
	void init(const vc<T>& d){
		int n=d.size();
		s=1;
		while(s<n)s*=2;
		buf.resize(s*2,g);
		rep(i,n)
			buf[i+s]=d[i];
		gnr(i,1,s)
			u(i);
	}
	SegTree(const vc<T>& d,F ff,T gg):f(ff),g(gg){
		init(d);
	}
	void u(int i){
		buf[i]=f(buf[i*2],buf[i*2+1]);
	}
	void set(int i,T t){
		i+=s;
		buf[i]=t;
		while(i>>=1)u(i);
	}
	void upd(int i,T t){
		i+=s;
		buf[i]=f(buf[i],t);
		while(i>>=1)u(i);
	}
	T get(int b,int e){
		T lf=g,rt=g;
		for(int l=b+s,r=e+s;l<r;l>>=1,r>>=1){
			if (l&1){
				lf=f(lf,buf[l]);
				l++;
			}
			if (r&1){
				r--;
				rt=f(buf[r],rt);
			}
		}
		return f(lf,rt);
	}
};
template<class T,class F>
SegTree<T,F> segtree(const vc<T>& d,F f,T g){
	return SegTree<T,F>(d,f,g);
}
const auto imin=[](int a,int b){return min(a,b);};
const auto imax=[](int a,int b){return max(a,b);};
const auto iplus=[](int a,int b){return a+b;};

//PakenCamp2019 Day2 Jikka
template<class t,class F>
struct Point1D{
	SegTree<t,F> seg;
	vi pos;
	const t g;
	Point1D(F ff,t gg):seg(ff,gg),g(gg){}
	void addp(int p){
		pos.pb(p);
	}
	void init(){
		mkuni(pos);
		seg.init(vc<t>(pos.size(),g));
	}
	int idx(int p){
		return lwb(pos,p);
	}
	void updv(int p,t v){
		seg.upd(idx(p),v);
	}
	t get(int b,int e){
		return seg.get(idx(b),idx(e));
	}
};
*/

//BIT 的なのりで定数倍を軽くしようという魂胆
//CF Detlix Round Spring 2021
template<class t,class F>
struct Point1D{
	vc<t> buf;
	vi pos;
	const F f;
	const t g;
	Point1D(F ff,t gg):f(ff),g(gg){}
	void addp(int p){
		pos.pb(p);
	}
	void init(){
		mkuni(pos);
		//seg.init(vc<t>(pos.size(),g));
		buf.resize(si(pos),g);
	}
	int idx(int p){
		return lwb(pos,p);
	}
	void updv(int p,t v){
		//seg.upd(idx(p),v);
		for(int i=idx(p);i<si(pos);i+=(i+1)&(-i-1)){
			buf[i]=f(buf[i],v);
		}
	}
	t get(int,int e){
		//discard b
		//return seg.get(idx(b),idx(e));
		t res=g;
		for(int i=idx(e)-1;i>=0;i-=(i+1)&(-i-1)){
			res=f(res,buf[i]);
		}
		return res;
	}
};

//PakenCamp2019 Day2 Jikka
//CF Detlix Round Spring 2021
template<class t,class F>
struct Point2D{
	vc<Point1D<t,F>> buf;
	vi pos,xs,ys;
	const F f;
	const t g;
	int s;
	Point2D(F ff,t gg):f(ff),g(gg){}
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
		s=1;
		while(s<(int)pos.size())s*=2;
		rep(i,s*2)buf.emplace_back(f,g);
		rep(i,xs.size()){
			int j=lwb(pos,xs[i])+s;
			while(j>=1){
				buf[j].addp(ys[i]);
				j>>=1;
			}
		}
		for(auto&b:buf)b.init();
	}
	void updv(int x,int y,t v){
		int j=idx(x)+s;
		while(j>=1){
			buf[j].updv(y,v);
			j>>=1;
		}
	}
	//[x1,x2)*[y1,y2)
	t get(int x1,int x2,int y1,int y2){
		int b=idx(x1),e=idx(x2);
		t lf=g,rt=g;
		for(int l=b+s,r=e+s;l<r;l>>=1,r>>=1){
			if (l&1){
				lf=f(lf,buf[l].get(y1,y2));
				l++;
			}
			if (r&1){
				r--;
				rt=f(buf[r].get(y1,y2),rt);
			}
		}
		return f(lf,rt);
	}
};

