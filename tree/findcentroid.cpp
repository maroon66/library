template<class E>
int findc(const vvc<E>&t,const vc<bool>&rem,int root){
	int s;
	{
		auto rec=[&](auto self,int v,int p)->int{
			int ret=1;
			for(auto to:t[v])if(to!=p&&!rem[to])
				ret+=self(self,to,v);
			return ret;
		};
		s=rec(rec,root,-1);
	}
	auto rec=[&](auto self,int v,int p)->int{
		int ret=1,mx=0;
		for(auto to:t[v])if(to!=p&&!rem[to]){
			int f=self(self,to,v);
			if(f<=0)
				return f;
			else{
				ret+=f;
				chmax(mx,f);
			}
		}
		chmax(mx,s-ret);
		if(mx*2<=s)
			return -v;
		else
			return ret;
	};
	return -rec(rec,root,-1);
}

//https://judge.yosupo.jp/problem/frequency_table_of_tree_distance
template<class E>
vi tree_dist_freq(const vvc<E>&t){
	vi res;
	
	auto add=[&](vi&x,int i,int v){
		if(si(x)<=i)x.resize(i+1);
		x[i]+=v;
	};
	
	vi x,y;
	vc<bool> rem(si(t));
	auto rec=[&](auto self,int root)->void{
		root=findc(t,rem,root);
		
		x.clear();
		add(x,0,1);

		auto dfs=[&](auto self2,int v,int p,int d)->void{
			add(x,d,1);
			add(y,d,1);
			for(auto to:t[v])if(!rem[to]&&to!=p)
				self2(self2,to,v,d+1);
		};
		for(auto to:t[root])if(!rem[to]){
			y.clear();
			dfs(dfs,to,root,1);
			auto z=integer_convolution::multiply(y,y,true);
			rep(i,si(z))add(res,i,-z[i]);
		}
		
		auto z=integer_convolution::multiply(x,x,true);
		rep(i,si(z))add(res,i,z[i]);
		
		rem[root]=true;
		for(auto to:t[root])if(!rem[to])
			self(self,to);
	};
	rec(rec,0);
	
	res.resize(si(t));
	res[0]=0;
	rng(i,1,si(t))res[i]/=2;
	return res;
}

//mint 版（なんとなもりで距離分布を求めるコードもあるよ）
//Newton 2022 triwizard F

vi t[nmax];
bool rem[nmax];

int tsize(int v,int p){
	int ret=1;
	for(auto to:t[v])if(to!=p&&!rem[to])
		ret+=tsize(to,v);
	return ret;
}

int findc(int v,int p,int s){
	int ret=1,mx=0;
	for(auto to:t[v])if(to!=p&&!rem[to]){
		int f=findc(to,v,s);
		if(f<=0)
			return f;
		else{
			ret+=f;
			chmax(mx,f);
		}
	}
	chmax(mx,s-ret);
	if(mx*2<=s)
		return -v;
	else
		return ret;
}

template<class E>
int find_center(vvc<E> t){
	int n=si(t);
	auto rec=[&](auto self,int v,int p)->int{
		int ret=1,mx=0;
		for(auto to:t[v])if(to!=p){
			int f=self(self,to,v);
			if(f<=0)
				return f;
			else{
				ret+=f;
				chmax(mx,f);
			}
		}
		chmax(mx,n-ret);
		if(mx*2<=n)
			return -v;
		else
			return ret;
	};
	return -rec(rec,0,-1);
}

template<class E>
int find_center(const vvc<E>&t,const vi&w){
	int tot=SUM(w);
	auto rec=[&](auto self,int v,int p)->int{
		int ret=w[v],mx=0;
		for(auto to:t[v])if(to!=p){
			int f=self(self,to,v);
			if(f>=inf)
				return f;
			else{
				ret+=f;
				chmax(mx,f);
			}
		}
		chmax(mx,tot-ret);
		if(mx*2<=tot)
			return v+inf;
		else
			return ret;
	};
	return rec(rec,0,-1)-inf;
}
