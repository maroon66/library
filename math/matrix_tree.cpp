//SRM551H
struct matrix_tree{
	vvc<mint> w;
	matrix_tree(int n):w(n-1,vc<mint>(n-1)){}
	void ae(int a,int b){
		if(a>b)swap(a,b);
		if(a<si(w))w[a][a]+=1;
		if(b<si(w)){
			w[a][b]-=1;
			w[b][a]-=1;
			w[b][b]+=1;
		}
	}
	mint calc(){
		return det(w);
	}
};


//USACO2021 USOPEN Platinum C
//directed tree rooted at v
//v -> other vertices
struct matrix_tree_directed{
	int n;
	vvc<mint> w;
	matrix_tree_directed(int nn=0):n(nn),w(n,vc<mint>(n)){}
	void ae(int a,int b){
		w[a][b]-=1;
		w[b][b]+=1;
	}
	mint calc(int v){
		vvc<mint> a(n-1,vc<mint>(n-1));
		rep(i,n-1)rep(j,n-1){
			a[i][j]=w[i+(i>=v)][j+(j>=v)];
		}
		return det(a);
	}
};

//全点についてそれを根にしたやつがほしい→adjugate

//USACO2021 USOPEN Platinum C
//Verified only for connected eulerian graph
struct best{
	int n;
	unionfind uf;
	vc<pi> es;
	vi in,out;
	best(int nn):n(nn),uf(n),in(n),out(n){}
	void ae(int a,int b){
		es.eb(a,b);
		uf.unite(a,b);
		in[b]++;
		out[a]++;
	}
	mint calc(){
		if(in!=out)return 0;
		vvc<int> ls(n);
		vc<matrix_tree_directed> x(n);
		rep(i,n)if(uf.find(i)==i)x[i]=matrix_tree_directed(uf.sz(i));
		rep(i,n)ls[uf.find(i)].pb(i);
		for(auto [a,b]:es){
			int i=uf.find(a);
			a=lwb(ls[i],a);
			b=lwb(ls[i],b);
			x[i].ae(a,b);
		}
		mint ans=1;
		rep(i,n)if(in[i])ans*=fact[in[i]-1];
		rep(i,n)if(uf.find(i)==i)ans*=x[i].calc(0);
		return ans;
	}
};
