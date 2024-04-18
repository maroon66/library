template<class E>
pair<vi,vc<E>> get_tree_path(const vvc<E>&t,int a,int b){
	vi vs;
	vc<E> es;
	auto rec=[&](auto self,int v,int p)->bool{
		vs.pb(v);
		if(v==b)return true;
		for(auto e:t[v])if(e!=p){
			es.pb(e);
			if(self(self,e,v))return true;
			es.pop_back();
		}
		vs.pop_back();
		return false;
	};
	rec(rec,a,-1);
	return mp(vs,es);
}

//Newton 2022 Triwizard F
//パス上の頂点を取り出して森に分解
	vi vs=get_tree_path(t,S,T).a;
	int L=si(vs);
	{
		auto del=[&](int v,int p){
			t[v].erase(find(all(t[v]),p));
		};
		rep(i,L-1){
			del(vs[i],vs[i+1]);
			del(vs[i+1],vs[i]);
		}
	}
