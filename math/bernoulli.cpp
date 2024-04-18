//Bernoulli (n)
//Xmas2023 E
mint B(int n){
	assert(n%2==0);
	n--;
	mint sum=0,ans=0;
	auto q=getpowers(n+1,n);
	per(k,n+1){
		mint v=parity(k)*(q[k+1]-q[k]);
		sum+=choose(n,k);
		ans+=v*sum;
	}
	//dmp(n);
	//dmp(ans);
	ans*=n+1;
	ans/=mint(2).pow(n+1);
	ans/=mint(2).pow(n+1)-1;
	return ans;
}
