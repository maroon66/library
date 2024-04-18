//a(b(x))
//veryfy: https://judge.yosupo.jp/problem/composition_of_formal_power_series
//サイズ si(b) を返す
vc<mint> composite(vc<mint> a,vc<mint> b){
	swap(a,b);
	int n=si(a),s=minp2(n*2-1);
	auto cut=[&](vc<mint>&x){
		inplace_fmt(x,true);
		rng(i,n,s)x[i]=0;
		inplace_fmt(x,false);
	};
	int m=si(b);
	assert(m>0);
	const int B=sqrtl(m);
	a.resize(s);inplace_fmt(a,false);
	vvc<mint> buf(B+1);
	buf[0].resize(s,1);
	rng(i,1,B+1){
		buf[i].resize(s);
		rep(j,s)buf[i][j]=buf[i-1][j]*a[j];
		cut(buf[i]);
	}
	vc<mint> ans(s);
	vc<mint> cur(s,1);
	for(int i=0;i<m;i+=B){
		if(i>0){
			rep(j,s)cur[j]*=buf[B][j];
			cut(cur);
		}
		rep(k,min(B,m-i)){
			rep(j,s)ans[j]+=b[i+k]*cur[j]*buf[k][j];
		}
	}
	inplace_fmt(ans,true);
	ans.resize(n);
	return ans;
}


//si(a)=n として
//[x^n-1] a^0,...,a^m を返す
vc<mint> pow_list(vc<mint> a,int m){
	int n=si(a),s=minp2(n*2-1);
	m++;assert(m>0);
	const int B=sqrtl(m);
	a.resize(s);inplace_fmt(a,false);
	vvc<mint> buf(B+1);
	buf[0].resize(s);buf[0][0]=1;
	rng(i,1,B+1){
		buf[i]=buf[i-1];
		inplace_fmt(buf[i],false);
		rep(j,s)buf[i][j]*=a[j];
		inplace_fmt(buf[i],true);
		rng(j,n,s)buf[i][j]=0;
	}
	auto z=buf[B];
	inplace_fmt(z,false);
	vc<mint> ans(m);
	vc<mint> cur(s);cur[0]=1;
	for(int i=0;i<m;i+=B){
		if(i>0){
			inplace_fmt(cur,false);
			rep(j,s)cur[j]*=z[j];
			inplace_fmt(cur,true);
			rng(j,n,s)cur[j]=0;
		}
		rep(k,min(B,m-i)){
			rep(j,n)ans[i+k]+=cur[j]*buf[k][n-1-j];
		}
	}
	return ans;
}
