//GCJ2019R3D
//input: connected graph
//0 -> unbounded area
//         dual
//           ^
//primal(l)--|->(r)
//           |
struct planardual{
	const int n;
	struct E{
		int to,rev;
		cm dir;
		bool operator<(const E&e)const{
			return argcmp(dir,e.dir)<0;
		}
	};
	struct E2{int to,l,r;};
	vvc<E> org;
	vvc<int> idx;
	int s;
	vvc<E2> g;
	planardual(const vc<cm>&ps,const vvc<int>&gg):
		n(ps.size()),org(n),idx(n),s(0){
		if(n==1){
			s=1;
			g.resize(1);
			return;
		}
		rep(i,n){
			for(auto to:gg[i])
				org[i].pb(E{to,-1,ps[to]-ps[i]});
			sort(all(org[i]));
			idx[i].resize(org[i].size(),-1);
		}
		rep(i,n)for(auto&e:org[i])
			e.rev=lower_bound(all(org[e.to]),E{-1,-1,-e.dir})-org[e.to].bg;
		vi id(n);
		iota(all(id),0);
		sort(all(id),[&](int a,int b){return cmless(ps[a],ps[b]);});
		for(auto i:id){
			rep(j,org[i].size())if(idx[i][j]==-1){
				int v=i,e=j;
				do{
					idx[v][e]=s;
					int to=org[v][e].to;
					e=org[v][e].rev+1;
					if(e==int(org[to].size()))e=0;
					v=to;
				}while(pi(v,e)!=pi(i,j));
				s++;
			}
		}
		g.resize(s);
		rep(i,n)rep(j,org[i].size()){
			int a=idx[i][j];
			int b=idx[org[i][j].to][org[i][j].rev];
			g[a].pb(E2{b,i,org[i][j].to});
		}
	}
};
