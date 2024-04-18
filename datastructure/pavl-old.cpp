//ARC030D
template<class N>
struct pavl{
	struct N2{
		N n;
		int l,r,d;
	};
	vc<N2> x;
	int nn(N w,int l=-1,int r=-1,int d=0){
		int i=x.size();
		x.pb(N2{w,l,r,d});
		return i;
	}
	int md(int i,N w){
		if(i==-1)return -1;
		return nn(w,x[i].l,x[i].r,x[i].d);
	}
	int np(int l,int r){
		return nn(N::merge(x[l].n,x[r].n),l,r,max(x[l].d,x[r].d)+1);
	}
	N gn(int i){
		return i==-1?N():x[i].n;
	}
	pair<N,N> push(int i,N w){
		N a=gn(x[i].l),b=gn(x[i].r);
		w.push(a);
		w.push(b);
		return {a,b};
	}
	pi m1(int a,int b,int c){
		if(abs(max(x[a].d,x[b].d)+1-x[c].d)<=1)
			return pi(np(a,b),c);
		if(abs(max(x[b].d,x[c].d)+1-x[a].d)<=1)
			return pi(a,np(b,c));
		auto e=push(b,x[b].n);
		return pi(np(a,md(x[b].l,e.a)),np(md(x[b].r,e.b),c));
	}
	pi m2(int a,int b,N c,N d){
		if(x[a].d<x[b].d){
			auto e=push(b,d);
			pi z=m2(a,x[b].l,c,e.a);
			return m1(z.a,z.b,md(x[b].r,e.b));
		}else if(x[a].d>x[b].d){
			auto e=push(a,c);
			pi z=m2(x[a].r,b,e.b,d);
			return m1(md(x[a].l,e.a),z.a,z.b);
		}else
			return pi(md(a,c),md(b,d));
	}
	int mr(int a,int b){
		if(a==-1)return b;
		if(b==-1)return a;
		pi z=m2(a,b,x[a].n,x[b].n);
		return np(z.a,z.b);
	}
	template<class F,class G>
	pi sp(int i,N w,F f,G g){
		if(x[i].l==-1){
			if((w.*f)(g))return pi(-1,md(i,w));
			else return pi(md(i,w),-1);
		}
		auto a=push(i,w);
		if((a.a.*f)(g)){
			pi b=sp(x[i].l,a.a,f,g);
			return pi(b.a,mr(b.b,md(x[i].r,a.b)));
		}else{
			pi b=sp(x[i].r,a.b,f,g);
			return pi(mr(md(x[i].l,a.a),b.a),b.b);
		}
	}
	template<class F,class G>
	pi sp(int i,F f,G g){return sp(i,x[i].n,f,g);}
	template<class G>
	pi sp(int i,G g){return sp(i,&N::split,g);}
	int cl(int i){return md(i,x[i].n);}
	void dfs(int i,N w,vc<N>&v){
		if(x[i].l==-1){
			v.pb(w);
			return;
		}
		auto a=push(i,w);
		dfs(x[i].l,a.a,v);
		dfs(x[i].r,a.b,v);
	}
	vc<N> tr(int i){
		vc<N> v;
		dfs(i,x[i].n,v);
		return v;
	}
	int bl(const vc<N>&v){
		vi idx;
		for(auto n:v)idx.pb(nn(n));
		int k;
		while((k=idx.size())>1){
			if(k%2)idx.pb(-1),k++;
			vi nx;
			rep(i,k/2)
				nx.pb(mr(idx[i*2],idx[i*2+1]));
			idx=nx;
		}
		return idx[0];
	}
	int rc(int r){
		auto v=tr(r);
		x.clear();
		return bl(v);
	}
	int mem(){
		return x.size()*sizeof(N2);
	}
	void res(int m){
		x.reserve(m/sizeof(N2)+1000);
	}
};
