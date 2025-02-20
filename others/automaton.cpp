template<int K>
struct automaton{
	using A=array<int,K>;
	int root;
	vc<A> to;
	vc<bool> accept;
	int size()const{return si(to);}
	void resize(int n){
		root=-1;
		to.resize(n);
		rep(i,n)rep(j,K)to[i][j]=-1;
		accept.assign(n,0);
	}
	friend ostream& operator<<(ostream&os,const automaton&a){
		return os<<"{"<<a.root<<","<<a.to<<","<<a.accept<<"}";
	}
	void show()const{
		cerr<<"size: "<<size()<<endl;
		cerr<<"root: "<<root<<endl;
		rep(i,size()){
			cerr<<accept[i]<<" "<<to[i]<<endl;
		}
	}
	//unreachable state の処理してねえ
	static automaton minimum(const automaton&a){
		int n=si(a);
		vi col(n);
		rep(i,n)col[i]=a.accept[i];
		while(1){
			using B=array<int,K+1>;
			vc<B> z(n);
			rep(i,n){
				z[i][0]=col[i];
				rep(j,K)z[i][j+1]=col[a.to[i][j]];
			}
			auto val=z;
			mkuni(val);
			vi tmp(n);
			rep(i,n)tmp[i]=lwb(val,z[i]);
			if(col==tmp)break;
			col=tmp;
		}
		int s=*max_element(all(col))+1;
		automaton res;
		res.resize(s);
		rep(i,n){
			int c=col[i];
			if(res.to[c][0]==-1){
				rep(j,K){
					int d=col[a.to[i][j]];
					res.to[c][j]=d;
				}
				res.accept[c]=a.accept[i];
			}
		}
		res.root=col[a.root];
		return res;
	}
	//GCJ2016WF B
	//parallel=0 直列につないでいる
	//parallel=1 並列につないでおり，どちらか一方を受理すれば良い
	//parallel=2 並列につないでおり，両方を受理する必要がある
	static automaton combine(const automaton&a,const automaton&b,int parallel){
		assert(inc(0,parallel,2));
		if(si(a)==0)return b;
		if(si(b)==0)return a;
		int n=si(a),m=si(b);
		map<vc<bool>,int> idx;
		vc<vc<bool>> q;
		vc<A> to;
		vc<bool> accept;
		int head=0;
		
		auto getidx=[&](vc<bool> x){
			if(!parallel)rep(i,n)if(x[i]&&a.accept[i])x[n+b.root]=1;
			
			auto itr=idx.find(x);
			if(itr!=idx.ed)return itr->b;
			
			int res=si(q);
			q.pb(x);
			to.eb();
			rep(i,K)to[res][i]=-1;
			
			bool oka=false,okb=false;
			rep(i,n)if(x[i]&&a.accept[i])oka=true;
			rep(i,m)if(x[n+i]&&b.accept[i])okb=true;
			if(parallel==0)accept.pb(okb);
			else if(parallel==1)accept.pb(oka||okb);
			else accept.pb(oka&&okb);
			
			return idx[x]=res;
		};
		{
			vc<bool> s(n+m);
			s[a.root]=1;
			if(parallel)s[n+b.root]=1;
			getidx(s);
		}
		while(head<si(q)){
			auto cur=q[head++];
			int curId=idx[cur];
			assert(curId==head-1);
			rep(nx,K){
				vc<bool> tmp(n+m);
				rep(i,n)if(cur[i])tmp[a.to[i][nx]]=1;
				rep(i,m)if(cur[n+i])tmp[n+b.to[i][nx]]=1;
				int nxId=getidx(tmp);
				to[curId][nx]=nxId;
			}
		}
		return minimum({0,to,accept});
	}
	//GCJ2016WF B
	//a+を返す
	//1個以上の繰り返し
	static automaton repeat1(const automaton&a){
		int n=si(a);
		map<vc<bool>,int> idx;
		vc<vc<bool>> q;
		vc<A> to;
		vc<bool> accept;
		int head=0;
		
		auto getidx=[&](vc<bool> x){
			rep(i,n)if(x[i]&&a.accept[i])x[a.root]=1;
			
			auto itr=idx.find(x);
			if(itr!=idx.ed)return itr->b;
			
			int res=si(q);
			q.pb(x);
			to.eb();
			rep(i,K)to[res][i]=-1;
			
			bool ok=false;
			rep(i,n)if(x[i]&&a.accept[i])ok=true;
			accept.pb(ok);
			
			return idx[x]=res;
		};
		{
			vc<bool> s(n);
			s[a.root]=1;
			getidx(s);
		}
		while(head<si(q)){
			auto cur=q[head++];
			int curId=idx[cur];
			assert(curId==head-1);
			rep(nx,K){
				vc<bool> tmp(n);
				rep(i,n)if(cur[i])tmp[a.to[i][nx]]=1;
				int nxId=getidx(tmp);
				to[curId][nx]=nxId;
			}
		}
		return minimum({0,to,accept});
	}
	//GCJ2016WF B
	static automaton onlyempty(){
		automaton res;
		res.resize(2);
		res.root=0;
		rep(i,2)rep(j,K)res.to[i][j]=1;
		res.accept[0]=true;
		return res;
	}
	//GCJ2016WF B
	//a*を返す
	//0個以上の繰り返し
	static automaton repeat0(const automaton&a){
		return combine(repeat1(a),onlyempty(),true);
	}
	bool operator==(const automaton&rhs)const{
		return root==rhs.root&&to==rhs.to&&accept==rhs.accept;
	}
	bool evaluate(vi x)const{
		int cur=root;
		for(auto i:x)cur=to[cur][i];
		return accept[cur];
	}
	//GCJ2016WF B ただし -1 のケースは調べていない
	//valid なものを数える
	//minimize されている前提で動く
	//答えが有限でないともちろん壊れる
	int count_valid()const{
		//minimize済み && 答えが有限 => どこかに吸収されている
		int n=size(),f=-1;
		rep(i,n)if(!accept[i]){
			bool ok=true;
			rep(j,K)if(to[i][j]!=i)ok=false;
			if(ok){
				assert(f==-1);
				f=i;
			}
		}
		if(f==-1)return -1;
		vi dp(n),cnt(n);
		dp[root]=1;
		rep(i,n)rep(j,K)cnt[to[i][j]]++;
		vi q;
		rep(i,n)if(cnt[i]==0)q.pb(i);
		int ans=0;
		rep(head,si(q)){
			int i=q[head];
			if(accept[i])ans+=dp[i];
			rep(j,K){
				int v=to[i][j];
				dp[v]+=dp[i];
				if(--cnt[v]==0)
					q.pb(v);
			}
		}
		if(si(q)!=n-1)return -1;
		return ans;
	}
};

