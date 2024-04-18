//Yukicoder No.1773
//辺の組があって閉路ができないようにとる
//N 頂点
int linear_matroid_parity(int n,vc<pair<pi,pi>> es){
	vvc<mint> w(n,vc<mint>(n));
	for(auto [e,f]:es){
		mint x=rand_int(1,mint::mod-1);
		w[e.a][f.a]+=x;
		w[e.a][f.b]-=x;
		w[e.b][f.a]-=x;
		w[e.b][f.b]+=x;
		w[f.a][e.a]-=x;
		w[f.a][e.b]+=x;
		w[f.b][e.a]+=x;
		w[f.b][e.b]-=x;
	}
	return matrank(w)/2;
}
