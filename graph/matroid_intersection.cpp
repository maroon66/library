//verify: https://open.kattis.com/problems/rainbowgraph
template<class L,class R>
struct matroid_intersection{
	using T=tuple<int,int,int>;
	L l;
	R r;
	int n,m,s;
	vc<pi> d;
	vi h,pv,pe;
	vvc<T> g;
	vc<T> es;
	vc<char> use;
	matroid_intersection(L a,R b):l(a),r(b),
		n(si(l)),m(si(r)),s(n+m),
		d(s),h(s),pv(s),pe(s),
		g(s)
	{}
	void ae(int i,int j,int c){
		assert(c>=0);
		es.eb(i,j,c);
		use.pb(0);
	}
	int augument(){
		fill(all(d),pi(inf,-1));
		priority_queue<T,vc<T>,greater<T>> pq;
		auto reach=[&](int v,int dist,int step,int pre_v,int pre_e){
			if(chmin(d[v],pi(dist,step))){
				pv[v]=pre_v;
				pe[v]=pre_e;
				pq.emplace(dist,step,v);
			}
		};
		rep(i,n)if(l.isfree(i))reach(i,0,0,-1,-1);
		int x=-1;
		while(si(pq)){
			auto [dist,step,v]=pq.top();pq.pop();
			if(d[v]!=pi(dist,step))continue;
			if(n<=v&&r.isfree(v-n)){
				x=v;
				break;
			}
			for(auto [to,c,i]:g[v]){
				if((v<n&&!use[i])||(n<=v&&use[i])){
					c+=h[v]-h[to]; assert(c>=0);
					reach(to,dist+c,step+1,v,i);
				}
			}
			rep(to,s)if((v<n)==(to<n)){
				int a=dist+h[v]-h[to],b=step+1;
				if(pi(a,b)<d[to]){
					if((v<n&&l.canswap(v,to))
					|| (n<=v&&r.canswap(to-n,v-n)))
						reach(to,a,b,v,-1);
				}
			}
		}
		if(x==-1)return inf;
		rep(i,s)h[i]=min(inf,h[i]+min(d[i].a,d[x].a));
		int res=h[x];
		while(pv[x]!=-1){
			if(pe[x]!=-1){
				auto [a,b,c]=es[pe[x]];
				l.flip(a);
			 	r.flip(b);
			 	use[pe[x]]^=1;
			}
			x=pv[x];
		}
		return res;
	}
	//最大サイズまで広げていく
	//最小コスト列を求める
	vi calc(){
		rep(i,si(es)){
			auto [a,b,c]=es[i];
			g[a].eb(n+b,c,i);
			g[n+b].eb(a,-c,i);
		}
		vi res{0};
		int f;
		while((f=augument())<inf){
			res.pb(res.back()+f);
		}
		return res;
	}
};

//verify: https://open.kattis.com/problems/rainbowgraph
const pi none(-1,-1);
struct dual_graphic{
	int n;
	vc<pi> es;
	int m;
	unionfind uf;
	vc<char> use,tmp;
	dual_graphic(int nn,vc<pi> ee):n(nn),es(ee),m(si(es)),uf(n),use(m),tmp(m){}
	void flip(int i){use[i]^=1;}
	int size()const{return m;}//台集合のサイズを返す
	bool tmpok(){
		uf.clear();
		rep(i,m)if(!tmp[i]&&es[i]!=none)
			uf.unite(es[i].a,es[i].b);
		return uf.c==1;
	}
	//add i
	bool isfree(int i){
		if(use[i])return false;
		
		copy(all(use),tmp.bg);
		tmp[i]=true;
		return tmpok();
	}
	//del i, and j
	bool canswap(int i,int j){
		if(!use[i]||use[j])return false;
		
		copy(all(use),tmp.bg);
		tmp[i]=false;
		tmp[j]=true;
		return tmpok();
	}
};

