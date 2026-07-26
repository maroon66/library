//KD tree
//各点に重みがついている
//距離2乗*重み　が最小となる別の点を見つけてくる
vc<ld> nearest(vi ms,vc<pt> ps){
	int n=si(ms);
	assert(n>=2);
	vc<ld> res(n,LLONG_MAX);

	auto upd=[&](int i,int j){
		assert(i!=j);
		ld d2=norm(ps[i],ps[j]);
		chmin(res[i],d2*ms[j]);
		chmin(res[j],d2*ms[i]);
	};

	auto eval=[&](int k,int i){return k==0?ps[i].x:ps[i].y;};

	struct KD{
		int k,lc,rc;
		ld xl,xr,yl,yr,cut;
	};
	vc<KD> t(2*n-1);
	vi par(2*n-1,-1);
	int head=n;
	{
		auto dfs=[&](auto self,vi idx)->int{
			if(si(idx)==1)return idx[0];

			ld xl=inf,xr=-inf,yl=inf,yr=-inf;
			for(auto i:idx){
				chmin(xl,ps[i].x);
				chmax(xr,ps[i].x);
				chmin(yl,ps[i].y);
				chmax(yr,ps[i].y);
			}
			int k=xr-xl<yr-yl;

			int ls=si(idx)/2;

			nth_element(idx.bg,idx.bg+ls,idx.ed,[&](int i,int j){
				return eval(k,i)<eval(k,j);
			});

			ld cut=(eval(k,idx[ls-1])+eval(k,idx[ls]))/2;

			int lc=self(self,vi(idx.bg,idx.bg+ls));
			int rc=self(self,vi(idx.bg+ls,idx.ed));

			int v=head++;
			t[v]=KD{k,lc,rc,xl,xr,yl,yr,cut};
			par[lc]=par[rc]=v;
			return v;
		};
		dfs(dfs,vid(n));
	}

	auto near0=[&](ld l,ld r)->ld{
		assert(l<=r);
		if(r<0)return abs(r);
		else if(0<l)return abs(l);
		else return 0;
	};

	rep(tar,n){
		auto dfs=[&](auto self,int v)->void{
			if(v<n)return upd(tar,v);
			{
				ld xmin=near0(t[v].xl-ps[tar].x,t[v].xr-ps[tar].x);
				ld ymin=near0(t[v].yl-ps[tar].y,t[v].yr-ps[tar].y);
				ld vmin=(sq(xmin)+sq(ymin));
				if(vmin>=res[tar])return;
			}
			
			if(eval(t[v].k,tar)<=t[v].cut){
				self(self,t[v].lc);
				self(self,t[v].rc);
			}else{
				self(self,t[v].rc);
				self(self,t[v].lc);
			}
		};

		int cur=tar;
		while(par[cur]!=-1){
			int nx=par[cur];

			if(t[nx].lc!=cur)dfs(dfs,t[nx].lc);
			else dfs(dfs,t[nx].rc);

			cur=nx;
		}
	}

	return res;
}