//cost(i,j) が monge
//O(N) で DP する
//stress-tested
//IOI 2016 Alien
//平均 7N 回程度 cost(i,j) が走っていそう
template<class F>
auto&larsch(int n,F cost){
	assert(2<=n);
	using t=typename invoke_result<F,int,int>::type;
	const int L=topbit(n);
	static vc<t> res;res.resize(n);
	static vc<pair<t,int>> val[30];
	static vc<pi> pos[30];
	static int s[30];
	rep(i,L){
		val[i].resize(n>>i);
		pos[i].resize(n>>i);
		s[i]=0;
	}
	s[0]=n;
	rep(i,n)pos[0][i].a=i;
	auto eval=[&](int lv,int i,int j){
		return res[pos[lv][i].a]+cost(pos[lv][i].a,((j+1)<<lv)-1);
	};
	auto setval=[&](int lv,int i,int j){
		val[lv][j]=mp(eval(lv,i,j),i);
	};
	res[0]=t{};
	rng(i,1,n){
		const int l=i,r=i+1;
		int lv=botbit(l);
		if((l>>lv)<(n>>lv)){
			setval(lv,(l>>lv)-1,l>>lv);
			if(l>>(lv+1))
				chmin(val[lv][l>>lv],
				mp(val[lv+1][l>>(lv+1)].a,pos[lv+1][val[lv+1][l>>(lv+1)].b].b));
		}
		while(lv>0){
			lv--;
			if((l>>lv)<(n>>lv)){
				int lw=val[lv][(l>>lv)-1].b;
				int up=(l>>(lv+1))<(n>>(lv+1))?
					pos[lv+1][val[lv+1][l>>(lv+1)].b].b:(l>>lv)-1;
				assert(lw<=up);
				setval(lv,lw,l>>lv);
				rng(j,lw+1,up+1)
					chmin(val[lv][l>>lv],mp(eval(lv,j,l>>lv),j));
			}
		}
		res[i]=val[0][i].a;
		while((r&1<<lv)==0&&lv<L-1){
			lv++;
			rng(b,(r>>(lv-1))-2,r>>(lv-1)){
				int a=pos[lv-1][b].a;
				while(s[lv]>=(r>>lv)){
					if(eval(lv,s[lv]-1,s[lv])>res[a]+cost(a,((s[lv]+1)<<lv)-1))s[lv]--;
					else break;
				}
				if(s[lv]+1<(n>>lv))pos[lv][s[lv]++]=pi(a,b);
			}
		}
	}
	return res;
}

//0->n-1 のジャンプ
//monge コストでちょうど k step でやれ，みたいな最小化問題 
//ans(step) が単調減少だとする(つまり小刻みであればあるほど嬉しい)
//max(ans(step))-max(ans(step)) の上界を dif で与えている
//stress-tested (define ll なし，復元もちゃんと確かめた)
//IOI 2016 Alien
//N=10^5,dif=10^18 で 0.2 sec 強かかる（costはO(1))
template<class F>
ll kstepmin(int n,int k,ll dif,F cost,vi*sol=nullptr){
	assert(inc(1,k,n-1));
	ll lw=-1,up=dif+1,ans=infLL;
	vc<pair<ll,int>> buf(n);
	while(up-lw>1){
		ll mid=(lw+up)/2;
		auto&dp=larsch(n,[&](int i,int j){
			return pair<ll,int>(cost(i,j)+mid,1);
		});
		if(dp[n-1].b<=k){
			up=mid;
			ans=dp[n-1].a-k*mid;
			copy(all(dp),buf.bg);
		}else lw=mid;
	}
	assert(ans<infLL);
	if(sol){
		sol->clear();
		int cur=n-1;
		sol->pb(cur);
		while(cur){
			assert(k);
			k--;
			int nx=cur;
			while(nx--){
				if(buf[nx].b<=k&&buf[nx].a+cost(nx,cur)+up==buf[cur].a){
					sol->pb(cur=nx);
					break;
				}
			}
			assert(nx>=0);
		}
		assert(k==0);
		reverse(sol->bg,sol->ed);
	}
	return ans;
}

//cost(i,j) が monge
//O(N log N) で DP する
//stress-tested
//https://noshi91.hatenablog.com/entry/2023/02/18/005856
//コストを求める部分が slide でできる．
template<class F>
auto&larsch(int n,F cost){
	assert(2<=n);
	using t=typename invoke_result<F,int,int>::type;
	static vc<t> res;res.resize(n);
	static vi pos;pos.resize(n);fill(all(pos),-1);
	auto upd=[&](int from,int to){
		t val=res[from]+cost(from,to);
		if(pos[to]==-1||res[to]>val){
			res[to]=val;
			pos[to]=from;
		}
	};
	auto dfs=[&](auto self,int l,int r)->void{
		if(r-l==1){
			if(l==0)res[0]=t{};
		}else{
			int m=(l+r)/2;
			//同じ lv でここだけみたら cost(i,j) の i,j が単調
			if(l)rng(i,max<int>(pos[l-1],0),pos[r-1]+1)upd(i,m-1);
			self(self,l,m);
			//同じ lv でここだけみたら cost(i,j) の i,j が単調
			rng(i,l,m)upd(i,r-1);
			self(self,m,r);
		}
	};
	dfs(dfs,0,n);
	return res;
}
