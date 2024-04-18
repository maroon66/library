//UCUP2-3-H
struct complement_bfs{
	vvc<int> g;
	const int n;
	vi d,q,vs;
	complement_bfs(const vvc<int>&gg):g(gg),n(si(g)),d(n),q(n),vs(n){
		rep(i,n)soin(g[i]);
	}
	void calc(int root){
		rep(i,n)d[i]=-1;
		rep(i,n)vs[i]=i;
		int s=0,t=n;
		auto reach=[&](int v,int dist){
			if(d[v]==-1){
				d[v]=dist;
				q[s++]=v;
			}
		};
		reach(root,0);
		rep(head,s){
			int v=q[head];
			for(auto to:g[v])if(d[to]==-1)d[to]=-2;
			int nx=0;
			rep(i,t){
				if(d[vs[i]]==-2)
					vs[nx++]=vs[i];
				else
					reach(vs[i],d[v]+1);
			}
			t=nx;
			for(auto to:g[v])if(d[to]==-2)d[to]=-1;
		}
	}
};
