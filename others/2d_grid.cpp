//a[i][j]=true iff a[i][j] is passable
int components_2d(vvc<bool> a){
	int n=si(a),m=si(a[0]);
	vvc<bool> vis(n,vc<bool>(m));
	const int dyx[]{-1,0,1,0,-1};
	int ans=0;
	vi q;
	rep(r,n)rep(c,m)if(a[r][c]&&!vis[r][c]){
		ans++;
		q.clear();
		auto reach=[&](int i,int j){
			if(vis[i][j])return;
			vis[i][j]=1;
			q.eb(i*m+j);
		};
		reach(r,c);
		for(int idx=0;idx<si(q);idx++){
			int i=q[idx]/m,j=q[idx]%m;
			rep(d,4){
				int y=i+dyx[d],x=j+dyx[d+1];
				if(inc(0,y,n-1)&&inc(0,x,m-1)&&a[y][x]){
					reach(y,x);
				}
			}
		}
	}
	return ans;
}


vi q;
rep(r,h)rep(c,w)if(!ng[r][c]){
	one(dist);
	const int dyx[]{-1,0,1,0,-1};
	q.clear();
	auto reach=[&](int i,int j,int d){
		if(dist[i][j]!=-1)return;
		dist[i][j]=d;
		q.eb(i*w+j);
	};
	reach(r,c,0);
	for(int idx=0;idx<si(q);idx++){
		int i=q[idx]/w,j=q[idx]%w;
		rep(d,4){
			int y=i+dyx[d],x=j+dyx[d+1];
			if(inc(0,y,h-1)&&inc(0,x,w-1)&&!ng[y][x]){
				reach(y,x,dist[i][j]+1);
			}
		}
	}
	rep(i,r+1)rep(j,w){
		buf[getid(r*w+c,i*w+j)]=dist[i][j];
		if(i==r&&j==c)break;
	}
}

vvc<int> dist_list(int n,int m,vc<pi> sts){
	vvc<int> vis(n,vc<int>(m,-1));
	const int dyx[]{-1,0,1,0,-1};
	vi q;
	auto reach=[&](int i,int j,int d){
		if(vis[i][j]!=-1)return;
		vis[i][j]=d;
		q.eb(i*m+j);
	};
	for(auto [r,c]:sts)reach(r,c,0);
	for(int idx=0;idx<si(q);idx++){
		int i=q[idx]/m,j=q[idx]%m;
		rep(d,4){
			int y=i+dyx[d],x=j+dyx[d+1];
			if(inc(0,y,n-1)&&inc(0,x,m-1)){
				reach(y,x,vis[i][j]+1);
			}
		}
	}
	return vis;
}

//FCH2023-2-A
const int dyx[]{-1,0,1,0,-1};
//a[i][j]=true iff a[i][j] is passable
vvc<pi> components_2d(vvc<bool> a){
	int n=si(a),m=si(a[0]);
	vvc<bool> vis(n,vc<bool>(m));
	int ans=0;
	vi q;
	vvc<pi> res;
	rep(r,n)rep(c,m)if(a[r][c]&&!vis[r][c]){
		ans++;
		q.clear();
		vc<pi> ijs;
		auto reach=[&](int i,int j){
			if(vis[i][j])return;
			vis[i][j]=1;
			q.eb(i*m+j);
			ijs.eb(i,j);
		};
		reach(r,c);
		for(int idx=0;idx<si(q);idx++){
			int i=q[idx]/m,j=q[idx]%m;
			rep(d,4){
				int y=i+dyx[d],x=j+dyx[d+1];
				if(inc(0,y,n-1)&&inc(0,x,m-1)&&a[y][x]){
					reach(y,x);
				}
			}
		}
		res.pb(ijs);
	}
	return res;
}

//ARC177 C
vvc<int> dijkstra_2d(const vvc<int>&cost,int si,int sj){
	int n=si(cost),m=si(cost[0]);
	VVC(int,dist,n,m,inf);
	const int dyx[]{-1,0,1,0,-1};
	pqmin<T> pq;
	auto reach=[&](int i,int j,int d){
		d+=cost[i][j];
		if(chmin(dist[i][j],d))
			pq.emplace(d,i,j);
	};
	reach(si,sj,0);
	while(si(pq)){
		auto [d,i,j]=pq.top();pq.pop();
		if(dist[i][j]!=d)continue;
		rep(dir,4){
			int y=i+dyx[dir],x=j+dyx[dir+1];
			if(inc(0,y,n-1)&&inc(0,x,m-1)){
				reach(y,x,d);
			}
		}
	}
	return dist;
}
