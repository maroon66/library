//i -> j (i>j) のみ
//ABC229H
template<int L>
vi surreal(vc<array<vi,2>> g){
	auto sub=[&](int l,int r){
		per(lv,L+1){
			int v=((l>>lv)+1)<<lv;
			if(v<r)return v;
		}
		assert(false);
	};
	int n=si(g);
	vi res(n);
	rep(i,n){
		int mx=-inf,mn=inf;
		for(auto j:g[i][0])chmax(mx,res[j]);
		for(auto j:g[i][1])chmin(mn,res[j]);
		assert(mx<mn);
		if(mx<0&&0<mn){
			res[i]=0;
		}else if(mn<=0){
			res[i]=-sub(-mn,-mx);
		}else if(0<=mx){
			res[i]=sub(mx,mn);
		}
	}
	return res;
}
