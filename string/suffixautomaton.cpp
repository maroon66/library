//suffix automaton
//extend(a,c): ノード a から c の遷移を生やし，遷移先のノードを返す
//文字列集合に対するオートマトンも構築できる
//一般の trie が与えられたら計算量が爆発炎上する
//yosupo number of substrings
//USACO 2017 Dec Platinum 1
template<class C>
struct suffixautomaton{
	struct N{
		//len:ノードに対応する文字列の最長の長さ
		//suf:suffix link
		//org:clone元のノード
		int len,suf,org;
		map<C,int> to;
	};
	vc<N> x;
	suffixautomaton():x{N{0,-1,-1,{}}}{}
	int add(int a,C c){
		assert(x[a].to.count(c));
		int b=x[a].to[c];
		if(x[a].len+1==x[b].len)
			return b;
		else{
			int w=x.size();
			x.pb(x[b]);
			x[w].len=x[a].len+1;
			x[w].org=b;
			while(a!=-1&&x[a].to[c]==b){
				x[a].to[c]=w;
				a=x[a].suf;
			}
			return x[b].suf=w;
		}
	}
	int extend(int a,C c){
		if(x[a].to.count(c))return add(a,c);
		int cur=x.size();
		x.pb(N{x[a].len+1,0,-1,{}});
		while(a!=-1&&!x[a].to.count(c)){
			x[a].to[c]=cur;
			a=x[a].suf;
		}
		if(a!=-1){
			int z=add(a,c);
			x[cur].suf=z;
		}
		return cur;
	}
	vi toposort(){
		int n=x.size();
		vi c(n);
		rep(i,n)for(auto kv:x[i].to)c[kv.b]++;
		vi res(n);
		int b=0,e=0;
		rep(i,n)if(c[i]==0)res[e++]=i;
		while(e<n){
			int i=res[b++];
			for(auto kv:x[i].to)if(--c[kv.b]==0)res[e++]=kv.b;
		}
		return res;
	}
	//Codechef 2021 August Lunchtime Longest Spanning Substrings
	//suf によってできる木の葉から順に削って行ったときの順序を返す
	//DP などに使えると言われている
	vi treeindex(){
		int n=x.size();
		vi c(n);
		rng(i,1,n)c[x[i].suf]++;
		vi res;
		rep(i,n)if(c[i]==0)res.pb(i);
		rep(i,n-1){
			int v=res[i];
			if(--c[x[v].suf]==0)res.pb(x[v].suf);
		}
		return res;
	}
	int size()const{return si(x);}
	N& operator[](int i){
		return x[i];
	}
	void show(){
		int n=si(x);
		vc<string> buf(n);
		rep(i,n){
			for(auto [c,to]:x[i].to){
				string tmp=buf[i]+c;
				if(si(buf[to])<si(tmp))
					buf[to]=tmp;
			}
			cerr<<i<<" "<<buf[i]<<endl;
		}
	}
};

//CF530F
//Ptz 2022S 7-E
struct ste{
	int to,l,r;
	operator int()const{return to;}
};
ostream&operator<<(ostream&os,const ste&a){
	return os<<"ste{"<<a.to<<","<<a.l<<" "<<a.r<<"}";
}
//0 が root
//idx[i]: s[i:n) に対応するノードの index
//辺に対応する [l,r) は多分 l 最小だが未検証
template<class t>
tuple<vvc<ste>,vi,vi> stree(t a){
	int n=si(a);
	vi idx(n+1);
	suffixautomaton<int> sa;
	per(i,n)idx[i]=sa.extend(idx[i+1],a[i]);
	int s=si(sa.x);
	vi l(s,inf);
	vi ord=sa.treeindex();
	rep(i,n+1)l[idx[i]]=i;
	vvc<ste> g(s);
	for(auto i:ord)if(i>0){
		int j=sa[i].suf;
		chmin(l[j],l[i]);
		g[j].pb({i,l[i]+sa[j].len,l[i]+sa[i].len});
	}
	//ノードの子を辞書順に並べる，skip 可
	for(auto&es:g)sort(all(es),[&](ste x,ste y){
		return a[x.l]<a[y.l];
	});
	vi len(s);
	rep(i,s)len[i]=sa[i].len;
	return mt(move(g),move(idx),move(len));
}

