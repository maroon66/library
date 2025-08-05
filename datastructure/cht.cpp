struct ln{
	ll a,b;
	ll eval(ll x)const{return a*x+b;}
};
ostream&operator<<(ostream&os,const ln&v){
	return os<<"L{"<<v.a<<","<<v.b<<"}";
}
//線分，クエリ が単調であるという仮定の元線形
//単調であれば増加なのか減少なのかは自動で判定してくれる
//https://www.codechef.com/problems/OUTOFBRACKET?tab=statement
//stress-tested
struct autocht{
	bool ismax;
	int lineinc=-1,queryinc=-1,head=0;
	//0 inc, 1 dec
	autocht(bool ismax_):ismax(ismax_){}
	//multiuni2025-1-D
	void init(bool ismax_){
		ismax=ismax_;
		lineinc=-1;
		queryinc=-1;
		head=0;
		ls.clear();
		prex=LLONG_MIN,preres=LLONG_MIN;
	}
	ll fdiv(ll a,ll b){
		return a/b-((a^b)<0&&a%b);
	}
	ll pos(ln x,ln y){
		return fdiv(x.b-y.b,y.a-x.a);
	}
	vc<ln> ls;
	void add(ll a,ll b){
		if(!ismax){
			a=-a;
			b=-b;
		}
		if(!lineinc){
			a=-a;
		}
		if(si(ls)&&ls.back().a==a){
			chmax(b,ls.back().b);
			ls.pop_back();
		}
		if(lineinc==-1&&si(ls)){
			lineinc=ls.back().a<a;
			if(!lineinc){
				for(auto&v:ls)v.a=-v.a;
				a=-a;
				if(prex!=LLONG_MIN)prex=-prex;
				if(queryinc!=-1)queryinc^=1;
			}
		}
		ln cur{a,b};
		if(si(ls))assert(ls.back().a<a);
		for(int s=si(ls);s>=2;ls.pop_back(),s--){
			ll x=pos(ls[s-2],ls[s-1]);
			ll y=pos(ls[s-1],cur);
			if(x<y)break;
		}
		chmin(head,si(ls));
		ls.pb(cur);
		if(prex!=LLONG_MIN)chmax(preres,cur.eval(prex));
	}
	ll prex=LLONG_MIN,preres=LLONG_MIN;
	ll query_sub(ll x){
		if(prex==LLONG_MIN){
			ll res=-infLL;
			for(const auto&v:ls)chmax(res,v.eval(x));
			return res;
		}else if(prex==x){
			return preres;
		}else{
			if(queryinc==-1){
				queryinc=prex<x;
			}
			if(queryinc==0){
				assert(prex>x);
				assert(head==0);
				if(ls.empty())return -infLL;
				for(int s=si(ls);s>=2;ls.pop_back(),s--){
					if(ls[s-2].eval(x)<ls[s-1].eval(x))break;
				}
				return ls.back().eval(x);
			}else if(queryinc==1){
				assert(prex<x);
				if(ls.empty())return -infLL;
				while(head+1<si(ls)&&ls[head].eval(x)<=ls[head+1].eval(x))
					head++;
				return ls[head].eval(x);
			}else assert(false);
		}
	}
	ll query(ll x){
		if(!lineinc)x=-x;
		ll res=query_sub(x);
		prex=x;
		preres=res;
		if(!ismax)res=-res;
		return res;
	}
};
