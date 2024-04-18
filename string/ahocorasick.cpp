//CF709F
//idx がノードの BFS 順序 (index 順だと fail 周りで壊れる)
//pos が各文字列の最後に対応するノード
struct Aho{
	struct N{
		map<char,int> nx;
		int fail;
		N():fail(-1){}
	};
	vc<N> x;
	vi idx,pos;
	void dfs(int cur,const string&s,int i){
		if(i<(int)s.size()){
			if(x[cur].nx.count(s[i])==0){
				x[cur].nx[s[i]]=x.size();
				x.eb();
			}
			dfs(x[cur].nx[s[i]],s,i+1);
		}else pos.pb(cur);
	}
	Aho(const vc<string>&ss):x(1){
		for(auto&s:ss)dfs(0,s,0);
		idx.pb(0);
		int h=0;
		while(h<(int)idx.size()){
			int i=idx[h++];
			for(auto e:x[i].nx){
				int j=e.b,f=x[i].fail;
				while(f!=-1&&x[f].nx.count(e.a)==0)
					f=x[f].fail;
				if(f!=-1)
					x[j].fail=x[f].nx[e.a];
				else
					x[j].fail=0;
				idx.pb(j);
			}
		}
	}
	N& operator[](int i){return x[i];}
};

//CF709F
//XXII Opencup GP of EDG H
//idx がノードの BFS 順序 (index 順だと fail 周りで壊れる)
//pos が各文字列の最後に対応するノード
template<int K>
struct Aho{
	struct N{
		int nx[K];
		int fail;
		N():fail(-1){one(nx);}
	};
	vc<N> x;
	vi idx,pos;
	template<class S>
	void dfs(int cur,const S&s,int i){
		if(i<(int)s.size()){
			if(x[cur].nx[s[i]]==-1){
				x[cur].nx[s[i]]=x.size();
				x.eb();
			}
			dfs(x[cur].nx[s[i]],s,i+1);
		}else pos.pb(cur);
	}
	template<class S>
	Aho(const vc<S>&ss):x(1){
		x.reserve(500010);
		idx.reserve(500010);
		pos.reserve(si(ss));
		for(auto&s:ss)dfs(0,s,0);
		idx.pb(0);
		int h=0;
		while(h<(int)idx.size()){
			int i=idx[h++];
			rep(c,K)if(x[i].nx[c]!=-1){
				int j=x[i].nx[c],f=x[i].fail;
				while(f!=-1&&x[f].nx[c]==-1)
					f=x[f].fail;
				if(f!=-1)
					x[j].fail=x[f].nx[c];
				else
					x[j].fail=0;
				idx.pb(j);
			}
		}
	}
	N& operator[](int i){return x[i];}
};

//idx がノードの BFS 順序 (index 順だと fail 周りで壊れる)
//pos が各文字列の最後に対応するノード
//nx[c] は failure-link を辿れるだけたどり nx に遷移した先を表す
//ARC141F
//CF857G
template<int K>
struct Aho2{
	struct N{
		int nx[K],fail,len;
		N(int l):fail(-1),len(l){one(nx);}
	};
	vc<N> x;
	vi idx,pos;
	void dfs(int cur,const vi&s,int i){
		if(i<(int)s.size()){
			if(x[cur].nx[s[i]]==-1){
				x[cur].nx[s[i]]=si(x);
				x.eb(x[cur].len+1);
			}
			dfs(x[cur].nx[s[i]],s,i+1);
		}else pos.pb(cur);
	}
	Aho2(const vc<vi>&ss):x(1,0){
		{
			int tot=1;
			for(const auto&s:ss)tot+=si(s);
			x.reserve(tot);
			idx.reserve(tot);
			pos.reserve(tot);
		}
		for(auto&s:ss)dfs(0,s,0);
		idx.pb(0);
		int h=0;
		while(h<(int)idx.size()){
			int i=idx[h++];
			rep(c,K)if(x[i].nx[c]!=-1){
				int j=x[i].nx[c],f=x[i].fail;
				if(f!=-1)x[j].fail=x[f].nx[c];
				else x[j].fail=0;
				idx.pb(j);
			}else{
				int f=x[i].fail;
				if(f!=-1)x[i].nx[c]=x[f].nx[c];
				else x[i].nx[c]=0;
			}
		}
	}
	N& operator[](int i){return x[i];}
	size_t size()const{return si(x);}
	void debug_show(){
		#ifdef LOCAL
		vvc<int> ls(si(x));
		for(auto i:idx){
			rep(k,K){
				int j=x[i].nx[k];
				if(x[i].len<x[j].len){
					ls[j]=ls[i];
					ls[j].pb(k);
				}
			}
		}
		cerr<<"Aho of Size "<<si(x)<<endl;
		rep(i,si(x)){
			cerr<<i<<" "<<x[i].fail<<" "<<ls[i]<<endl;
		}
		#endif
	}
};
