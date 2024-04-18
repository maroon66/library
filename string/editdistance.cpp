int editdistance(string s,string t){
	int n=si(s),m=si(t);
	vvc<int> dp(n+1,vi(m+1,inf));
	dp[0][0]=0;
	rep(i,n+1)rep(j,m+1){
		if(i<n&&j<m)chmin(dp[i+1][j+1],dp[i][j]+(s[i]!=t[j]));
		if(i<n)chmin(dp[i+1][j],dp[i][j]+1);
		if(j<m)chmin(dp[i][j+1],dp[i][j]+1);
	}
	return dp[n][m];
}
