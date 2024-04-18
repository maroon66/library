//exponential gf を返す
//実際使うときは fact[i] をかける
//ans,exp(ans)
//UOJ Goodbye Jihai D
pair<vc<mint>,vc<mint>> connected_graphs(int n){
	Poly<mint> f(n+1);
	rep(i,n+1)f[i]=mint(2).pow(i*(i-1)/2)*finv[i];
	auto g=f.log(n+1,invs);
	return mp(g,f);
}

//exponential gf を返す
//実際使うときは fact[i] をかける
//ans,exp(ans)
//UOJ Goodbye Jihai D
pair<vc<mint>,vc<mint>> connected_bipartite_graphs(int n){
	Poly<mint> a(n+1);
	rep(i,n+1)a[i]=invs[2].pow(i*(i-1)/2)*finv[i];
	Poly<mint> f=a*a;
	f.resize(n+1);
	rep(i,n+1)f[i]*=mint(2).pow(i*(i-1)/2);
	auto g=f.log(n+1,invs);
	rep(i,n+1)g[i]*=invs[2];
	return mp(g,f);
}

//exponential gf を返す
//実際使うときは fact[i] をかける
//1 component あたりの通り数 c
//それの exp e を用いて連結成分の個数を繰り出す
//c に bipartite の個数，e に普通のグラフの個数を入れることで，
//ランダムグラフ中の bipartite components の数，などにも対応している
//UOJ Goodbye Jihai D
vc<mint> num_of_conponents(const vc<mint>&c,const vc<mint>&e){
	int n=si(c);
	auto res=multiply(c,e);
	res.resize(n);
	return res;
}
