//(v,p) の前に (ch,v) が全部出てくる例のリスト
//UCUP 2-18-A
vc<pi> vp_list(const vvc<int>&t){
	vc<pi> res;
	auto dfs=[&](auto self,int v,int p)->void{
		for(auto ch:t[v])if(ch!=p)
			self(self,ch,v);
		if(p!=-1)res.eb(v,p);
	};
	dfs(dfs,0,-1);
	auto tmp=res;
	rein(tmp);
	for(auto&[a,b]:tmp)swap(a,b);
	pb(res,tmp);
	rep(i,si(t))res.eb(i,-1);
	return res;
}

//部分木の圧縮？UCUP-2-18-A
