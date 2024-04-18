struct und{
	vvc<pi> buf;
	und(int n):buf(n){}
	void add(int a,int b,int c){
		if(a>b)swap(a,b);
		buf[a].eb(b,c);
	}
	void init(){
		for(auto&x:buf){
			soin(x);
			int s=0;
			for(auto [v,c]:x){
				if(s==0||x[s-1].a!=v){
					x[s++]=pi(v,c);
				}
			}
			x.resize(s);
		}
	}
	int ask(int a,int b){
		if(a>b)swap(a,b);
		int i=lwb(buf[a],pi(b,-1));
		if(i<si(buf[a])&&buf[a][i].a==b)
			return buf[a][i].b;
		else
			return -1;
	}
};