//base のサイズを最大化
//その上でコストを最小化
//CF Hello2020
//CF Deltix Summer 2021 H
template<class LM,class RM>
struct MatroidIntersection{
	int n,m,s;
	LM& lm;
	RM& rm;
	struct Edge{
		int to,cost,id;
		friend ostream& operator<<(ostream&os,const Edge&e){
			return os<<"{T:"<<e.to<<", C:"<<e.cost<<", I:"<<e.id<<"}";
		}
	};
	vc<tuple<int,int,int>> es;
	vvc<Edge> g;
	MatroidIntersection(LM& llm,RM& rrm):lm(llm),rm(rrm){
		n=lm.size();
		m=rm.size();
		s=n+m;
	}
	void AddEdge(int a,int b,int c){
		assert(0<=a&&a<n);
		assert(0<=b&&b<m);
		assert(0<=c);
		es.eb(a,b,c);
	}
	vi h;
	vc<pi> dist,prev;
	void FindShortest(const vi&src){
		dist.assign(s,pi(inf,-1));
		prev.assign(s,pi(-1,-1));
		
		using Node=tuple<int,int,int>;
		priority_queue<Node,vc<Node>,greater<Node>> pq;
		const auto Reach=[&](int v,int d,int step,int j,int k){
			if(pi(d,step)<dist[v]){
				dist[v]=pi(d,step);
				prev[v]=pi(j,k);
				pq.push(make_tuple(d,step,v));
			}
		};

		for(auto i:src)
			Reach(i,0,0,-1,-1);
		
		while(!pq.empty()){
			int d,step,v;
			tie(d,step,v)=pq.top();
			pq.pop();
			if(dist[v]!=pi(d,step))
				continue;
			for(auto e:g[v]){
				assert(e.cost+h[v]-h[e.to]>=0);
				Reach(e.to,d+e.cost+h[v]-h[e.to],step+1,v,e.id);
			}
		}
	}
	int Augment(vc<bool>&use){
		g.assign(s,vc<Edge>());
		vi m1,m2;
		rep(i,es.size()){
			int a,b,c;
			tie(a,b,c)=es[i];
			if(use[i]){
				g[n+b].pb(Edge{a,-c,i});
				m1.pb(a);
				m2.pb(b);
			}else{
				g[a].pb(Edge{n+b,c,i});
			}
		}
		vi src,dst;
		{
			auto w=lm.Get(m1);
			src=w.first;
			if(src.empty())return -1;
			for(auto e:w.second)
				g[e.first].pb(Edge{e.second,0,-1});
		}
		{
			auto w=rm.Get(m2);
			dst=w.first;
			if(dst.empty())return -1;
			for(auto e:w.second)
				g[n+e.second].pb(Edge{n+e.first,0,-1});
		}
		
		FindShortest(src);
		
		pair<pi,int> w(pi(inf/2,-1),-1);
		for(auto i:dst)
			chmin(w,mp(dist[n+i],n+i));
		if(w.second==-1)
			return -1;
		int x=w.second;
		while(prev[x].first!=-1){
			int i=prev[x].second;
			if(i!=-1)
				use[i]=!use[i];
			x=prev[x].first;
		}
		int mn=w.first.first;
		rep(i,s){
			h[i]+=min(dist[i].first,mn);
			chmin(h[i],inf);
		}
		return h[w.second];
	}
	pair<int,vi> Solve(){
		h.assign(s,0);
		vc<bool> use(es.size());
		int c=0,f;
		while((f=Augment(use))>=0)
			c+=f;
		vi sol;
		rep(i,es.size())
			if(use[i])
				sol.pb(i);
		return mp(c,sol);
	}
};
//Matroid
//size(): returns the size of the universe
//Get(vi cur): 
//given is a set of indices of currently used elements.
//returns pair of
//1. indices of src(vertices that it is free to add)
//2. all (x,y) s.t. del x & add y is possible (y is not a src)
//practically it runs with low constants (why?)

