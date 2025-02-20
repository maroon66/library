//内部でグラフをいじるから in,out を使うときは注意
//hei[v] -> heavy edge で潜っていった時，自分含めて何個あるか
//pe[v]: v->par[v] の辺の情報
//-有向木のときは上から下の辺を入れてる
//-無向木のときは下から上の辺を入れてる
//VERIFY: yosupo
//CF530F
//CodeChef Persistent Oak
//AOJ GRL5C
template<class E>
struct HLD{
	vvc<E> g;
	int n,rt,cnt;
	vi sub,in,out,par,head,dep,hei,ni;
	vc<E> pe;
	int dfs1(int v,int p,int d){
		par[v]=p;
		dep[v]=d;
		for(auto itr=g[v].bg;itr!=g[v].ed;itr++)
			if(*itr==p){
				pe[v]=*itr;
				g[v].erase(itr);
				break;
			}
		for(auto&e:g[v]){
			pe[e]=e;
			sub[v]+=dfs1(e,v,d+1);
			if(sub[g[v][0]]<sub[e])
				swap(g[v][0],e);
		}
		return sub[v];
	}
	void dfs2(int v,int h){
		in[v]=cnt++;
		head[v]=h;
		for(int to:g[v])
			dfs2(to,to==g[v][0]?h:to);
		out[v]=cnt;
		if(si(g[v]))hei[v]=hei[g[v][0]]+1;
	}
	HLD(){}
	HLD(const vvc<E>&gg,int rr):g(gg),n(g.size()),rt(rr),cnt(0),
		sub(n,1),in(n),out(n),par(n,-1),head(n),dep(n),hei(n,1),ni(n),
		pe(n){
		dfs1(rt,-1,0);
		dfs2(rt,rt);
		rep(i,n)ni[in[i]]=i;
	}
	int lca(int a,int b){
		while(head[a]!=head[b]){
			if(dep[head[a]]>dep[head[b]])
				swap(a,b);
			b=par[head[b]];
		}
		if(dep[a]>dep[b])
			swap(a,b);
		return a;
	}
	int len(int a,int b){
		return dep[a]+dep[b]-dep[lca(a,b)]*2;
	}
	bool asde(int a,int b){
		return in[a]<=in[b]&&out[b]<=out[a];
	}
	//UCUP 1-22 F
	int adv(int a,int d){
		if(hei[a]<=d)return -1;
		else return ni[in[a]+d];
	}
	//CF692F
	int getpar(int v,int len){
		assert(dep[v]>=len);
		int tar=dep[v]-len;
		while(1){
			int h=head[v];
			if(dep[h]<=tar){
				return ni[in[h]+(tar-dep[h])];
			}
			v=par[h];
		}
		assert(false);
	}
	//1st UCUP 13 G
	int jump(int a,int b,int d){
		int c=lca(a,b);
		if(d<=(dep[a]-dep[c])){
			return getpar(a,d);
		}else{
			d=(dep[a]+dep[b]-dep[c]*2)-d;
			assert(d>=0);
			return getpar(b,d);
		}
	}
	//XX Opencup GP of Korea
	//CF625 F
	//2020 Multi-Uni Contest Day5 G
	//CF415E
	//Universal Cup 2023 Stage 1 G
	vi index;
	//vs を含む virtual tree を返す
	//返すのは virtual tree に使われた頂点と，辺の集合
	//辺の端点は，virtual tree における番号
	//元の木における番号を virtual tree の頂点番号に写すのが，index という変数
	//辺は ch->par の順
	//virtual tree は行き掛け順で番号がついている
	//特に，頂点 0 が根になるようにできている
	//vsは頂点が行きがけ順に並ぶ
	//pair<vi,vc<pi>> tree_compress(vi vs){
	void tree_compress(vi&vs,vc<pi>&es){
		if(si(index)==0)index.resize(n);
		assert(index.size());
		auto comp = [&](int x,int y){
			return in[x] < in[y];
		};
		sort(all(vs),comp);
		assert(is_sorted(all(vs),comp));
		vs.erase(unique(all(vs)),vs.ed);
		int k = vs.size();
		rep(i,k-1){
			vs.pb(lca(vs[i],vs[i+1]));
		}
		sort(all(vs),comp);
		vs.erase(unique(all(vs)),vs.ed);
		k = vs.size();
		rep(i,k) index[vs[i]] = i;
		es.clear();
		rng(i,1,k){
			int p = lca(vs[i-1],vs[i]);
			es.eb(i,index[p]);
		}
		//return mp(vs,es);
	}
	//assume a is desdendant of b
	//ex=true <=> exclude b
	template<class F>
	void subpath_work(int a,int b,bool ex,F f){
		while(1){
			if(head[a]==head[b]){
				f(in[b]+ex,in[a]+1);
				break;
			}else{
				int h=head[a];
				f(in[h],in[a]+1);
				a=par[h];
			}
		}
	}
	//KUPC2021E
	//パスに対する操作順に注意
	//euler-tour 順にしたときの区間に作用していることに注意
	//ex=true exclude lca(a,b) (=apply path edges)
	template<class F>
	void path_work(int a,int b,bool ex,F f){
		int c=lca(a,b);
		subpath_work(a,c,ex,f);
		subpath_work(b,c,true,f);
	}
	//v->false
	//-1->true
	//root-v パス上で f(x)=true となる最も深い頂点を返す
	//CF857G
	template<class F>
	int find_lowest(int v,F f)const{
		while(v>=0){
			int h=head[v];
			if(!f(h)){
				v=par[h];
			}else{
				int l=0,r=dep[v]-dep[h]+1;
				while(r-l>1){
					const int mid=(l+r)/2;
					if(f(ni[in[h]+mid]))l=mid;
					else r=mid;
				}
				return ni[in[h]+l];
			}
		}
		return -1;
	}
	//-1->false
	//v->true
	//root-v パス上で f(x)=true となる最も浅い頂点を返す
	//Yandex Cup 2023 Semifinal F (TLE...)
	template<class F>
	int find_highest(int v,F f)const{
		while(1){
			int h=head[v];
			int p=par[h];
			if(p!=-1&&f(p)){
				v=p;
			}else{
				int l=-1,r=dep[v]-dep[h];
				while(r-l>1){
					const int mid=(l+r)/2;
					if(f(ni[in[h]+mid]))r=mid;
					else l=mid;
				}
				return ni[in[h]+r];
			}
		}
		assert(false);
	}
	//CF969E
	void upddia(int&a,int&b,int&d,int v){
		int x=len(a,v);
		int y=len(b,v);
		if(x<y){
			swap(x,y);
			swap(a,b);
		}
		if(d<x){
			b=v;
			d=x;
		}
	}
};

struct E{
	int to,idx;
	operator int()const{
		return to;
	}
};
