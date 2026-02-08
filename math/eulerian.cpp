
vvc<mint> eulerian_table(int n){
	vvc<mint> res(n+1);
	res[0]={1};
	rep(i,n){
		res[i+1].resize(i+2);
		rep(j,i+1){
			res[i+1][j]+=res[i][j]*j;
			res[i+1][j+1]+=res[i][j]*(i+1-j);
		}
	}
	return res;
}
