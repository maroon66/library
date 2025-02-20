//重み i の石が A_i 種類あるという状況を考えます．
//これらを並べ，重みの合計が n になるようにします．
//回転で一致するものは省きます．
//何通りありますか？
//multiuni2024-10-M
Poly<mint> polya_circle_weighted(const Poly<mint>&a){
	int n=si(a);
	assert(a[0]==0);
	auto b=(1-a).log(n,invs);
	Poly<mint> c(n);
	rng(d,1,n){
		mint w=-phi[d]*invs[d];
		for(int i=1;i*d<n;i++)
			c[i*d]+=w*b[i];
	}
	return c;
}

//重み i の石が A_i 種類あるという状況を考えます．
//石の多重集合であって，重みの合計が n になるものは何個あるでしょうか？
//multiuni2024-10-M
Poly<mint> polya_exp(const Poly<mint>&a){
	int n=si(a);
	assert(a[0]==0);
	Poly<mint> b(n);
	rng(d,1,n){
		for(int i=1;i*d<n;i++)
			b[i*d]+=invs[d]*a[i];
	}
	return b.exp(n,invs);
}
