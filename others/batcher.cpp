//並列ソート
// x[i],x[i+step] の大小を揃える，のリスト
vc<pair<int,vi>> batcher(int n){
	vc<pair<int,vi>> res;
	for(int p=1;p<n;p*=2)
		for(int k=p;k>=1;k/=2){
			vi ls;
			for(int i=0;i<n;i+=2*p)
				rng(ini,k%p,k%p+k)
					for(int j=ini;j+k<min(n-i,2*p);j+=2*k)
						ls.pb(i+j);
			res.eb(k,ls);
		}
	return res;
}
