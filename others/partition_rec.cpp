template<class F>
void partition_rec(int n,F f){
	auto dfs=[&](auto self,int rem,vi&cur)->void{
		if(rem==0){
			f(cur);
		}else{
			int mx=rem;
			if(si(cur))chmin(mx,cur.back());
			rng(nx,1,mx+1){
				cur.pb(nx);
				self(self,rem-nx,cur);
				cur.pop_back();
			}
		}
	};
	vi tmp;
	dfs(dfs,n,tmp);
}
