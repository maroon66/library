
//VERIFY: yosupo
//KUPC2017J
//AOJDSL1A
//without rank
struct unionfind{
	vi p,s;
	int c;
	unionfind(int n):p(n,-1),s(n,1),c(n){}
	void clear(){
		fill(all(p),-1);
		fill(all(s),1);
		c=si(p);
	}
	int find(int a){
		return p[a]==-1?a:(p[a]=find(p[a]));
	}
	//set b to a child of a
	bool unite(int a,int b){
		a=find(a);
		b=find(b);
		if(a==b)return false;
		p[b]=a;
		s[a]+=s[b];
		c--;
		return true;
	}
	bool same(int a,int b){
		return find(a)==find(b);
	}
	int sz(int a){
		return s[find(a)];
	}
};

vc<pair<int*,int>> buf;
void save(int&a){
	buf.eb(&a,a);
}
int snapshot(){
	return si(buf);
}
void rollback(int w){
	while(si(buf)>w){
		auto [a,b]=buf.back();buf.pop_back();
		*a=b;
	}
}

//Unionfind with Rollback
//CF Global 24 H
//https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum
struct unionfind{
	vi p,s;
	int c;
	unionfind(int n):p(n,-1),s(n,1),c(n){}
	int find(int a){
		return p[a]==-1?a:find(p[a]);
	}
	int unite(int a,int b){
		a=find(a);
		b=find(b);
		if(a==b)return a;
		if(s[a]<s[b])swap(a,b);
		save(p[b]);
		save(s[a]);
		p[b]=a;
		s[a]+=s[b];
		save(c);
		c--;
		return a;
	}
	bool same(int a,int b){
		return find(a)==find(b);
	}
	int sz(int a){
		return s[find(a)];
	}
};

//Unionfind with Rollback
//各点に 01 の情報を持っていて，二部グラフ的でないと怒る
//CF680E
