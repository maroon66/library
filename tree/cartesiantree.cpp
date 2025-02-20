//min cartesian tree
vi mincartesiantree(const vi&a){
	int n=si(a);
	vi par(n,-1);
	vi s;
	rep(i,n){
		int last=-1;
		while(si(s)&&a[s.back()]>=a[i]){
			last=s.back();
			s.pop_back();
		}
		if(last!=-1)par[last]=i;
		if(si(s))par[i]=s.back();
		s.pb(i);
	}
	return par;
}

//max cartesian tree
vi maxcartesiantree(const vi&a){
	int n=si(a);
	vi par(n,-1);
	vi s;
	rep(i,n){
		int last=-1;
		while(si(s)&&a[s.back()]<=a[i]){
			last=s.back();
			s.pop_back();
		}
		if(last!=-1)par[last]=i;
		if(si(s))par[i]=s.back();
		s.pb(i);
	}
	return par;
}

//おまたせ
	int n;cin>>n;
	vi a=readvi(n);
	using A=array<int,2>;
	vc<A> t(n,A{-1,-1});
	vi par=cartesiantree(a);
	int root=-1;
	rep(i,n)if(par[i]==-1)root=i;
	else t[par[i]][par[i]<i]=i;


//sum of range max
//UCUP2-6-F
int maxsum(const vi&a){
	int n=si(a);
	using A=array<int,2>;
	vc<A> t(n,A{-1,-1});
	vi par=cartesiantree(a);
	int root=-1;
	rep(i,n)if(par[i]==-1)root=i;
	else t[par[i]][par[i]<i]=i;
	
	int res=0;
	auto dfs=[&](auto self,int i,int l,int r)->void{
		if(i==-1)return;
		res+=a[i]*(i-l+1)*(r-i);
		self(self,t[i][0],l,i);
		self(self,t[i][1],i+1,r);
	};
	dfs(dfs,root,0,n);
	return res;
}

//sum of range min
//CF CodeTON3-E
int minsum(const vi&a){
	int n=si(a);
	using A=array<int,2>;
	vc<A> t(n,A{-1,-1});
	vi par=mincartesiantree(a);
	int root=-1;
	rep(i,n)if(par[i]==-1)root=i;
	else t[par[i]][par[i]<i]=i;
	
	int res=0;
	auto dfs=[&](auto self,int i,int l,int r)->void{
		if(i==-1)return;
		res+=a[i]*(i-l+1)*(r-i);
		self(self,t[i][0],l,i);
		self(self,t[i][1],i+1,r);
	};
	dfs(dfs,root,0,n);
	return res;
}

//UCUP 3-4-C
template<class F>
void maximal_rectangle(const vvc<int>&avail,F f){
	int n=si(avail),m=si(avail[0]);
	vi a(n);
	rep(j,m){
		rep(i,n){
			if(avail[i][j]){
				a[i]++;
			}else{
				a[i]=0;
			}
		}
		
		using A=array<int,2>;
		vc<A> t(n,A{-1,-1});
		vi par=mincartesiantree(a);
		int root=-1;
		rep(i,n)if(par[i]==-1)root=i;
		else t[par[i]][par[i]<i]=i;
		
		auto dfs=[&](auto self,int i,int l,int r)->void{
			if(i==-1)return;
			if(a[i]>0)f(l,r,j+1-a[i],j+1);
			self(self,t[i][0],l,i);
			self(self,t[i][1],i+1,r);
		};
		dfs(dfs,root,0,n);
	}
}
