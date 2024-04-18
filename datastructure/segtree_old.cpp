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
	T get(int b,int e,int l,int r,int i){
		if(e<=l||r<=b)return g;
		if(b<=l&&r<=e)return buf[i];
		int m=(l+r)/2;
		return f(get(b,e,l,m,i*2),get(b,e,m,r,i*2+1));
	}
	T get(int b,int e){
		return get(b,e,0,s,1);
	}
};
template<class T,class F>
SegTree<T,F> segtree(const vc<T>& d,F f,T g){
	return SegTree<T,F>(d,f,g);
}
const auto imin=[](int a,int b){return min(a,b);};
const auto imax=[](int a,int b){return max(a,b);};
const auto iplus=[](int a,int b){return a+b;};

//VERIFY: yosupo
template<class N>
struct segtree{
	vc<N> buf;
	int s;
	template<class t>
	segtree(vc<t> a){
		int n=a.size();
		s=1;
		while(s<n){s*=2;}
		buf.resize(s*2);
		rep(i,n)
			buf[s+i]=N(a[i]);
		gnr(i,1,s)
			buf[i]=N::merge(buf[i*2],buf[i*2+1]);
	}
	void set(int i,const N&t){
		i+=s;
		buf[i]=t;
		while(i>>=1)buf[i]=N::merge(buf[i*2],buf[i*2+1]);
	}
	N query(int b,int e,int l,int r,int i){
		if(e<=l||r<=b)return N();
		if(b<=l&&r<=e)return buf[i];
		return N::merge(query(b,e,l,(l+r)/2,i*2),query(b,e,(l+r)/2,r,i*2+1));
	}
	N query(int b,int e){
		return query(b,e,0,s,1);
	}
	N getall(){
		return buf[1];
	}
};

