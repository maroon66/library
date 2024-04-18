//O(N+max(A)) なので座標圧縮してから動かしてください
//Verify yosupo (N=500000,string,43ms)
template<class t>vi sais(t a){
	int n=si(a),m=*max_element(all(a))+1;
	vi pos(m+1),x(m),sa(n),val(n),lms;
	for(auto c:a)pos[c+1]++;
	rep(i,m)pos[i+1]+=pos[i];
	vc<bool> s(n);
	per(i,n-1)s[i]=a[i]!=a[i+1]?a[i]<a[i+1]:s[i+1];
	auto ind=[&](const vi&ls){
		fill(all(sa),-1);
		auto L=[&](int i){if(i>=0&&!s[i])sa[x[a[i]]++]=i;};
		auto S=[&](int i){if(i>=0&& s[i])sa[--x[a[i]]]=i;};
		rep(i,m)x[i]=pos[i+1];
		per(i,si(ls))S(ls[i]);
		rep(i,m)x[i]=pos[i];
		L(n-1);
		rep(i,n)L(sa[i]-1);
		rep(i,m)x[i]=pos[i+1];
		per(i,n)S(sa[i]-1);
	};
	auto ok=[&](int i){return i==n||(!s[i-1]&&s[i]);};
	auto same=[&](int i,int j){
		do{
			if(a[i++]!=a[j++])return false;
		}while(!ok(i)&&!ok(j));
		return ok(i)&&ok(j);
	};
	rng(i,1,n)if(ok(i))lms.pb(i);
	ind(lms);
	if(si(lms)){
		int p=-1,w=0;
		for(auto v:sa)if(v&&ok(v)){
			if(p!=-1&&same(p,v))w--;
			val[p=v]=w++;
		}
		vi b=lms;
		for(auto&v:b)v=val[v];
		b=sais(b);
		for(auto&v:b)v=lms[v];
		ind(b);
	}
	return sa;
}

struct SA{
	int n;
	vi sa,as,lcp;
	template<class t> SA(t s):n(si(s)),as(n),lcp(n-1){
		{//座標圧縮,値域が小さいなら skip 可
			auto vs=s;mkuni(vs);
			for(auto&v:s)v=lwb(vs,v);
		}
		sa=sais(s);
		//as
		rep(i,n)as[sa[i]]=i;
		//lcp
		int w=0;
		for(auto i:as){
			if(w)w--;
			if(i<n-1){
				while(max(sa[i],sa[i+1])+w<n&&s[sa[i]+w]==s[sa[i+1]+w]) w++;
				lcp[i]=w;
			}
		}
	}
};

//Xmas2012B
template<class S>
struct string_cmp{
	S s;
	int n;
	SA sa;
	minst t;
	string_cmp(const S&ss):s(ss),n(si(s)),sa(s),t(sa.lcp,imin,inf){}
	int getlcp(int i,int j){
		if(i==n||j==n)return 0;
		if(i==j)return n-i;
		i=sa.as[i];
		j=sa.as[j];
		if(i>j)swap(i,j);
		return t.get(i,j);
	}
	int cmpchar(int i,int j){
		assert(0<=i&&i<n);
		assert(0<=j&&j<n);
		assert(s[i]!=s[j]);
		return s[i]<s[j]?-1:1;
	}
	//[a,b)<[c,d)-> -1
	//same -> 0
	//[a,b)>[c,d) -> 1
	int cmp(int a,int b,int c,int d){
		int len=min({getlcp(a,c),b-a,d-c});
		if(a+len==b&&c+len==d)return 0;
		if(a+len==b)return -1;
		if(c+len==d)return 1;
		return cmpchar(a+len,c+len);
	}
	//ABC240H
	int cmp(pi a,pi b){
		return cmp(a.a,a.b,b.a,b.b);
	}
	int cmp_samelen(int a,int b,int len){
		return cmp(a,a+len,b,b+len);
	}
	//[l,r) のリストを受け取り，それらを連結してできる文字列の比較をする
	//リストの長さ 2 の場合は verify (Xmas2012 B)
	int cmp_list(vc<pi> a,vc<pi> b){
		int i=0,j=0;
		while(1){
			while(i<si(a)&&a[i].a==a[i].b)i++;
			while(j<si(b)&&b[j].a==b[j].b)j++;
			if(i==si(a)&&j==si(b))return 0;
			if(i==si(a))return -1;
			if(j==si(b))return 1;
			int k=min(a[i].b-a[i].a,b[j].b-b[j].a);
			int x=cmp_samelen(a[i].a,b[j].a,k);
			if(x)return x;
			a[i].a+=k;
			b[j].a+=k;
		}
		assert(0);
	}
	//[l,r) のリストを受け取り，それらを連結してできる文字列の LCP を求める
	//UCUP 2-17-H
	int lcp_list(vc<pi> a,vc<pi> b){
		int i=0,j=0,res=0;
		while(1){
			while(i<si(a)&&a[i].a==a[i].b)i++;
			while(j<si(b)&&b[j].a==b[j].b)j++;
			if(i==si(a)||j==si(b))return res;
			int k=min(a[i].b-a[i].a,b[j].b-b[j].a);
			int x=getlcp(a[i].a,b[j].a);
			if(x<k)return res+x;
			res+=k;
			a[i].a+=k;
			b[j].a+=k;
		}
		assert(0);
	}
	//UCUP 2-24-E
	//s[i,i+len) = s[j,j+len) が一致するような j の範囲
	//範囲というのは SA 上での範囲
	//TLE したので segtree で代用 (UCUP 2-24-E)
	pi common_range(int i,int len){
		assert(inc(0,i,n-1));
		i=sa.as[i];
		int l=find_min_true(-1,i,[&](int j){
			return t.get(j,i)>=len;
		});
		int r=find_max_true(i,n,[&](int j){
			return t.get(i,j)>=len;
		})+1;
		return pi(l,r);
	}
};

