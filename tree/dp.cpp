//有向木じゃダメ！！
//AGC034E
//XX Opencup GP of Korea
//CF446D
//int(E)->to
//par[0] が -1 でないことに注意してほしい
//sub[v] -> vの部分木情報
//bus[v] -> p を根にした部分木 (v) 含まずの情報
//Universal Cup 2023 Stage 1
//頂点や辺の情報を重複して入れないようにした
//数え上げなどに使える
//CF902E
//辺の向きに意味がある問題でverify
template<class E,class N>
struct treedp{
	const vvc<E>& g;
	vc<N> a,b,res,sub,bus;
	vc<E> par;
	void dfs1(int v,int p){
		a[v].init(v);
		for(auto e:g[v])
			if(e!=p){
				dfs1(e,v);
				b[e]=a[v];
				a[v]=a[v]+a[e];
			}else{
				par[v]=e;
			}
		sub[v]=a[v];
		if(p!=-1)a[v]=a[v].up(par[v]);
	}
	void dfs2(int v,int p,N cur){
		bool f=p==-1;
		per(i,g[v].size()){
			auto e=g[v][i];
			if(e==p)continue;
			if(f){
				bus[e]=b[e];
				cur=a[e];
				f=false;
			}else{
				bus[e]=cur+b[e];
				cur=cur+a[e];
			}
			dfs2(e,v,bus[e].up(e));
		}
		N tmp;tmp.init(v);
		if(f)res[v]=tmp;
		else res[v]=cur+tmp;
	}
	treedp(const vvc<E>&gg):g(gg),a(si(g)),b(si(g)),res(si(g)),sub(si(g)),bus(si(g)),par(si(g)){
		{//make sure the graph is undirected
			int sum=0;
			rep(i,si(gg))sum+=si(g[i]);
			assert(sum==2*(si(gg)-1));
		}
		dfs1(0,-1);
		dfs2(0,-1,N());
	}
	N& getval(int v,int p){
		if(par[v]==p)return sub[v];
		else return bus[p];
	}
};

//Bytecamp 2022 Day4 E
//部分木の根から葉までの最長パスの長さを求めている
struct N{
	int mx;
	void init(int){
		mx=0;
	}
	N up(int)const{
		return N{mx+1};
	}
	N operator+(const N&r)const{
		return N{max(mx,r.mx)};
	}
};

//usage
vi cnt;
struct N{
	int v,a,b,c;
	pi mx;
	void init(int vv){
		v=vv;
		a=cnt[v];
		b=c=0;
		mx=pi(0,0);
	}
	N up(int to)const{
		return N{to,a+cnt[to],b+a,c+a,pi(b+a,c+a)};
	}
	N operator+(const N&r)const{
		assert(v==r.v);
		int sum=b+r.b;
		int mn;
		pi z=max(mx,r.mx);
		if(z.b>=sum-z.a)
			mn=z.b-(sum-z.a);
		else
			mn=sum%2;
		return N{v,a+r.a-cnt[v],sum,mn,z};
	}
};

signed main(){
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(20);
	
	int n;cin>>n;
	string s;cin>>s;
	rep(i,n)cnt.pb(s[i]-'0');
	vvc<int> g(n);
	rep(i,n-1){
		int a,b;cin>>a>>b;
		a--;b--;
		g[a].pb(b);
		g[b].pb(a);
	}
	
	auto dp=treedp<N,int>(g).res;
	const int inf=LLONG_MAX/3;
	int ans=inf;
	rep(i,n)if(dp[i].c==0)
		chmin(ans,dp[i].b);
	if(ans==inf)cout<<-1<<endl;
	else cout<<ans/2<<endl;
}

//定数倍高速化
//CF854H
const int nmax=5010;
template<class E,class N>
struct treedp{
	const vvc<E>& g;
	static N a[nmax],b[nmax],res[nmax],sub[nmax],bus[nmax];
	static E par[nmax];
	N tmp;
	void dfs1(int v,int p){
		a[v].init(v);
		for(auto e:g[v])
			if(e!=p){
				dfs1(e,v);
				b[e].copy(a[v]);
				N::add(a[v],a[e],tmp);
				a[v].copy(tmp);
			}else{
				par[v]=e;
			}
		sub[v].copy(a[v]);
		if(p!=-1){
			a[v].up(par[v],tmp);
			a[v].copy(tmp);
		}
	}
	void dfs2(int v,int p){
		bool f=p==-1;
		per(i,g[v].size()){
			auto e=g[v][i];
			if(e==p)continue;
			if(f){
				//bus[e]=b[e];
				//res[v]=a[e];
				bus[e].copy(b[e]);
				res[v].copy(a[e]);
				f=false;
			}else{
				//bus[e]=res[v]+b[e];
				N::add(res[v],b[e],bus[e]);
				//res[v]=res[v]+a[e];
				N::add(res[v],a[e],tmp);
				res[v].copy(tmp);
			}
			//res[e]=bus[e].up(e);
			bus[e].up(e,res[e]);
			dfs2(e,v);
		}
		if(f)res[v].init(v);
		else{
			static N tmp2;tmp2.init(v);
			N::add(res[v],tmp2,tmp);
			res[v].copy(tmp);
		}
	}
	treedp(const vvc<E>&gg):g(gg){
		{//make sure the graph is undirected
			int sum=0;
			rep(i,si(gg))sum+=si(g[i]);
			assert(sum==2*(si(gg)-1));
		}
		dfs1(0,-1);
		dfs2(0,-1);
	}
	N& getval(int v,int p){
		if(par[v]==p)return sub[v];
		else return bus[p];
	}
};
template<class E,class N>N treedp<E,N>::a[nmax];
template<class E,class N>N treedp<E,N>::b[nmax];
template<class E,class N>N treedp<E,N>::res[nmax];
template<class E,class N>N treedp<E,N>::sub[nmax];
template<class E,class N>N treedp<E,N>::bus[nmax];
template<class E,class N>E treedp<E,N>::par[nmax];

struct E{
	int to,idx;
	operator int()const{return to;}
};
