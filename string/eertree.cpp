//CF 463 G
//Yukicoder No.263
//Codechef 2022 March Cookoff PALQUE (l,r) が r 最小を取るようになった
template<class S>
struct eertree{
	struct N{
		int suf,ser,l,r;
		int len(){return r-l;}
		map<int,int> ch;
	};
	vc<N> x;
	int dist(int v){
		if(x[v].suf<=0)return -1;
		return x[v].len()-x[x[v].suf].len();
	}
	int c;
	S s;
	eertree(){
		x.pb({-1,-1,1,0,{}});
		x.pb({0,-1,0,0,{}});
		c=1;
	}
	vi pos;
	eertree(const S&ini):eertree(){
		pos.pb(c);
		for(auto v:ini)pos.pb(extend(v));
	}
	bool chk(int v,int i){
		int l=i-x[v].len();
		return l>0&&s[l-1]==s[i];
	}
	template<class t>
	int extend(t z){
		int i=si(s);
		s.pb(z);
		
		while(!chk(c,i))c=x[c].suf;
		if(x[c].ch.count(s[i])==0){
			int d=x[c].suf;
			if(d!=-1)while(!chk(d,i))d=x[d].suf;
			int e=d==-1?1:x[d].ch[s[i]];
			int f=x[c].ch[s[i]]=x.size();
			x.pb({e,e,i-x[c].len()-1,i+1,{}});
			if(dist(f)==dist(e))
				x[f].ser=x[e].ser;
		}
		return c=x[c].ch[s[i]];
	}
	N& operator[](int i){
		return x[i];
	}
};

//文字列をpalindromicなsubstringに分解する方法を数える例
signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	
	vi s=readinput();
	int n=si(s);
	
	vc<mint> dp(n+1);
	dp[0]=1;
	
	vc<mint> buf(n*2+2);
	
	eertree<vi> t;
	
	rep(i,n){
		int v=t.extend(s[i]);
		while(t[v].len()){
			buf[v]=dp[i+1-t[t[v].ser].len()-t.dist(v)];
			if(t[v].suf!=t[v].ser)
				buf[v]+=buf[t[v].suf];
			dp[i+1]+=buf[v];
			v=t[v].ser;
		}
	}
	
	cout<<dp[n]<<endl;
}

//https://codeforces.com/gym/103698/problem/H
template<class C>
struct eertree{
	struct N{
		int suf,ser,len;
		rhash::P h;
		map<C,int> ch;
	};
	vc<N> x;
	vi ls;
	vc<C> s;
	eertree(){
		x.pb({-1,-1,-1,rhash::P(),{}});
		x.pb({0,-1,0,rhash::P(),{}});
		ls.pb(1);
	}
	int dist(int v){
		if(x[v].suf<=0)return -1;
		return x[v].len-x[x[v].suf].len;
	}
	bool chk(int v,int i){
		int l=i-x[v].len;
		return l>0&&s[l-1]==s[i];
	}
	void reduce(int&c,int i){
		while(!chk(c,i)){
			int d=x[c].suf;
			if(chk(d,i)){
				c=d;
				return;
			}
			c=x[c].ser;
		}
	}
	void extend(C z){
		int i=si(s);
		s.pb(z);
		
		int c=ls.back();
		reduce(c,i);
		if(x[c].ch.count(s[i])==0){
			int d=x[c].suf;
			if(d!=-1)reduce(d,i);
			int e=d==-1?1:x[d].ch[s[i]];
			int f=x[c].ch[s[i]]=si(x);
			auto tmp=rhash::single(z);
			x.pb({e,e,x[c].len+2,c==0?tmp:rhash::mrg(tmp,x[c].h,tmp),{}});
			if(dist(f)==dist(e))
				x[f].ser=x[e].ser;
		}
		ls.pb(c=x[c].ch[s[i]]);
	}
	void pop(){
		ls.pop_back();
		s.pop_back();
	}
	int gettop(){
		return ls.back();
	}
	N& operator[](int i){
		return x[i];
	}
};
