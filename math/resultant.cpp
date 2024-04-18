//let f(x) = sum a[i]*x^i, g(x) = sum b[i]*x^i
//n=deg(f),m=deg(g)
//let lambda_i be solutions of f
//let mu_j be solutions of g
//returns b[m]^n prod f(mu_j)
// = a[n]^m b[m]^n prod (mu_j - lambda_i)
// = (-1)^nm a[n]^m b[m]^n prod (lambda_i - mu_j)
// = (-1)^nm a[n]^m prod g(lambda_i)

//Petrozavodsk 2019w Day7 G
//Petrozavodsk 2020w Day9 K
mint resultant(vc<mint>a,vc<mint> b){
	assert(si(a));
	assert(si(b));
	mint ans=1;
	auto swapab=[&](){
		swap(a,b);
		if((si(a)-1)*(si(b)-1)%2)ans*=-1;
	};
	auto reduce0=[&](){
		while(si(a)>1&&a.back()==0){
			ans*=b.back();
			a.pop_back();
		}
	};
	reduce0();
	swapab();
	reduce0();
	while(1){
		int n=si(a)-1,m=si(b)-1;
		assert(n>=0);
		assert(m>=0);
		if(n<m){
			swapab();
			swap(n,m);
		}
		if(m==0){
			rep(_,n)ans*=b[0];
			return ans;
		}
		mint t=a[n]/b[m];
		rep(i,m+1)a[n-m+i]-=t*b[i];
		reduce0();
	}
	assert(false);
}
