//UCUP4-22-K
vi continued_fraction(int n){
	int s=sqrtl(n);
	vi res{s};
	int a=s,b=n-sq(s);
	if(b==0)return res;
	int A=a,B=b;
	do{
		//(sqrt(n)+a)/b を作る
		//b | n-a^2
		int k=(s+a)/b;
		//↓hos が書いてた
		assert(inc(0,a,2*s));
		assert(inc(1,b,2*s));
		assert(inc(1,k,2*s));
		res.pb(k);
		a=b*k-a;
		b=(n-sq(a))/b;
	}while(!(a==A&&b==B));
	return res;
}

vi approx(int n,int budget){
	vi z=continued_fraction(n);
	if(si(z)==1)return z;
	assert(budget>=z[0]);
	vi res{z[0]};
	budget-=z[0];
	int i=1;
	while(1){
		int u=z[i];
		
		if(budget>=u){
			res.pb(u);
			budget-=u;
		}else{
			if(budget>(u+1)/2)res.pb(budget);
			break;
		}
		
		i++;
		if(i==si(z))i=1;
	}
	
	return res;
}
