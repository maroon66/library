vc<pi> factorize(int n){
	vc<pi> res;
	for(int p=2;p*p<=n;p++){
		if(n%p==0){
			int c=0;
			while(n%p==0){
				n/=p;
				c++;
			}
			res.eb(p,c);
		}
	}
	if(n>1)res.eb(n,1);
	return res;
}

vi divisors_easy(int n){
	vi ls;
	for(int i=1;i*i<=n;i++){
		if(n%i==0){
			ls.pb(i);
			if(i<n/i)ls.pb(n/i);
		}
	}
	sort(all(ls));
	return ls;
}

bool isprime(ll p){
	if(p==1)return false;
	for(ll n=2;n*n<=p;n++){
		if(p%n==0)return false;
	}
	return true;
}
