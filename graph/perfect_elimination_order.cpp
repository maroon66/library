//Baekjoon 8477
//Codechef 2020 December Long PALINDEQ
//Perfect Elimination Order
//p_i と隣接する p_j (i<j) は，clique をなす
template<class E>
vi perfect_elimination_order(const vvc<E>&g){
	int n=si(g);
	priority_queue<pi> pq;
	rep(i,n)pq.emplace(0,i);
	vi deg(n);
	vc<bool> used(n);
	vi res;
	while(si(pq)){
		int v=pq.top().b;pq.pop();
		if(used[v])continue;
		used[v]=true;
		res.pb(v);
		for(auto e:g[v]){
			if(!used[e])pq.emplace(++deg[e],e);
		}
	}
	reverse(all(res));
	return res;
}

vi inverse_permutation(const vi&p){
	int n=si(p);
	vi q(n);
	rep(i,n)q[p[i]]=i;
	return q;
}

//p に沿って頂点番号を振り直し，0,1,... が PEO になるようにする
//さらに，g 内の頂点をソートする(後々辺の存在判定などに活用)
//そして，g[i] ないから i 以下の辺を消し去る
void rearrange_graph(vvc<int>&g,const vi&p){
	int n=si(g);
	assert(si(p)==n);
	vvc<int> tmp(n);
	rep(i,n)swap(tmp[i],g[p[i]]);
	swap(g,tmp);
	vi q=inverse_permutation(p);
	rep(i,n){
		for(auto&j:g[i])j=q[j];
		sort(all(g[i]));
		g[i].erase(g[i].bg,upper_bound(all(g[i]),i));
	}
}

//rearrange されてるとする
//g の内部を破壊的に変更する!
bool is_peo(vvc<int>&g){
	int n=si(g);
	rep(i,n){
		rep(jj,si(g[i]))rng(kk,jj+1,si(g[i])){
			int j=g[i][jj],k=g[i][kk];
			if(!binary_search(all(g[j]),k))return false;
		}
	}
	return true;
}

//clique tree について
//g[i]=i の隣接頂点 v の中で i<v を満たすものとする
//p[i]=min(g[i])とすると，この木で clique tree が定義できる
//頂点 i に対応する頂点には，g[i]+{i} が入っていると思えばいい
