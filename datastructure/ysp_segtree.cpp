//CF616E
//strict-increasing な階段凸包の頂点数を数える
struct ysp_segtree{
	struct N{
		int cut,mx;
		N(int v=0):cut(0),mx(v){}
	};
	int s;
	vc<N> x;
	ysp_segtree(vi a){
		int n=si(a);
		s=1;
		while(s<n)s*=2;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	pi sub(int i,int v){
		if(x[i].mx<=v)return pi(0,v);
		if(i>=s)return pi(1,x[i].mx);
		if(v<x[i*2].mx){
			return pi(sub(i*2,v).a+x[i].cut,x[i].mx);
		}else{
			return sub(i*2+1,v);
		}
	}
	void upd(int i){
		assert(i<s);
		tie(x[i].cut,x[i].mx)=sub(i*2+1,x[i*2].mx);
	}
	void point_update(int i,int v){
		i+=s;
		x[i]=N(v);
		while(i>>=1)upd(i);
	}
	pi get(int b,int e,int v,int l,int r,int i){
		if(e<=l||r<=b)return pi(0,v);
		if(b<=l&&r<=e)return sub(i,v);
		int m=(l+r)/2;
		pi a=get(b,e,v,l,m,i*2);
		pi c=get(b,e,a.b,m,r,i*2+1);
		return pi(a.a+c.a,c.b);
	}
	//cnt,mx
	pi get(int b,int e,int v){
		assert(b<=e);
		return get(b,e,v,0,s,1);
	}
};


const int V=ten(9);

//increasing な階段凸包の面積を求める
//x 座標を V で切る
//UCup 1st Stage11(Shanghai) G
struct ysp_segtree{
	//葉ノードの cut には対応する点の x 座標を入れておく
	struct N{
		int cut,mx;
		N(pi v=pi(V,0)):cut(v.a),mx(v.b){}
	};
	int s;
	vc<N> x;
	ysp_segtree(vc<pi> a){
		int n=si(a);
		s=1;
		while(s<n)s*=2;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	//cut,mx
	//cut というのは，高さ v より上にある部分の面積
	pi sub(int i,int v){
		if(x[i].mx<=v)return pi(0,v);
		if(i>=s)return pi((x[i].mx-v)*(V-x[i].cut),x[i].mx);
		if(v<x[i*2].mx){
			return pi(sub(i*2,v).a+x[i].cut,x[i].mx);
		}else{
			return sub(i*2+1,v);
		}
	}
	void upd(int i){
		assert(i<s);
		tie(x[i].cut,x[i].mx)=sub(i*2+1,x[i*2].mx);
	}
	void point_set(int i,pi v){
		i+=s;
		x[i]=N(v);
		while(i>>=1)upd(i);
	}
	pi get(int b,int e,int v,int l,int r,int i){
		if(e<=l||r<=b)return pi(0,v);
		if(b<=l&&r<=e)return sub(i,v);
		int m=(l+r)/2;
		pi a=get(b,e,v,l,m,i*2);
		pi c=get(b,e,a.b,m,r,i*2+1);
		return pi(a.a+c.a,c.b);
	}
	//cnt,mx
	pi get(int b,int e,int v){
		assert(b<=e);
		return get(b,e,v,0,s,1);
	}
};

//UCUP 2-25-K