//GCJ2016WF B
using N=automaton<10>;

//1以上lim未満の数を受け取るオートマトン
//0は入ってない
N lessthan(int lim){
	string s=to_string(lim);
	int n=si(s);
	N res;res.resize(2*n+1);
	res.root=0;
	auto ae=[&](int a,int k,int b){
		assert(a<2*n+1);
		if(b==2*n+1)b=n;
		res.to[a][k]=b;
	};
	rep(i,n){
		int v=s[i]-'0';
		rep(j,v)ae(i,j,i+n+1);
		ae(i,v,i+1);
		rng(j,v+1,10)ae(i,j,i+1+n+1);
	}
	ae(0,0,n);
	rep(j,10)ae(n,j,n);
	rng(i,n+1,2*n+1)rep(j,10)ae(i,j,i+1);
	
	rng(i,1,n)res.accept[i]=true;
	rng(i,n+1,2*n+1)res.accept[i]=true;
	return res;
}

N onedigit(int v){
	N res;res.resize(3);
	res.root=0;
	rep(i,3)rep(j,10)res.to[i][j]=2;
	res.to[0][v]=1;
	res.accept[1]=true;
	return res;
}

//Not Verified
template<int K>
struct DFA{
	using A=array<int,K>;
	int root;
	vc<A> to;
	vc<bool> accept;
	int size()const{return si(to);}
	void resize(int n){
		root=-1;
		to.resize(n);
		rep(i,n)rep(j,K)to[i][j]=-1;
		accept.assign(n,0);
	}
	friend ostream& operator<<(ostream&os,const DFA&a){
		return os<<"{"<<a.root<<","<<a.to<<","<<a.accept<<"}";
	}
	void show()const{
		cerr<<"size: "<<size()<<endl;
		cerr<<"root: "<<root<<endl;
		rep(i,size()){
			cerr<<accept[i]<<" "<<to[i]<<endl;
		}
	}
	//unreachable state の処理してねえ
	static DFA minimum(const DFA&a){
		int n=si(a);
		vi col(n);
		rep(i,n)col[i]=a.accept[i];
		while(1){
			using B=array<int,K+1>;
			vc<B> z(n);
			rep(i,n){
				z[i][0]=col[i];
				rep(j,K)z[i][j+1]=col[a.to[i][j]];
			}
			auto val=z;
			mkuni(val);
			vi tmp(n);
			rep(i,n)tmp[i]=lwb(val,z[i]);
			if(col==tmp)break;
			col=tmp;
		}
		int s=*max_element(all(col))+1;
		DFA res;
		res.resize(s);
		rep(i,n){
			int c=col[i];
			if(res.to[c][0]==-1){
				rep(j,K){
					int d=col[a.to[i][j]];
					res.to[c][j]=d;
				}
				res.accept[c]=a.accept[i];
			}
		}
		res.root=col[a.root];
		return res;
	}
	//GCJ2016WF B
	//parallel=0 直列につないでいる
	//parallel=1 並列につないでおり，どちらか一方を受理すれば良い
	//parallel=2 並列につないでおり，両方を受理する必要がある
	static DFA combine(const DFA&a,const DFA&b,int parallel){
		assert(inc(0,parallel,2));
		if(si(a)==0)return b;
		if(si(b)==0)return a;
		int n=si(a),m=si(b);
		map<vc<bool>,int> idx;
		vc<vc<bool>> q;
		vc<A> to;
		vc<bool> accept;
		int head=0;
		
		auto getidx=[&](vc<bool> x){
			if(!parallel)rep(i,n)if(x[i]&&a.accept[i])x[n+b.root]=1;
			
			auto itr=idx.find(x);
			if(itr!=idx.ed)return itr->b;
			
			int res=si(q);
			q.pb(x);
			to.eb();
			rep(i,K)to[res][i]=-1;
			
			bool oka=false,okb=false;
			rep(i,n)if(x[i]&&a.accept[i])oka=true;
			rep(i,m)if(x[n+i]&&b.accept[i])okb=true;
			if(parallel==0)accept.pb(okb);
			else if(parallel==1)accept.pb(oka||okb);
			else accept.pb(oka&&okb);
			
			return idx[x]=res;
		};
		{
			vc<bool> s(n+m);
			s[a.root]=1;
			if(parallel)s[n+b.root]=1;
			getidx(s);
		}
		while(head<si(q)){
			auto cur=q[head++];
			int curId=idx[cur];
			assert(curId==head-1);
			rep(nx,K){
				vc<bool> tmp(n+m);
				rep(i,n)if(cur[i])tmp[a.to[i][nx]]=1;
				rep(i,m)if(cur[n+i])tmp[n+b.to[i][nx]]=1;
				int nxId=getidx(tmp);
				to[curId][nx]=nxId;
			}
		}
		return minimum({0,to,accept});
	}
	//GCJ2016WF B
	//a+を返す
	//1個以上の繰り返し
	static DFA repeat1(const DFA&a){
		int n=si(a);
		map<vc<bool>,int> idx;
		vc<vc<bool>> q;
		vc<A> to;
		vc<bool> accept;
		int head=0;
		
		auto getidx=[&](vc<bool> x){
			rep(i,n)if(x[i]&&a.accept[i])x[a.root]=1;
			
			auto itr=idx.find(x);
			if(itr!=idx.ed)return itr->b;
			
			int res=si(q);
			q.pb(x);
			to.eb();
			rep(i,K)to[res][i]=-1;
			
			bool ok=false;
			rep(i,n)if(x[i]&&a.accept[i])ok=true;
			accept.pb(ok);
			
			return idx[x]=res;
		};
		{
			vc<bool> s(n);
			s[a.root]=1;
			getidx(s);
		}
		while(head<si(q)){
			auto cur=q[head++];
			int curId=idx[cur];
			assert(curId==head-1);
			rep(nx,K){
				vc<bool> tmp(n);
				rep(i,n)if(cur[i])tmp[a.to[i][nx]]=1;
				int nxId=getidx(tmp);
				to[curId][nx]=nxId;
			}
		}
		return minimum({0,to,accept});
	}
	//GCJ2016WF B
	static DFA onlyempty(){
		DFA res;
		res.resize(2);
		res.root=0;
		rep(i,2)rep(j,K)res.to[i][j]=1;
		res.accept[0]=true;
		return res;
	}
	static DFA onlyX(int x){
		DFA res;
		res.resize(2);
		res.root=0;
		rep(i,2)rep(j,K)res.to[i][j]=1;
		res.to[0][x]=0;
		res.accept[0]=true;
		return res;
	}
	//GCJ2016WF B
	//a*を返す
	//0個以上の繰り返し
	static DFA repeat0(const DFA&a){
		return combine(repeat1(a),onlyempty(),true);
	}
	bool operator==(const DFA&rhs)const{
		return root==rhs.root&&to==rhs.to&&accept==rhs.accept;
	}
	bool evaluate(vi x)const{
		int cur=root;
		for(auto i:x)cur=to[cur][i];
		return accept[cur];
	}
	//GCJ2016WF B ただし -1 のケースは調べていない
	//valid なものを数える
	//minimize されている前提で動く
	//答えが有限でないともちろん壊れる
	int count_valid()const{
		//minimize済み && 答えが有限 => どこかに吸収されている
		int n=size(),f=-1;
		rep(i,n)if(!accept[i]){
			bool ok=true;
			rep(j,K)if(to[i][j]!=i)ok=false;
			if(ok){
				assert(f==-1);
				f=i;
			}
		}
		if(f==-1)return -1;
		vi dp(n),cnt(n);
		dp[root]=1;
		rep(i,n)rep(j,K)cnt[to[i][j]]++;
		vi q;
		rep(i,n)if(cnt[i]==0)q.pb(i);
		int ans=0;
		rep(head,si(q)){
			int i=q[head];
			if(accept[i])ans+=dp[i];
			rep(j,K){
				int v=to[i][j];
				dp[v]+=dp[i];
				if(--cnt[v]==0)
					q.pb(v);
			}
		}
		if(si(q)!=n-1)return -1;
		return ans;
	}
};