//CF Deltix Summer 2021 H
//全域木のマトロイド，ただし必ず使う辺をいくつか指定されている．
//disjoint な頂点の次数制約のマトロイドは欲しければ当該コンテストの提出を参照
struct unionfind{
	vi p,s;
	int c;
	unionfind(int n=0):p(n,-1),s(n,1),c(n){}
	void init(int n){
		p.clear();p.resize(n,-1);
		s.clear();s.resize(n,1);
		c=n;
	}
	int find(int a){
		return p[a]==-1?a:(p[a]=find(p[a]));
	}
	//set b to a child of a
	bool unite(int a,int b){
		a=find(a);
		b=find(b);
		if(a==b)return false;
		p[b]=a;
		s[a]+=s[b];
		c--;
		return true;
	}
	bool same(int a,int b){
		return find(a)==find(b);
	}
	int sz(int a){
		return s[find(a)];
	}
};
struct MST{
	int n;
	vc<pi> base,es;
	MST(int nn,vc<pi> bb,vc<pi> ee){
		n=nn;
		base=bb;
		es=ee;
	}
	int size(){
		return es.size();
	}
	unionfind uf;
	void ufinit(){
		uf.init(n);
		for(auto [a,b]:base){
			uf.unite(a,b);
		}
	}
	pair<vi,vc<pi>> Get(const vi&cur){
		ufinit();
		vi t(size(),0);
		for(auto i:cur){
			uf.unite(es[i].first,es[i].second);
			t[i]=1;
		}
		vi src;
		rep(i,size())if(!t[i]){
			if(!uf.same(es[i].first,es[i].second)){
				src.pb(i);
				t[i]=2;
			}
		}
		vc<pi> res;
		rep(i,size())if(t[i]==1){
			ufinit();
			rep(j,size())if(t[j]==1&&j!=i)
				uf.unite(es[j].first,es[j].second);
			rep(j,size())if(!t[j])
				if(!uf.same(es[j].first,es[j].second))
					res.eb(i,j);
		}
		return mp(src,res);
	}
};
struct Free{
	const int n;
	Free(int nn):n(nn){}
	int size(){return n;}
	pair<vi,vc<pi>> Get(const vi&cur){
		vc<bool> used(n);
		for(auto i:cur)used[i]=true;
		vi res;
		rep(i,n)if(!used[i])res.pb(i);
		return mp(res,vc<pi>());
	}
};



struct UnionFind{
	vi rank,par;
	void Init(int n){
		rank.assign(n,0);
		par.assign(n,-1);
	}
	int Find(int i){
		return par[i]==-1?i:(par[i]=Find(par[i]));
	}
	bool Same(int a,int b){
		return Find(a)==Find(b);
	}
	bool Unite(int a,int b){
		a=Find(a);
		b=Find(b);
		if(a==b)
			return false;
		if(rank[a]<rank[b])
			swap(a,b);
		par[b]=a;
		if(rank[a]==rank[b])
			rank[a]++;
		return true;
	}
};

struct MST{
	int n;
	vector<pi> es;
	MST(int nn){
		n=nn;
	}
	void AddEdge(int a,int b){
		assert(0<=a&&a<n);
		assert(0<=b&&b<n);
		es.eb(a,b);
	}
	int size(){
		return es.size();
	}
	UnionFind uf;
	pair<vi,vc<pi>> Get(const vi&cur){
		uf.Init(n);
		vi t(size(),0);
		for(auto i:cur){
			uf.Unite(es[i].first,es[i].second);
			t[i]=1;
		}
		vi src;
		rep(i,size())if(!t[i]){
			if(!uf.Same(es[i].first,es[i].second)){
				src.pb(i);
				t[i]=2;
			}
		}
		vc<pi> res;
		rep(i,size())if(t[i]==1){
			uf.Init(n);
			rep(j,size())if(t[j]==1&&j!=i)
				uf.Unite(es[j].first,es[j].second);
			rep(j,size())if(!t[j])
				if(!uf.Same(es[j].first,es[j].second))
					res.eb(i,j);
		}
		return mp(src,res);
	}
};
