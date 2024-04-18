//yosupo
//ARC 055 C
//res[0]=n,res[n]=0
template<class S>
vi zalgo(const S&s){
	int n=s.size();
	vi res(n+1,0);
	int j=0;
	rng(i,1,n)
		if(j+res[j]>i+res[i-j])
			res[i]=res[i-j];
		else{
			res[i]=max(int(0),j+res[j]-i);
			while(i+res[i]<n&&s[res[i]]==s[i+res[i]])
				res[i]++;
			j=i;
		}
	res[0]=n;
	return res;
}

//yosupo
//MW 2019 Day2 J
template<class S>
struct EnumRun{
	const S&s;
	int n;
	vvc<pi> run;
	vc<tuple<int,int,int>> sub(const S&x,const S&y){
		S p=x;reverse(all(p));
		S q=y;
		q.insert(q.end(),all(x));
		q.insert(q.end(),all(y));
		vi zp=zalgo(p),zq=zalgo(q);
		vc<tuple<int,int,int>> res;
		rep(i,x.size()){
			int a=x.size()-i;
			int b=i-zp[a];
			int c=max(int(0),int(y.size())-zq[y.size()+i]);
			if(int(x.size()+y.size())-b-c>=2*a)
				res.eb(a,b,c);
		}
		return res;
	}
	void rec(int l,int r){
		if(r-l<=1)return;
		int m=(l+r)/2;
		S x(s.bg+l,s.bg+m),y(s.bg+m,s.bg+r);
		{
			auto z=sub(x,y);
			for(auto w:z){
				int a,b,c;
				tie(a,b,c)=w;
				run[a].eb(l+b,r-c);
			}
		}
		reverse(all(x));
		reverse(all(y));
		{
			auto z=sub(y,x);
			for(auto w:z){
				int a,b,c;
				tie(a,b,c)=w;
				run[a].eb(l+c,r-b);
			}
		}
		rec(l,m);
		rec(m,r);
	}
	EnumRun(const S&ss):s(ss),n(s.size()),run(n+1){
		rec(0,n);
		rng(i,1,n+1){
			sort(all(run[i]),[&](pi a,pi b){
				return pi(a.a,-a.b)<pi(b.a,-b.b);
			});
			int mx=-1;
			vc<pi> tmp;
			for(auto p:run[i])
				if(mx<p.b){
					tmp.pb(p);
					mx=p.b;
				}
			run[i]=tmp;
		}
	}
};

template<class S>
vvc<pi> enum_run(const S&s){
	return EnumRun<S>(s).run;
}

//XIX Opencup GP of Zhejiang D
template<class S>
vvc<pi> enum_run_del_duplicate(const S&s){
	auto res=enum_run(s);
	vc<pi> vals;
	rep(i,si(res))vals.insert(vals.ed,all(res[i]));
	mkuni(vals);
	vc<bool> used(si(vals));
	rep(i,si(res)){
		int k=0;
		rep(j,si(res[i])){
			int x=lwb(vals,res[i][j]);
			if(used[x])continue;
			used[x]=1;
			res[i][k++]=res[i][j];
		}
		res[i].resize(k);
	}
	return res;
}

//maintain the set of indices i>0 such that s[i:] coincides with the prefix of s.
//append returns the pair of
//newly added element to the set
//and
//removed elements

//CF612 E
struct online_zalgo{
	vi s;
	vvc<int> del;
	set<int> pos;
	pair<int,vi> append(int c){
		int i=s.size();
		del.eb();
		s.pb(c);
		for(auto itr=pos.bg;itr!=pos.ed;itr=pos.erase(itr)){
			int j=*itr;
			if(s[i-j]==s[i])break;
			del[i].pb(j);
		}
		if(pos.size()){
			int off=*pos.bg;
			for(auto k:del[i-off]){
				auto itr=pos.find(k+off);
				assert(itr!=pos.ed);
				del[i].pb(*itr);
				pos.erase(itr);
			}
		}
		int ad=-1;
		if(i&&s[0]==s[i]){
			pos.insert(i);
			ad=i;
		}
		return mp(ad,del[i]);
	}
};
