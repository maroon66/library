//x_1<x_2<x_3<... に対する重み
//in[x_1]* wei[x_i] * f[x_{i+1}-x{i}] -> out[x_k]
//CF think-cell 1
vc<mint> relaxed_convolution(const vc<mint>&in,const vc<mint>&wei,const vc<mint>&f){
	int n=si(in);
	assert(si(wei)==n);
	assert(si(f)>=n);
	
	int s=minp2(n);
	int L=__lg(s);
	vvc<mint> fs(L+1);
	rep(i,L+1){
		fs[i].resize(1<<i);
		rep(j,min(si(fs[i]),si(f)))fs[i][j]=f[j];
		inplace_fmt(fs[i],false);
	}
	vc<mint> g(s);
	
	vc<mint> res=in;
	auto dfs=[&](auto self,int l,int r,int lv)->void{
		assert(l<n);
		if(r-l==1){
			res[l]*=wei[l];
		}else{
			int m=(l+r)/2;
			self(self,l,m,lv-1);
			if(m<n){
				int len=r-l;
				g.assign(len,0);
				rep(i,len/2)g[i]=res[l+i];
				inplace_fmt(g,false);
				rep(i,len)g[i]*=fs[lv][i];
				inplace_fmt(g,true);
				rng(i,len/2,min(len,n-l))res[l+i]+=g[i];
				self(self,m,r,lv-1);
			}
		}
	};
	dfs(dfs,0,s,L);
	
	return res;
}