//res[i]=start index of the lex-min suffix of a[:i+1]
vi lexmin_suffix(const vi&a){
	int n=si(a);
	string_cmp<vi> x(a);
	int val=inf,idx=-1;
	vi len(n),res(n);
	priority_queue<int> pq;
	rep(i,n){
		if(chmin(val,x.sa.as[i]))idx=i;
		len[i]=x.getlcp(idx,i);
		pq.push(i);
		while(1){
			int j=pq.top();
			if(i<j+len[j]){
				res[i]=j;
				break;
			}else{
				pq.pop();
			}
		}
	}
	return res;
}

//2020 Multi-Uni Contest Day8 H
int common_substrings(vc<string> ss){
	mkuni(ss);
	int n=si(ss);
	
	string tot;
	vi idx;
	rep(i,n){
		tot+=ss[i];
		rep(_,si(ss[i]))idx.pb(i);
		tot+='$';
		idx.pb(-1);
	}
	
	SA sa(tot);
	minst st(sa.lcp,imin,inf);
	
	vi cnt(n);
	int zs=n;
	auto add=[&](int i){
		i=idx[sa.sa[i]];if(i==-1)return;
		if(cnt[i]==0)zs--;
		cnt[i]++;
	};
	auto del=[&](int i){
		i=idx[sa.sa[i]];if(i==-1)return;
		cnt[i]--;
		if(cnt[i]==0)zs++;
	};
	
	int ans=0;
	int j=0;
	rep(i,si(tot)){
		while(j<si(tot)&&zs)add(j++);
		if(zs)break;
		ans+=st.get(i,j);
		if(i)ans-=st.get(i-1,j);
		del(i);
	}
	
	return ans;
}

//CF530F
//returns the suffix tree
//the i-th (0<=i<=n-1) element corresponds to the lexicographically i-th suffix
struct ste{
	int to,l,r;
	operator int()const{return to;}
};
ostream&operator<<(ostream&os,const ste&a){
	return os<<"ste{"<<a.to<<","<<a.l<<" "<<a.r<<"}";
}
pair<vvc<ste>,int> stree(const SA&sa){
	int n=sa.n,s=n*2-1;
	vi len(s);
	rep(i,n)len[i]=n-sa.sa[i];
	rep(i,n-1)len[i+n]=sa.lcp[i];
	vi idx(n-1);
	iota(all(idx),n);
	stable_sort(all(idx),[&](int a,int b){return len[a]>len[b];});
	unionfind uf(s);
	vi par(s,-1),c(s,-1);
	rep(i,n)c[i]=sa.sa[i];
	for(auto i:idx){
		int a=uf.find(i-n),b=uf.find(i-n+1);
		uf.unite(i,a);
		uf.unite(i,b);
		par[a]=i;
		par[b]=i;
		c[i]=c[a];
	}
	par[n==1?0:idx.back()]=s++;
	len.pb(0);
	uf=unionfind(s);
	rep(i,s-1)if(len[i]==len[par[i]])
		uf.unite(i,par[i]);
	vi vs;
	rep(i,s)if(uf.find(i)==i)vs.pb(i);
	auto z=[&](int i){return lower_bound(all(vs),uf.find(i))-vs.bg;};
	vvc<ste> res(vs.size());
	rep(ii,s-1){
		int i=ii/2+ii%2*n;
		int a=z(i),b=z(par[i]);
		if(a==b)continue;
		res[b].pb(ste{a,c[i]+len[par[i]],c[i]+len[i]});
	}
	return make_pair(res,z(s-1));
}