template<int K>
struct NFA{
	using A=array<vi,K>;
	int root;
	vc<A> to;
	vc<bool> accept;
	int size()const{return si(to);}
	void resize(int n){
		root=-1;
		to.resize(n);
		rep(i,n)rep(j,K)to[i][j]=-1;
		accept.assign(n,0);
	}
	friend ostream& operator<<(ostream&os,const NFA&a){
		return os<<"{"<<a.root<<","<<a.to<<","<<a.accept<<"}";
	}
	void show()const{
		cerr<<"size: "<<size()<<endl;
		cerr<<"root: "<<root<<endl;
		rep(i,size()){
			cerr<<accept[i]<<" "<<to[i]<<endl;
		}
	}
	static DFA<K> convert_to_dfa(const NFA<K>&a){
		int n=a.size();
		map<vc<bool>,int> idx;
		vc<vc<bool>> q;
		vc<array<int,K>> to;
		vc<bool> accept;
		int head=0;
		
		auto getidx=[&](vc<bool> x){
			auto itr=idx.find(x);
			if(itr!=idx.ed)return itr->b;
			
			int res=si(q);
			q.pb(x);
			to.eb();
			
			bool ok=false;
			rep(i,n)if(x[i]&&a.accept[i])ok=true;
			accept.pb(ok);
			
			return idx[x]=res;
		};
		{
			vc<bool> s(n);
			s[a.root]=1;
			getidx(s);
		}
		while(head<si(q)){
			auto cur=q[head++];
			int curId=idx[cur];
			assert(curId==head-1);
			rep(nx,K){
				vc<bool> tmp(n);
				rep(i,n)if(cur[i])for(auto j:a.to[i][nx])tmp[j]=1;
				int nxId=getidx(tmp);
				to[curId][nx]=nxId;
			}
		}
		return DFA<K>::minimum({0,to,accept});
	}
};

//適当な word w を受理する DFA x を用意する
//w に登場する a (のうち0個以上)を b で置き換えたものも受理するようにする
template<int K>
DFA<K> replace_a_by_b(const DFA<K>&x,int a,vi b){
	assert(si(b)>=1);
	for(auto v:b)assert(inc(0,v,K-1));
	
	int n=si(x),m=si(b);
	using A=array<vi,K>;
	int root=x.root*m;
	vc<A> to(n*m);
	vc<bool> accept(n*m,false);
	
	rep(i,n){
		rep(j,K)to[i*m][j].pb(x.to[i][j]*m);
		int z=x.to[i][a];
		rep(j,m-1)to[i*m+j][b[j]].pb(i*m+j+1);
		to[i*m+m-1][b[m-1]].pb(z*m);
	}
	
	rep(i,n)accept[i*m]=x.accept[i];
	
	return NFA<K>::convert_to_dfa({root,to,accept});
}

template<int K>
DFA<K> onedigit(int v){
	N res;res.resize(3);
	res.root=0;
	rep(i,3)rep(j,K)res.to[i][j]=2;
	res.to[0][v]=1;
	res.accept[1]=true;
	return res;
}
