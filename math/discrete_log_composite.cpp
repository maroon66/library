//yosupo discrete log
//minimum n s.t. a^n=b mod m
//or -1 if infeasible
int discrete_log(int a,int b,int m){
	a%=m;
	b%=m;
	
	int base=0,g,x;
	while(1){
		if(b==1%m)return base;
		tie(g,x)=modinv(a,m);
		if(g==1)break;
		if(b%g)return -1;
		m/=g;
		b=b/g*x%m;
		a%=m;
		base++;
	}
	assert(m>1);
	
	const int L=ceil(sqrt(m-1));
	int ans=inf;
	
	map<int,int> v;
	{
		int y=pow_mod<int>(a,L,m);
		int cur=1;
		for(int i=0;i<m-1;i+=L){
			if(!v.count(cur))v[cur]=i;
			cur=cur*y%m;
		}
	}
	int cur=1;
	rep(i,L){
		int y=b*cur%m;
		if(v.count(y))
			chmin(ans,v[y]+i);
		cur=cur*x%m;
	}
	
	if(ans==inf)
		ans=-1;
	else{
		assert(inc(0,ans,m-2));
		assert(pow_mod<int>(a,ans,m)==b);
		ans+=base;
	}
	
	return ans;
}