//複数文字列の suffix array
//ABC280H
template<class C>
struct stree{
	int n,s;
	vvc<C> ss;
	vvc<int> idx;//idx[i][j] ss[i][j:] が対応するノードの番号
	suffixautomaton<C> sa;
	vvc<int> t;
	vc<pi> term;//term[i] ノード i 以下のノードに対応する suffix の中で (i,pos) 最小
	vi ord;
	int par(int i){return sa[i].suf;}
	int dep(int i){return sa[i].len;}
	using T=tuple<int,int,int>;
	T edge(int v){//par(v)->v の辺の情報 s[i][l:r]
		auto [i,j]=term[v];
		return mt(i,j+dep(par(v)),j+dep(v));
	}
	C ec(int v){
		auto [i,l,r]=edge(v);
		return ss[i][l];
	}
	template<class S>stree(const vc<S>&rw):
		n(si(rw)),ss(n),idx(n){
		rep(i,n){
			ss[i]=vc<C>(all(rw[i]));
			int m=si(ss[i]);
			idx[i].resize(m+1);
			per(j,m)idx[i][j]=sa.extend(idx[i][j+1],ss[i][j]);
		}
		s=si(sa);
		t.resize(s);
		rng(i,1,s)t[par(i)].pb(i);
		term.resize(s,pi(inf,inf));
		ord=sa.treeindex();
		rep(i,n)rep(j,si(ss[i]))
			chmin(term[idx[i][j]],pi(i,j));
		for(auto i:ord)if(i)
			chmin(term[par(i)],term[i]);
		rep(i,s)sort(all(t[i]),[&](int j,int k){return ec(j)<ec(k);});
		
		init();
	}
	//ABC280H
	/*vi cnt,sum,vs;
	void dfs(int v){
		vs.pb(v);
		for(auto to:t[v]){
			auto [i,l,r]=edge(to);
			sum.pb((r-l)*cnt[to]);
			dfs(to);
		}
	}
	void init(){
		cnt.resize(s);
		rep(i,n)rep(j,si(ss[i]))cnt[idx[i][j]]++;
		for(auto i:ord)if(i)
			cnt[par(i)]+=cnt[i];
		dfs(0);
		sum=presum(sum);
	}
	T query(int i){
		assert(i<sum.back());
		int pos=lwb(sum,i+1);
		assert(pos>0);
		int v=vs[pos];
		i-=sum[pos-1];
		int len=i/cnt[v];
		//int q=i%cnt[v];
		//use q-th word in the subtree of v
		//in this case we can ignore it
		auto [x,y]=term[v];
		return mt(x,y,y+dep(par(v))+len+1);
	}*/
	//(i,l,r) でタイブレークする
	//stress-tested 
	vvc<pi> ijs;
	vc<pi> ls;
	vi cnt,in,out,sum,vs;
	unique_ptr<Wavelet<pi>> wv;
	void dfs(int v){
		in[v]=si(ls);
		for(auto w:ijs[v])ls.pb(w);
		vs.pb(v);
		for(auto to:t[v]){
			auto [i,l,r]=edge(to);
			sum.pb((r-l)*cnt[to]);
			dfs(to);
		}
		out[v]=si(ls);
	}
	void init(){
		ijs.resize(s);
		rep(i,n)rep(j,si(ss[i]))ijs[idx[i][j]].eb(i,j);
		cnt.resize(s);
		rep(i,n)rep(j,si(ss[i]))cnt[idx[i][j]]++;
		for(auto i:ord)if(i)
			cnt[par(i)]+=cnt[i];
		in.resize(s);
		out.resize(s);
		dfs(0);
		sum=presum(sum);
		wv=make_unique<Wavelet<pi>>(ls);
	}
	T query(int i){
		assert(i<sum.back());
		int pos=lwb(sum,i+1);
		assert(pos>0);
		int v=vs[pos];
		i-=sum[pos-1];
		int len=i/cnt[v],q=i%cnt[v];
		auto [x,y]=wv->kthval(in[v],out[v],q);
		return mt(x,y,y+dep(par(v))+len+1);
	}
};
