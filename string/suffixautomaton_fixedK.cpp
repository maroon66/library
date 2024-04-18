//suffix automaton 文字種数定数
//reserveいる？？
//CF869F
template<int K>
struct suffixautomaton{
	struct N{
		//len:ノードに対応する文字列の最長の長さ
		//suf:suffix link
		//org:clone元のノード
		int len,suf,org,to[K];
		N():len(0),suf(-1),org(-1){one(to);}
		N(int a,int b,int c):len(a),suf(b),org(c){one(to);}
	};
	vc<N> x;
	suffixautomaton():x(1){x.reserve(200000);}
	int add(int a,int c){
		assert(x[a].to[c]!=-1);
		int b=x[a].to[c];
		if(x[a].len+1==x[b].len)
			return b;
		else{
			int w=si(x);
			x.pb(x[b]);
			x[w].len=x[a].len+1;
			x[w].org=b;
			while(a!=-1&&x[a].to[c]==b){
				x[a].to[c]=w;
				a=x[a].suf;
			}
			return x[b].suf=w;
		}
	}
	int extend(int a,int c){
		if(x[a].to[c]!=-1)return add(a,c);
		int cur=si(x);
		x.eb(x[a].len+1,0,-1);
		while(a!=-1&&x[a].to[c]==-1){
			x[a].to[c]=cur;
			a=x[a].suf;
		}
		if(a!=-1){
			int z=add(a,c);
			x[cur].suf=z;
		}
		return cur;
	}
	vi toposort(){
		int n=x.size();
		vi c(n);
		rep(i,n)rep(k,K)if(int j=x[i].to[k];j!=-1)c[j]++;
		vi res(n);
		int b=0,e=0;
		rep(i,n)if(c[i]==0)res[e++]=i;
		while(e<n){
			int i=res[b++];
			rep(k,K)if(int j=x[i].to[k];j!=-1)
				if(--c[j]==0)res[e++]=j;
		}
		return res;
	}
	//Codechef 2021 August Lunchtime Longest Spanning Substrings
	//suf によってできる木の葉から順に削って行ったときの順序を返す
	//DP などに使えると言われている
	vi treeindex(){
		int n=x.size();
		vi c(n);
		rng(i,1,n)c[x[i].suf]++;
		vi res;
		rep(i,n)if(c[i]==0)res.pb(i);
		rep(i,n-1){
			int v=res[i];
			if(--c[x[v].suf]==0)res.pb(x[v].suf);
		}
		return res;
	}
	int size()const{return si(x);}
	N& operator[](int i){
		return x[i];
	}
	void show(){
		int n=si(x);
		vvc<int> buf(n);
		rep(i,n)rep(k,K)if(int j=x[i].to[k];j!=-1){
			if(si(buf[j])<si(buf[i])+1){
				buf[j]=buf[i];
				buf[j].pb(k);
			}
			cerr<<i<<" "<<buf[i]<<endl;
		}
	}
};
