//sz: size of the matching
//mt[i]: matching mate of the vertex i
//VERIFY: yosupo
//NEERC2018B
//Petrozavodsk 2019w Day1 A
//頂点をランダムシャッフルすることで速くなる(UTPC2022)
//頂点を見る順番をランダムにするよりも入力のグラフをランダムにしたほうがいい（なんで？）
struct maxmatching{
	struct E{int a,b;};
	const vvc<int>&g;
	int n,sz;
	vi p,t,mt;
	vc<E> f;
	int non(int v){
		return t[v]!=sz||p[v]==-1?v:(p[v]=non(p[v]));
	}
	void R(int a,int b){
		int d=mt[a];
		mt[a]=b;
		if(d==-1||mt[d]!=a)return;
		if(f[a].b==-1){
			mt[d]=f[a].a;
			R(f[a].a,d);
		}else{
			R(f[a].a,f[a].b);
			R(f[a].b,f[a].a);
		}
	}
	bool arg(int rt){
		static vi q;q.clear();
		int head=0;
		t[rt]=sz;
		p[rt]=-1;
		f[rt]=E{-1,-1};
		q.pb(rt);
		while(head<si(q)){
			int a=q[head++];
			for(auto b:g[a]){
				if(b==rt)continue;
				if(mt[b]==-1){
					mt[b]=a;
					R(a,b);
					return true;
				}
				if(t[b]==sz){
					int x=non(a),y=non(b);
					if(x==y)continue;
					int z=rt;
					while(x!=rt||y!=rt){
						if(y!=rt)swap(x,y);
						if(f[x].a==a&&f[x].b==b){
							z=x;
							break;
						}
						f[x]=E{a,b};
						x=non(f[mt[x]].a);
					}
					for(auto v:{non(a),non(b)}){
						while(v!=z){
							t[v]=sz;
							p[v]=z;
							q.pb(v);
							v=non(f[mt[v]].a);
						}
					}
					continue;
				}
				if(t[mt[b]]==sz)continue;
				f[b]=E{-1,-1};
				t[mt[b]]=sz;
				p[mt[b]]=b;
				f[mt[b]]=E{a,-1};
				q.pb(mt[b]);
			}
		}
		return false;
	}
	maxmatching(const vvc<int>&gg):g(gg),n(g.size()),sz(0),
		p(n),t(n,-1),mt(n,-1),f(n){
		rep(i,n)if(mt[i]==-1){
			sz+=arg(i);
		}
	}
	//codechef HAMILG
	//res[i]=最大マッチングに必ず含まれるべき頂点なら 1, そうでないなら 0
	vi need(){
		rep(i,n)if(mt[i]==-1)
			arg(i);
		vi res(n);
		rep(i,n)res[i]=(mt[i]!=-1&&t[i]!=sz);
		return res;
	}
};
