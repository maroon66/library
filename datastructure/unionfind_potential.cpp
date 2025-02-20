//ICPC WF 2017 H
template<class N>
struct potential_uf{
	vc<N> x;
	vi p,s;
	int c;
	potential_uf(vc<N> ini):x(ini),p(si(ini),-1),s(si(ini),1),c(si(ini)){}
	potential_uf(int n):x(n),p(n,-1),s(n,1),c(n){}
	void clear(){
		fila(x,N{});
		fila(p,-1);
		fila(s,1);
		c=si(p);
	}
	int find(int a){
		if(p[a]==-1){
			return a;
		}else{
			int v=find(p[a]);
			if(p[a]!=v){
				x[a]+=x[p[a]];
				p[a]=v;
			}
			return v;
		}
	}
	N get(int a){
		int v=find(a);
		if(v==a)return x[a];
		else return x[a]+x[v];
	}
	//set b to a child of a
	bool unite(int a,int b){
		a=find(a);
		b=find(b);
		if(a==b)return false;
		p[b]=a;
		s[a]+=s[b];
		x[b]-=x[a];
		c--;
		return true;
	}
	N& operator[](int i){
		return x[i];
	}
	bool same(int a,int b){
		return find(a)==find(b);
	}
	int sz(int a){
		return s[find(a)];
	}
};
//multiuni2024-8-C
//uf[uf.find(v)]+=C+uf.get(u)-uf.get(v)
