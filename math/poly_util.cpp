template<class D>
struct PolyBuf{
	const vc<D>&xs;
	PolyBuf(const vc<D>& xxs):xs(xxs){}
	using P=Poly<D>;
	map<pi,P> buf;
	const P& get(int l,int r){
		if(buf.count(pi(l,r)))return buf[pi(l,r)];
		if(r-l==1){
			return buf[pi(l,r)]=P{-xs[l],1};
		}else{
			int m=(l+r)/2;
			return buf[pi(l,r)]=get(l,m)*get(m,r);
		}
	}
};

template<class D>
vc<D> multi_eval(const Poly<D>&f,const vc<D>& xs,PolyBuf<D>& buf){
	const int n=xs.size();
	vc<D> res(n);
	static const int B=256;
	function<void(int,int,const Poly<D>&)> rec=[&](int l,int r,const Poly<D>&p){
		if(r-l<=B){
			rng(i,l,r)
				res[i]=p.eval(xs[i]);
		}else{
			int m=(l+r)/2;
			rec(l,m,p%buf.get(l,m));
			rec(m,r,p%buf.get(m,r));
		}
	};
	rec(0,n,f%buf.get(0,n));
	return res;
}

//Petrozavodsk 2019w Day1 I
//yosupo judge
template<class D>
vc<D> multi_eval(const Poly<D>&f,const vc<D>& xs){
	PolyBuf<D> buf(xs);
	return multi_eval(f,xs,buf);
}

//ARC033 D
//yosupo judge
template<class D>
Poly<D> interpolate(const vc<D>& xs,const vc<D>& ys){
	assert(xs.size()==ys.size());
	PolyBuf<D> buf(xs);
	Poly<D> w=buf.get(0,xs.size()).dif();
	vc<D> vs=multi_eval(w,xs,buf);
	function<Poly<D>(int,int)>rec=[&](int l,int r){
		if(r-l==1)
			return Poly<D>{ys[l]/vs[l]};
		int m=(l+r)/2;
		return rec(l,m)*buf.get(m,r)+rec(m,r)*buf.get(l,m);
	};
	return rec(0,xs.size());
}
