#ifndef LOCAL
#pragma GCC optimize ("Ofast")
#pragma GCC optimize ("unroll-loops")
#endif

#include <bits/stdc++.h>
using namespace std;

using ll=long long;
#define int ll

#define rng(i,a,b) for(int i=int(a);i<int(b);i++)
#define rep(i,b) rng(i,0,b)
#define gnr(i,a,b) for(int i=int(b)-1;i>=int(a);i--)
#define per(i,b) gnr(i,0,b)
#define pb push_back
#define eb emplace_back
#define a first
#define b second
#define bg begin()
#define ed end()
#define all(x) x.bg,x.ed
#define si(x) int(x.size())
#ifdef LOCAL
#define dmp(x) cerr<<__LINE__<<" "<<#x<<" "<<x<<endl
#else
#define dmp(x) void(0)
#endif

template<class t,class u> bool chmax(t&a,u b){if(a<b){a=b;return true;}else return false;}
template<class t,class u> bool chmin(t&a,u b){if(b<a){a=b;return true;}else return false;}

template<class t> using vc=vector<t>;
template<class t> using vvc=vc<vc<t>>;

using pi=pair<int,int>;
using vi=vc<int>;

template<class t,class u>
ostream& operator<<(ostream& os,const pair<t,u>& p){
	return os<<"{"<<p.a<<","<<p.b<<"}";
}

template<class t> ostream& operator<<(ostream& os,const vc<t>& v){
	os<<"{";
	for(auto e:v)os<<e<<",";
	return os<<"}";
}

#define mp make_pair
#define mt make_tuple
#define one(x) memset(x,-1,sizeof(x))
#define zero(x) memset(x,0,sizeof(x))
#ifdef LOCAL
void dmpr(ostream&os){os<<endl;}
template<class T,class... Args>
void dmpr(ostream&os,const T&t,const Args&... args){
	os<<t<<" ";
	dmpr(os,args...);
}
#define dmp2(...) dmpr(cerr,__LINE__,##__VA_ARGS__)
#else
#define dmp2(...) void(0)
#endif

using uint=unsigned;
using ull=unsigned long long;

template<class t,size_t n>
ostream& operator<<(ostream&os,const array<t,n>&a){
	return os<<vc<t>(all(a));
}

template<int i,class T>
void print_tuple(ostream&,const T&){
}

template<int i,class T,class H,class ...Args>
void print_tuple(ostream&os,const T&t){
	if(i)os<<",";
	os<<get<i>(t);
	print_tuple<i+1,T,Args...>(os,t);
}

template<class ...Args>
ostream& operator<<(ostream&os,const tuple<Args...>&t){
	os<<"{";
	print_tuple<0,tuple<Args...>,Args...>(os,t);
	return os<<"}";
}

template<class t>
void print(t x,int suc=1){
	cout<<x;
	if(suc==1)
		cout<<"\n";
	if(suc==2)
		cout<<" ";
}

ll read(){
	ll i;
	cin>>i;
	return i;
}

vi readvi(int n,int off=0){
	vi v(n);
	rep(i,n)v[i]=read()+off;
	return v;
}

pi readpi(int off=0){
	int a,b;cin>>a>>b;
	return pi(a+off,b+off);
}

template<class t,class u>
void print(const pair<t,u>&p,int suc=1){
	print(p.a,2);
	print(p.b,suc);
}

template<class T>
void print(const vector<T>&v,int suc=1){
	rep(i,v.size())
		print(v[i],i==int(v.size())-1?suc:2);
}

template<class T>
void print_offset(const vector<T>&v,ll off,int suc=1){
	rep(i,v.size())
		print(v[i]+off,i==int(v.size())-1?suc:2);
}

template<class T,size_t N>
void print(const array<T,N>&v,int suc=1){
	rep(i,N)
		print(v[i],i==int(N)-1?suc:2);
}

string readString(){
	string s;
	cin>>s;
	return s;
}

template<class T>
T sq(const T& t){
	return t*t;
}

