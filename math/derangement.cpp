vc<mint> derangement(int n){
	vc<mint> dp(n+1);
	dp[0]=1;
	dp[1]=0;
	rng(i,2,n+1)dp[i]=(i-1)*(dp[i-1]+dp[i-2]);
	return dp;
}
