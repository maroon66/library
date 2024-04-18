
int lcs(string s,string t){
	int n=si(s),m=si(t);
	vvc<int> dp(n+1,vi(m+1,0));
	dp[0][0]=0;
	rep(i,n+1)rep(j,m+1){
		if(i<n&&j<m&&s[i]==t[j])chmax(dp[i+1][j+1],dp[i][j]+1);
		if(i<n)chmax(dp[i+1][j],dp[i][j]);
		if(j<m)chmax(dp[i][j+1],dp[i][j]);
	}
	return dp[n][m];
}
