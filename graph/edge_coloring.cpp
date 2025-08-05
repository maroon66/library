//vizing's theorem
//最大次数+1 で色を塗る
//stress-tested
vi vizing(int n,vc<pi> es){
	vi deg(n);
	for(auto [a,b]:es){
		deg[a]++;
		deg[b]++;
	}
	int C=MAX(deg)+1;
	
	VVC(int,to,n,C,-1);
	auto upd=[&](int i,int j,int c){
		to[i][c]=j;
		to[j][c]=i;
	};
	
	vi vis(n,-1),tar(n);
	vi st;
	for(auto [root,ini]:es){
		for(auto v:st)vis[v]=-1;
		st.clear();
		int v=ini;
		while(v!=-1&&vis[v]==-1){
			vis[v]=si(st);
			st.pb(v);
			int z=-1;
			rep(c,C)if(to[v][c]==-1&&to[root][c]==-1){
				z=c;
				break;
			}
			if(z==-1){
				rep(c,C)if(to[v][c]==-1){
					z=c;
					break;
				}
			}
			assert(z!=-1);
			tar[v]=z;
			v=to[root][z];
		}
		if(v==-1){
			rep(i,si(st)){
				int c=tar[st[i]];
				upd(st[i],root,c);
				if(i+1<si(st))to[st[i+1]][c]=-1;
			}
			continue;
		}
		int pos=vis[v];
		int x=tar[st[pos-1]];
		assert(to[root][x]==v);
		int y=-1;
		rep(c,C)if(to[root][c]==-1)y=c;
		
		{
			int cur=root;
			int p=x,q=y;
			while(cur!=-1){
				int nx=to[cur][p];
				swap(to[cur][p],to[cur][q]);
				if(tar[cur]==p||tar[cur]==q)tar[cur]^=p^q;
				cur=nx;
				swap(p,q);
			}
		}
		
		bool done=false;
		rep(i,si(st)){
			int c=tar[st[i]];
			int tmp=to[root][c];
			upd(st[i],root,c);
			if(tmp==-1){
				done=true;
				break;
			}else{
				to[st[i+1]][c]=-1;
			}
		}
		assert(done);
	}
	
	using P=pair<pi,int>;
	vc<P> buf;
	rep(i,n)rep(c,C)if(to[i][c]!=-1)buf.eb(pi(i,to[i][c]),c);
	soin(buf);
	
	vi res;
	for(auto e:es)res.pb(buf[lwb(buf,P(e,-1))].b);
	return res;
}
