
struct twosat{
	int n;
	vvc<int> g;
	twosat(int nn):n(nn),g(n*2){}
	//x/2 が変数の番号，x%2 で 0,1 を表す
	//条件 x,y のどちらかを満たす，という制約を追加
	void add_or(int x,int y){
		g[x^1].pb(y);
		g[y^1].pb(x);
	}
	void add_or(int x,int xv,int y,int yv){
		add_or(x*2+xv,y*2+yv);
	}
	//x&y ではない，という制約を追加
	void add_nand(int x,int y){
		g[x].pb(y^1);
		g[y].pb(x^1);
	}
	void add_nand(int x,int xv,int y,int yv){
		add_nand(x*2+xv,y*2+yv);
	}
	//solve the 2-sat
	//充足する 0/1 列を返す
	//充足不可能な場合は {} を返す
	vi solve(){
		scc<int> s(g,false);
		rep(i,n)if(s.bl[i*2]==s.bl[i*2+1])return {};
		vi res(n,-1);
		per(i,s.s)
			for(auto j:s.idx[i])
				if(res[j/2]==-1)
					res[j/2]=j%2;
		return res;
	}
	//solve the 2-sat
	//yes/no だけ，ちょっと軽量
	/*bool solve(){
		scc<int> s(g);
		rep(i,n)if(s.bl[i*2]==s.bl[i*2+1])return false;
		return true;
	}*/
};
