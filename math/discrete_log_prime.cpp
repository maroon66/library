//require a^s=1
//minimum n s.t. a^n=b, 0<=n<=s-1
//or -1 if infeasible
template<class t>
int discrete_log(t a,t b,int s){
	dmp2(a,b,s);
	
	assert(a.pow(s)==t(1));
	
	const int L=ceil(sqrt(s));
	int ans=inf;
	
	map<t,int> v;
	{
		t x=a.pow(L);
		t cur(1);
		for(int i=0;i<s;i+=L){
			if(!v.count(cur))v[cur]=i;
			cur*=x;
		}
	}
	t x=a.inv();
	dmp(x);
	t cur(1);
	rep(i,L){
		t y=b*cur;
		if(v.count(y))
			chmin(ans,v[y]+i);
		cur*=x;
	}
	
	if(ans>=s)
		ans=-1;
	else{
		dmp(L);
		dmp(ans);
		assert(inc(0,ans,s-1));
		assert(a.pow(ans)==b);
	}
	
	return ans;
}

//require a^s=1
//returns minimum p s.t. a^p=1
template<class t>
int mult_period(t a,int s){
	vi f=factors(s);
	int p=s;
	for(auto v:f){
		while(p%v==0){
			if(a.pow(p/v)==t(1))
				p/=v;
			else
				break;
		}
	}
	return p;
}

//require b^s=1
//pair of
//minimum n s.t. a*b^n=c,0<=n<=s-1
//and period
//or (-1,-1) if infeasible
template<class t>
pi discrete_log_helper(t a,t b,t c,int s){
	dmp2(a,b,c,s);
	if(!a||!b){
		if(!c)return pi(0,1);
		else return pi(-1,-1);
	}
	assert(b.pow(s)==t(1));
	int w=discrete_log(b,c*a.inv(),s);
	if(w==-1)return pi(-1,-1);
	return pi(w,mult_period(b,s));
}
