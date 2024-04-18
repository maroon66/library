template<class t>
struct BIT{
	vc<t> buf;
	int s;
	BIT(int n=0){init(n);}
	BIT(const vc<t>&a){init(a);}
	void init(int n){buf.clear();buf.resize(s=n);}
	void init(const vc<t>&a){
		s=si(a);
		buf.resize(s);
		rep(i,s)buf[i]=a[i];
		rep(i,s){
			int j=i+((i+1)&(-i-1));
			if(j<s)buf[j]+=buf[i];
		}
	}
	void add(int i,t v){
		for(;i<s;i+=(i+1)&(-i-1))
			buf[i]+=v;
	}
	t get(int i){
		t res=t();
		for(;i>=0;i-=(i+1)&(-i-1))
			res+=buf[i];
		return res;
	}
	t sum(int b,int e){
		return get(e-1)-get(b-1);
	}
	void add_range(int b,int e,t v){
		add(b,v);
		add(e,-v);
	}
	int kth(int k){
		int res=0;
		for(int i=topbit(s);i>=0;i--){
			int w=res+(1<<i);
			if(w<=s&&buf[w-1]<=k){
				k-=buf[w-1];
				res=w;
			}
		}
		return res;
	}
	//UCUP 2-9-I
	int kth_helper(int i,int k){
		return kth(k+get(i-1));
	}
};
