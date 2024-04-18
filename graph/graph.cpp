//vector<vector<>> をやめた
//本当に速いのか謎
template<class E>
struct Graph{
	const int n;
	vi head;
	vc<pair<int,E>> es;
	Graph(int nn):n(nn),head(n){clear();}
	void clear(){
		rep(i,n)head[i]=-1;
		es.clear();
	}
	void ae(int v,E e){
		int i=si(es);
		es.eb(head[v],e);
		head[v]=i;
	}
	void aeu(int a,int b,int c){
		ae(a,{b,c});
		ae(b,{a,c});
	}
	template<class F>
	void trav(int v,F f){
		for(int e=head[v];e!=-1;e=es[e].a){
			f(es[e].b);
		}
	}
};
