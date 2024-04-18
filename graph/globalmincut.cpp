//cost, vertices in T
//stress-tested, original:kactl
pair<int,vi> global_mincut(vvc<int> mat) {
	pair<int,vi> best(inf,{});
	int n=si(mat);
	vvc<int> co(n); rep(i,n)co[i].pb(i);
	rng(ph,1,n){
		vi w=mat[0];
		int s=0,t=0;
		rep(it,n-ph){ // O(V^2) -> O(E log V) with prio. queue
			w[t]=-inf;
			s=t; t=max_element(all(w))-w.bg;
			rep(i,n) w[i]+=mat[t][i];
		}
		chmin(best,mp(w[t]-mat[t][t],co[t]));
		co[s].insert(co[s].ed,all(co[t]));
		rep(i,n)mat[s][i]+=mat[t][i];
		rep(i,n)mat[i][s]=mat[s][i];
		mat[0][t]=-inf;
	}
	return best;
}
