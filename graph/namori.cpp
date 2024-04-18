//gr cycles
//rt v->root of the tree
//AGC8 E
struct Namori{
	const vi&p;
	int n;
	vvc<int> gr;
	vi rt;
	Namori(const vi& pp):p(pp),n(p.size()),rt(n){
		vi c(n);
		rep(i,n)c[p[i]]++;
		vi ls;
		rep(i,n)if(c[i]==0)
			ls.pb(i);
		int j=0;
		while(j<int(ls.size())){
			int i=ls[j++];
			if(--c[p[i]]==0)
				ls.pb(p[i]);
		}
		rep(i,n)if(c[i]){
			int x=i;
			vi v;
			do{
				c[x]=0;
				rt[x]=x;
				v.pb(x);
				x=p[x];
			}while(x!=i);
			gr.pb(v);
		}
		per(ii,ls.size()){
			int i=ls[ii];
			rt[i]=rt[p[i]];
		}
	}
};

//gr cycles
//rt v->root of the tree
//p par
//AGC005 F (only for one connected component)
struct Namori{
	const vvc<int>&g;
	int n;
	vvc<int> gr;
	vi rt,p;
	Namori(const vvc<int>&gg):g(gg),n(g.size()),rt(n),p(n,-1){
		vi c(n);
		rep(i,n)c[i]=g[i].size();
		vi ls;
		rep(i,n)if(c[i]==1)
			ls.pb(i);
		int j=0;
		while(j<int(ls.size())){
			int i=ls[j++];
			for(auto to:g[i])if(c[to]>1){
				p[i]=to;
				if(--c[to]==1)
					ls.pb(to);
			}
		}
		rep(i,n)if(c[i]>1){
			assert(c[i]==2);
			
			int x=i;
			vi v;
			while(x!=-1){
				c[x]=1;
				rt[x]=x;
				v.pb(x);
				int nx=-1;
				for(auto y:g[x])
					if(c[y]==2)
						nx=y;
				x=nx;
			}
			gr.pb(v);
		}
		assert(c==vi(n,1));
		per(ii,ls.size()){
			int i=ls[ii];
			rt[i]=rt[p[i]];
		}
	}
};
