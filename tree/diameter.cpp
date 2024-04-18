//AGC33 C
template<class E>
struct treediameter{
	const vvc<E>&g;
	int r1,r2;
	vi d1,d2;
	int dia;
	void dfs(int v,int p,int cur,vi&d){
		d[v]=cur;
		for(auto e:g[v])if(e!=p)
			dfs(e,v,cur+1,d);
	}
	treediameter(const vvc<E>&gg):g(gg),d1(g.size()),d2(g.size()){
		dfs(0,-1,0,d1);
		r1=max_element(all(d1))-d1.bg;
		dfs(r1,-1,0,d1);
		r2=max_element(all(d1))-d1.bg;
		dfs(r2,-1,0,d2);
		dia=d1[r2];
	}
};

//https://pjudge.ac/contest/892/problem/21622
template<class E>
struct treediameter{
	const vvc<E>&g;
	int r1,r2;
	vi d1,d2;
	int dia;
	void dfs(int v,int p,int cur,vi&d){
		d[v]=cur;
		for(auto e:g[v])if(e!=p)
			dfs(e,v,cur+e.cost,d);
	}
	treediameter(const vvc<E>&gg):g(gg),d1(g.size()),d2(g.size()){
		dfs(0,-1,0,d1);
		r1=max_element(all(d1))-d1.bg;
		dfs(r1,-1,0,d1);
		r2=max_element(all(d1))-d1.bg;
		dfs(r2,-1,0,d2);
		dia=d1[r2];
	}
};
struct E{
	int to,cost;
	operator int()const{return to;}
};
