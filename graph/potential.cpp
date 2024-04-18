struct E{
	int to,cost;
};

vi potential(const vvc<E>&g){
	int n=si(g);
	vi a(n);
	rep(_,n+1){
		bool upd=false;
		rep(i,n){
			for(auto e:g[i]){
				if(chmin(a[e.to],a[i]+e.cost)){
					upd=true;
				}
			}
		}
		if(!upd)return a;
	}
	return {};
}

struct solve_inequality{
	int n;
	vvc<E> g;
	solve_inequality(int nn):n(nn),g(n+1){}
	//v<=a[i]
	void set_lower_bound(int i,int v){
		g[i].pb({n,-v});
	}
	//a[i]<=v
	void set_upper_bound(int i,int v){
		g[n].pb({i,v});
	}
	//lw<=a[i]<=up
	void set_bounds(int i,int lw,int up){
		set_lower_bound(i,lw);
		set_upper_bound(i,up);
	}
	//a[i]-a[j]<=v
	void set_inequality(int i,int j,int v){
		g[j].pb({i,v});
	}
	//lw<=a[i]-a[j]<=up
	void set_inequality2(int i,int j,int lw,int up){
		set_inequality(i,j,up);
		set_inequality(j,i,-lw);
	}
	//a[i]-a[j]=v
	void set_equality(int i,int j,int v){
		set_inequality(i,j,v);
		set_inequality(j,i,-v);
	}
	vi solve(){
		vi a=potential(g);
		if(a.empty())return {};
		rep(i,n)a[i]-=a[n];
		a.pop_back();
		return a;
	}
};
