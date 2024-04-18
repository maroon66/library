//https://judge.yosupo.jp/problem/hafnian_of_matrix
//複数回呼んで壊れたらごめん
//2N=38 で 1676ms
namespace hafnian{
	const int L=20;
	using A=array<mint,L+1>;
	A x[L+1][L*2][L*2],y[L+1];
	int n;
	mint ans;
	void mult(const A&a,const A&b,A&c){
		rep(i,n)rep(j,n-i)c[i+j+1]+=a[i]*b[j];
	}
	void cp(const A&a,A&b){
		rep(i,n+1)b[i]=a[i];
	}
	void dfs(int cur,int lv,mint w){
		if(lv==n){
			ans+=y[cur][n]*w;
			return;
		}
		dfs(cur,lv+1,-w);
		int s=(n-lv-1)*2;
		rep(i,s)rng(j,i+1,s){
			cp(x[cur][i][j],x[cur+1][i][j]);
			mult(x[cur][i][s],x[cur][j][s+1],x[cur+1][i][j]);
			mult(x[cur][i][s+1],x[cur][j][s],x[cur+1][i][j]);
		}
		cp(y[cur],y[cur+1]);
		mult(y[cur],x[cur][s][s+1],y[cur+1]);
		dfs(cur+1,lv+1,w);
	}
	mint calc(const vvc<mint>&a){
		zero(x);
		zero(y);
		assert(si(a)%2==0);
		n=si(a)/2;
		rep(i,n*2)rng(j,i+1,n*2)x[0][i][j][0]=a[i][j];
		y[0][0]=1;
		ans=0;
		dfs(0,0,1);
		return ans;
	}
}
