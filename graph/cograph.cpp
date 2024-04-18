//O(N^3) とか
//cograph であることが保証されている場合のみ動く
//そうでないときは・・・適当に
//先頭 N 項がもとの頂点に対応
//Xmas2012 C
struct cotree{
	int s,rt;
	vvc<int> t;
	vi k;//-1:leaf, 0:union, 1:join
};
cotree cotree_decomp(const vvc<bool>&g){
	int n=si(g);
	vvc<int> t(n);
	vi k(n,-1);
	auto add=[&](){
		int res=si(t);
		t.eb();
		k.pb(0);
		return res;
	};
	auto rec=[&](auto self,vi idx)->int{
		int s=si(idx);
		if(s==1){
			return idx[0];
		}
		int res=add();
		unionfind a(s),b(s);
		rep(i,s)rep(j,i)
			if(g[idx[i]][idx[j]])a.unite(i,j);
			else b.unite(i,j);
		if(b.c>1){
			swap(a,b);
			k[res]=1;
		}
		assert(a.c>1);
		vi tmp(s);iota(all(tmp),0);
		sort(all(tmp),[&](int i,int j){
			return a.find(i)<a.find(j);
		});
		for(int i=0;i<s;){
			int j=i;
			while(j<s&&a.same(tmp[i],tmp[j]))j++;
			vi z(j-i);
			rng(q,i,j)z[q-i]=idx[tmp[q]];
			int ch=self(self,z);
			t[res].pb(ch);
			i=j;
		}
		return res;
	};
	vi idx(n);iota(all(idx),0);
	int rt=rec(rec,idx);
	return {si(t),rt,t,k};
}
