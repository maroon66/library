//https://judge.yosupo.jp/problem/prefix_substring_lcs
//C(k,l,r)=LCS(a[0,k),b[l,r))

//k,r>0 を固定する．
//C(k,l,r)-C(k,l,r-1)=0 l<h[k][r]
//C(k,l,r)-C(k,l,r-1)=1 h[k][r]<=l
//を満たす h[k][r] がとれる．(h[k][r]=r かもしれない)

//k>0,r を固定する．
//C(k,l,r)-C(k-1,l,r)=1 l<v[k][r]
//C(k,l,r)-C(k-1,l,r)=0 v[k][r]<=l
//をみたす v[k][r] がとれる．(v[k][r]=r かもしれない)

//(h,v) を DP で求め，返す．
//O(|A||B|)
template<class S>
pair<vvc<int>,vvc<int>> prefix_substring_lcs(S a,S b){
	int n=si(a),m=si(b);
	vvc<int> h(n+1,vi(m+1,-1));
	vvc<int> v(n+1,vi(m+1,-1));
	
	rng(r,1,m+1)h[0][r]=r;
	rng(k,1,n+1)v[k][0]=0;
	
	rng(k,1,n+1)rng(r,1,m+1){
		if(a[k-1]==b[r-1]){
			v[k][r]=h[k-1][r];
			h[k][r]=v[k][r-1];
		}else{
			tie(v[k][r],h[k][r])=minmax(v[k][r-1],h[k-1][r]);
		}
	}
	
	return mp(h,v);
}

//res[i][j]=LCS(a,b[i,j))
//UCUP2-5-L
template<class S>
vvc<int> lcs_list(const S&a,const S&b){
	int n=si(a),m=si(b);
	auto [h,v]=prefix_substring_lcs(a,b);
	vi val(m+1);
	vvc<int> res(m+1,vi(m+1,-inf));
	rep(j,m+1){
		if(j>0){
			val[h[n][j]]++;
			val[j]--;
		}
		int x=0;
		rep(i,j+1){
			x+=val[i];
			res[i][j]=x;
		}
	}
	return res;
}