//#define CAPITAL
void yes(bool ex=true){
	#ifdef CAPITAL
	cout<<"YES"<<"\n";
	#else
	cout<<"Yes"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void no(bool ex=true){
	#ifdef CAPITAL
	cout<<"NO"<<"\n";
	#else
	cout<<"No"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void possible(bool ex=true){
	#ifdef CAPITAL
	cout<<"POSSIBLE"<<"\n";
	#else
	cout<<"Possible"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void impossible(bool ex=true){
	#ifdef CAPITAL
	cout<<"IMPOSSIBLE"<<"\n";
	#else
	cout<<"Impossible"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}

constexpr ll ten(int n){
	return n==0?1:ten(n-1)*10;
}

const ll infLL=LLONG_MAX/3;

#ifdef int
const int inf=infLL;
#else
const int inf=INT_MAX/2-100;
#endif

int topbit(signed t){
	return t==0?-1:31-__builtin_clz(t);
}
int topbit(ll t){
	return t==0?-1:63-__builtin_clzll(t);
}
int botbit(signed a){
	return a==0?32:__builtin_ctz(a);
}
int botbit(ll a){
	return a==0?64:__builtin_ctzll(a);
}
int botbit(ull a){
	return a==0?64:__builtin_ctzll(a);
}
int popcount(signed t){
	return __builtin_popcount(t);
}
int popcount(ll t){
	return __builtin_popcountll(t);
}
int popcount(ull t){
	return __builtin_popcountll(t);
}
bool ispow2(int i){
	return i&&(i&-i)==i;
}
ll mask(int i){
	return (ll(1)<<i)-1;
}

bool inc(int a,int b,int c){
	return a<=b&&b<=c;
}

template<class t> void mkuni(vc<t>&v){
	sort(all(v));
	v.erase(unique(all(v)),v.ed);
}

ll rand_int(ll l, ll r) { //[l, r]
	static mt19937_64 gen;
	return uniform_int_distribution<ll>(l, r)(gen);
}

ll rand_int(ll k){ //[0,k)
	return rand_int(0,k-1);
}

template<class t>
void myshuffle(vc<t>&a){
	rep(i,si(a))swap(a[i],a[rand_int(0,i)]);
}

template<class t>
int lwb(const vc<t>&v,const t&a){
	return lower_bound(all(v),a)-v.bg;
}

vvc<int> readGraph(int n,int m){
	vvc<int> g(n);
	rep(i,m){
		int a,b;
		cin>>a>>b;
		//sc.read(a,b);
		a--;b--;
		g[a].pb(b);
		g[b].pb(a);
	}
	return g;
}

vvc<int> readTree(int n){
	return readGraph(n,n-1);
}

vc<ll> presum(const vi&a){
	vc<ll> s(si(a)+1);
	rep(i,si(a))s[i+1]=s[i]+a[i];
	return s;
}

string curtestname;
void SetName(string name){
	curtestname=name;
}

void OpenFile(){
	cout.flush();//これしないと今のファイルに書き込む前に新しいファイル開いてしまう
	static map<string,int> cnt;
	stringstream stream;
	stream<<"../in/"<<curtestname<<"-"<<setw(3)<<setfill('0')<<++cnt[curtestname];
	stream<<".txt";
	cerr<<"\nOpen "<<stream.str()<<"\n"<<endl;
	#ifndef LOCAL
	freopen(stream.str().c_str(), "w", stdout);
	#endif
}

//VERIFY: yosupo
//KUPC2017J
//AOJDSL1A
//without rank
struct unionfind{
	vi p,s;
	int c;
	unionfind(int n):p(n,-1),s(n,1),c(n){}
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

vi genperm(int n){
	vi perm(n);iota(all(perm),0);
	myshuffle(perm);
	return perm;
}

vi kofn(int n,int k){
	vi res=genperm(n);
	res.resize(k);
	return res;
}

//AGC058F
vc<pi> gentree(int n,int t){
	vc<pi> es;
	if(t==0){
		//line
		rep(i,n-1)es.eb(i,i+1);
	}else if(t==1){
		//star
		rep(i,n-1)es.eb(0,i+1);
	}else if(t==2){
		//random
		rng(i,1,n)es.eb(rand_int(i),i);
	}else if(t==3){
		//line and star
		int h=n/2;
		rep(i,h-1)es.eb(i,i+1);
		rng(i,h,n)es.eb(0,i);
	}else if(t==4){
		//line and random
		int h=n/2;
		rep(i,h-1)es.eb(i,i+1);
		rng(i,h,n)es.eb(rand_int(i),i);
	}else if(t==5){
		//star and random
		int h=n/2;
		rep(i,h-1)es.eb(0,i+1);
		rng(i,h,n)es.eb(rand_int(i),i);
	}else if(t==6){
		//binary
		rng(i,1,n)es.eb(i/2,i);
	}else if(t==7){
		//binary and random
		int h=n/2;
		rng(i,1,h)es.eb(i/2,i);
		rng(i,h,n)es.eb(rand_int(i),i);
	}else if(t==8){
		//ladder
		rng(i,1,n){
			if(i%2==0)es.eb(i-2,i);
			else es.eb(i-1,i);
		}
	}else assert(false);
	{
		vi perm=genperm(n);
		for(auto&[a,b]:es){
			a=perm[a];
			b=perm[b];
			//if(a>b)swap(a,b);
			if(rand_int(2))swap(a,b);
		}
		myshuffle(es);
	}
	return es;
}

template<class F> void permrep(int n,F f){
	vi p(n);iota(all(p),0);
	do{
		f(p);
	}while(next_permutation(all(p)));
};

void checkperm(vi p){
	const int n=si(p);
	for(auto v:p)assert(inc(0,v,n-1));
	mkuni(p);
	assert(si(p)==n);
}

//0-s の範囲で distinct に n 個選ぶ
vi rand_vi_distinct(int n,int s){
	set<int> u;
	while(si(u)<n){
		u.insert(rand_int(0,s));
	}
	return vi(all(u));
}

vi rand_vi_distinct(int n,int lw,int up){
	vi res=rand_vi_distinct(n,up-lw);
	for(auto&v:res)v+=lw;
	return res;
}

vi rand_vi_givensum(int n,int s){
	s+=n-1;
	vi pos=rand_vi_distinct(n-1,s-1);
	pos.pb(s);
	int pre=-1;
	vi res(n);
	rep(i,n){
		res[i]=pos[i]-pre-1;
		pre=pos[i];
	}
	return res;
}

vi rand_vi_givensum(int n,int s,int lw){
	assert(s>=n*lw);
	vi res=rand_vi_givensum(n,s-lw*n);
	for(auto&v:res)v+=lw;
	return res;
}

const int nmax=400;
const int vmax=10000;
void Output(vvc<int> c){
	OpenFile();
	int n=si(c);
	assert(inc(2,n,nmax));
	rep(i,n){
		assert(si(c[i])==n);
		rng(j,i,n){
			assert(inc(1,c[i][j],vmax));
		}
	}
	print(n);
	rep(i,n){
		rng(j,i,n){
			print(c[i][j],j==n-1?1:2);
		}
	}
}

vvc<int> genrand(int n,int up=vmax){
	vvc<int> c(n,vi(n));
	rep(i,n)rng(j,i,n)c[i][j]=rand_int(1,up);
	return c;
}

signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	
	SetName("00-sample");
	Output({{1,2},{0,1}});
	Output({{1,1,1},{0,1,1},{0,0,1}});
	Output(genrand(4,10));
	Output(genrand(10));
	
	SetName("01");
	#ifdef LOCAL
	#else
	rep(_,10){
		int n=rand_int(2,nmax);
		Output(genrand(n));
	}
	rep(_,3){
		int n=nmax-_;
		Output(genrand(n));
	}
	#endif
}
