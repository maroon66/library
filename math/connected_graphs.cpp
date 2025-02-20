int c2(int n){
	return n*(n-1)/2;
}

//i<=n,j<=m に対し，i 頂点 j 辺の連結グラフの個数を返す
//辺に番号がついており多重辺もありであることに注意
vvc<mint> connected_graphs(int n,int m){
	vvc<mint> dp(n+1);
	rng(i,1,n+1)dp[i].resize(c2(i)+1);
	rng(i,1,n+1)dp[i][c2(i)]+=1;
	rng(i,1,n+1)rep(j,c2(i)+1)rng(k,1,n-i+1)
		dp[i+k][j+c2(k)]-=dp[i][j]*binom(i-1,k);
	
	VVC(mint,res,n+1,m+1);
	rng(i,1,n+1)rep(e,c2(i)+1){
		mint w=1;
		rep(j,m+1){
			res[i][j]+=w*dp[i][e];
			w*=e;
		}
	}
	
	return res;
}

//f,g のサイズ同じ
void chmult2d_easy(vvc<mint>&f,const vvc<mint>&g){
	int n=si(f),m=si(f[0]);
	per(i,n)per(j,m){
		mint v=f[i][j];
		f[i][j]=0;
		rep(x,n-i)rep(y,m-j){
			f[i+x][j+y]+=v*g[x][y];
		}
	}
}

//i<=n,j<=m に対し，i 頂点 j 辺の二重辺連結グラフの個数を返す
//辺に番号がついており多重辺もありであることに注意
vvc<mint> biconnected_graphs(int n,int m){
	auto f=connected_graphs(n,m);
	rng(i,1,n+1)rep(j,m+1)f[i][j]*=i*finv[i]*finv[j];
	VVC(mint,cur,n+1,m+1);
	VVC(mint,res,n+1,m+1);
	cur[0][0]=1;
	rng(k,1,n+1){
		chmult2d_easy(cur,f);
		rng(i,1,n+1){
			mint w=fact[i]*parity(k-1)*finv[k]*mint(i).pow(k-2);
			rep(j,m-(k-1)+1){
				res[i][j+k-1]+=cur[i][j]*w;
			}
		}
	}
	rep(i,n+1)rep(j,m+1)res[i][j]*=fact[j];
	return res;
}
