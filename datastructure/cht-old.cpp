int fdiv(int a,int b){
	return a/b-((a^b)<0&&a%b);
}

struct CHT{
	struct ln{
		int a,b;
		int eval(int x){return a*x+b;}
	};
	int pos(ln x,ln y){
		return fdiv(x.b-y.b,y.a-x.a);
	}
	vc<ln> ls;
	//a nonincreasing
	void add(int a,int b){
		if(ls.size()&&ls.back().a==a){
			chmin(b,ls.back().b);
			ls.pop_back();
		}
		int s;
		while((s=ls.size())>=2){
			int x=pos(ls[s-2],ls[s-1]);
			int y=pos(ls[s-1],ln{a,b});
			if(x<y)break;
			ls.pop_back();
		}
		ls.pb(ln{a,b});
	}
	//x nonincreasing
	//return min
	int query(int x){
		if(ls.empty())return inf;
		int s;
		while((s=ls.size())>=2)
			if(ls[s-2].eval(x)>ls[s-1].eval(x))
				break;
			else
				ls.pop_back();
		return ls.back().eval(x);
	}
};

//KUPC2019L
struct Line{
	int a,b;
	int Eval(int x){
		return a*x+b;
	}
};
ostream&operator<<(ostream&os,const Line&ln){
	return os<<"L{"<<ln.a<<","<<ln.b<<"}";
}
struct ConvexHull{
	static int div(int a,int b){
		return a / b - ((a ^ b) < 0 && a % b);
	}
	//cross(a,b)<cross(b,c)
	static bool cmpline(const Line&a,const Line&b,const Line&c){
		int ay=a.b-b.b,ax=b.a-a.a;
		int by=b.b-c.b,bx=c.a-b.a;
		return div(ay,ax)<div(by,bx);
	}
	vector<Line> ls;
	int head;
	ConvexHull():head(0){}
	//z.a:non-decreasing
	//max-hull
	void push_back(Line z){
		if(!ls.empty()&&ls.back().a==z.a){
			chmax(z.b,ls.back().b);
			ls.pop_back();
		}
		while(ls.size()>=2){
			int s=ls.size();
			if(cmpline(ls[s-2],ls[s-1],z))
				break;
			ls.pop_back();
		}
		chmin(head,int(ls.size()));
		ls.push_back(z);
	}
	//x:non-decreasing
	/*int get(int x){
		while(head+1<int(ls.size())){
			if(ls[head].Eval(x)>ls[head+1].Eval(x))
				break;
			head++;
		}
		return ls[head].Eval(x);
	}*/
	//arbitrary x
	int get(int x){
		int lw=0,up=ls.size();
		while(up-lw>1){
			const int mid=(lw+up)/2;
			if(ls[mid-1].Eval(x)<ls[mid].Eval(x))
				lw=mid;
			else
				up=mid;
		}
		return ls[lw].Eval(x);
	}
};

//TROC #26 G (int128 なしで動くらしい)
//XIX Opencup GP of Zhejiang I
//使ったステップ数の情報を持つ convex hull
//F(a,b)=true なら a を優先
//min-hull なら less をいれればいい
//eval の値が 10^18 オーダーならだいたいうまく行くようになっているはず
struct Line{
	int a,b,c,d;
	int eval(int x){
		return a*x+b;
	}
};
ostream&operator<<(ostream&os,const Line&ln){
	return os<<"L{"<<ln.a<<","<<ln.b<<","<<ln.c<<","<<ln.d<<"}";
}
template<class F>
struct convexhull_minmax{
	static int fdiv(int a,int b){
		return a / b - ((a ^ b) < 0 && a % b);
	}
	static int cdiv(int a,int b){
		return a / b + ((a ^ b) > 0 && a % b);
	}
	//can b be the opt?
	static bool cmpline(const Line&a,const Line&b,const Line&c){
		int ay=a.b-b.b,ax=b.a-a.a;
		int by=b.b-c.b,bx=c.a-b.a;
		return cdiv(ay,ax)<=fdiv(by,bx);
	}
	vector<Line> ls;
	int head;
	convexhull_minmax():head(0){}
	//min-hull -> z.a is non-increasing
	void push_back(Line z){
		if(si(ls)&&ls.back().a==z.a){
			auto&w=ls.back();
			if(F()(w.b,z.b)){
				return;
			}else if(F()(z.b,w.b)){
				ls.pop_back();
			}else{
				chmin(w.c,z.c);
				chmax(w.d,z.d);
				return;
			}
		}
		while(si(ls)>=2){
			int s=si(ls);
			if(cmpline(ls[s-2],ls[s-1],z))
				break;
			ls.pop_back();
		}
		chmin(head,si(ls));
		ls.push_back(z);
	}
	//x is non-decreasing
	tuple<int,int,int> get(int x){
		while(head+1<si(ls)){
			if(F()(ls[head+1].eval(x),ls[head].eval(x)))
				head++;
			else
				break;
		}
		int res=ls[head].eval(x),c=ls[head].c,d=ls[head].d;
		int cur=head;
		while(cur+1<si(ls)&&ls[cur+1].eval(x)==res){
			cur++;
			chmin(c,ls[cur].c);
			chmax(d,ls[cur].d);
		}
		if(head<cur){
			head=cur-1;
			ls[head].c=c;
			ls[head].d=d;
		}
		return mt(res,c,d);
	}
};

//TROC #26 G (int128 なしで動くらしい)
//a,b of length N
//a[0]>=a[1]>=a[2]...
//c of length N+1
//cost(i,j)=vs[i]*b[i:j]+L
//あと k を踏む度に c[k] のコスト
//(min,minsteps,maxsteps)
tuple<int,int,int> do_cht_dp(const vi&a,const vi&b,const vi&c,int L){
	int n=si(a);
	rep(i,n-1)assert(a[i]>=a[i+1]);
	assert(si(b)==n);
	rep(i,n)assert(b[i]>=0);
	assert(si(c)==n+1);
	convexhull_minmax<less<int>> z;
	using T=tuple<int,int,int>;
	vc<T> dp(n+1);
	int bsum=0;
	rep(i,n+1){
		if(i==0){
			dp[i]=T(c[i],0,0);
		}else{
			bsum+=b[i-1];
			auto [v,l,r]=z.get(bsum);
			dp[i]=T(v+L+c[i],l+1,r+1);
		}
		if(i<n){
			auto [v,l,r]=dp[i];
			z.pb(Line{a[i],v-a[i]*bsum,l,r});
		}
	}
	return dp[n];
}

//TROC #26 G (int128 なしで動くらしい)
//上記と同様
int kstep_cht_dp(const vi&a,const vi&b,const vi&c,int k){
	int n=si(a);
	assert(inc(1,k,n));
	int lw=-ten(18),up=ten(18);
	while(up-lw>1){
		int mid=(lw+up)/2;
		auto [v,l,r]=do_cht_dp(a,b,c,mid);
		if(inc(l,k,r))return v-k*mid;
		if(k<l)lw=mid;
		else up=mid;
	}
	assert(false);
}
