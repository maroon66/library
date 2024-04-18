template<class S>
int lcs(const S&a,const S&b){
	int n=si(a);
	static vi dp;
	dp.clear();dp.resize(n+1);
	for(auto v:b){
		per(i,n)if(a[i]==v)chmax(dp[i+1],dp[i]+1);
		rep(i,n)chmax(dp[i+1],dp[i]);
	}
	return dp[n];
}
