//need linear_sieve
vc<mint> enum_pows(int n,int p){
	assert(p>0);
	vc<mint> ans(n);
	ans[1]=1;
	rng(i,2,n)if(pri[i])
		ans[i]=mint(i).pow(p);
	else
		ans[i]=ans[i/sf[i]]*ans[sf[i]];
	return ans;
}
