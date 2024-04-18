template<int L>
int chromatic_number(const vi&g){
	int n=si(g);
	assert(n<=L);
	static uint dp[1<<L],buf[1<<L];
	zero(dp);
	mt19937 gen;
	rep(bit,1<<n){
		bool ok=true;
		rep(i,n)if(bit&1<<i)
			if(g[i]&bit)ok=false;
		if(ok)dp[bit]=gen();
	}
	rep(i,n)rep(bit,1<<n)if((bit&1<<i)==0)
		dp[bit^1<<i]+=dp[bit];
	rep(bit,1<<n)buf[bit]=popcount(bit)%2==n%2?1:-1;
	rng(ans,1,n+1){
		uint s=0;
		rep(bit,1<<n)s+=(buf[bit]*=dp[bit]);
		if(s)return ans;
	}
	assert(false);
}
