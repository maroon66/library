//長さ N の数列を持っている
//[l,r) の値を set する，という操作が可能
//書き換えられる部分について，同じ値の連続区間に関してなんか関数を呼んでくれる
//(JPN Mirror) PKU Contest 2, PTZ Summer 2022 Day 5 D
struct set_segtree{
	const int none=-inf;
	int s;
	vi x,y,z;
	set_segtree(int n){
		s=1;
		while(s<n)s*=2;
		x.resize(s*2);
		y.resize(s*2);
		z.resize(s*2,none);
	}
	void set_node(int i,int v){
		x[i]=y[i]=z[i]=v;
	}
	void push(int i){
		if(z[i]!=none){
			set_node(i*2,z[i]);
			set_node(i*2+1,z[i]);
			z[i]=none;
		}
	}
	void upd(int i){
		x[i]=max(x[i*2],x[i*2+1]);
		y[i]=min(y[i*2],y[i*2+1]);
	}
	vc<tuple<int,int,int>> buf;
	void dfs(int i,int l,int r,int b,int e,int v){
		if(e<=l||r<=b)return;
		if(b<=l&&r<=e&&x[i]==y[i]){
			buf.eb(l,r,x[i]);
			set_node(i,v);
		}else{
			push(i);
			int m=(l+r)/2;
			dfs(i*2,l,m,b,e,v);
			dfs(i*2+1,m,r,b,e,v);
			upd(i);
		}
	}
	template<class F>
	void set_range(int b,int e,int v,F f){
		buf.clear();
		dfs(1,0,s,b,e,v);
		int len=0;
		rep(i,si(buf)){
			auto [l,r,w]=buf[i];
			if(len&&get<1>(buf[len-1])==l&&get<2>(buf[len-1])==w){
				get<1>(buf[len-1])=r;
			}else{
				buf[len++]=buf[i];
			}
		}
		buf.resize(len);
		for(auto [l,r,w]:buf)f(l,r,w);
	}
};
