//隣接するものを取らないという条件の元で，総和を*最小化*するように取る
vi candy(vi a){
	int n=si(a);
	int h=(n+1)/2;
	
	a.insert(a.bg,inf);
	a.pb(inf);
	n=si(a);
	
	priority_queue<pi,vc<pi>,greater<pi>> pq;
	rep(i,n)pq.emplace(a[i],i);
	
	int cur=0;
	vi res(h+1,inf);
	vi r(n);iota(all(r),0);
	unionfind uf(n);
	rng(c,1,h+1){
		while(1){
			int w,i;tie(w,i)=pq.top();
			pq.pop();
			if(uf.find(i)==i&&a[i]==w){
				cur+=w;
				int j=r[i];
				int l=uf.find(i-1);
				uf.unite(l,i);
				uf.unite(l,j+1);
				a[l]+=a[j+1];
				a[l]-=a[i];
				r[l]=r[j+1];
				pq.emplace(a[l],l);
				break;
			}
		}
		res[c]=cur;
	}
	
	//勘で足した，何かを破壊したらごめん
	res[0]=0;
	
	return res;
}
