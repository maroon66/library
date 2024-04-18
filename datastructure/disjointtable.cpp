//verify yosupo static range sum
template<class t,class u>struct disjointtable{
	vvc<t> a;
	u f;
	const t id;
	disjointtable(const vc<t>& d,u ff,t w):f(ff),id(w){
		if(d.empty())return;
		int n=si(d),h=__lg(max<int>(1,n-1));
		a.resize(h+1);
		a[0]=d;
		rng(k,1,h+1){
			a[k].resize(n,id);
			int s=1<<k;
			for(int i=s;i<n;i+=s*2){
				t cur=id;
				gnr(j,i-s,i)a[k][j]=cur=f(d[j],cur);
				cur=id;
				rng(j,i,min(i+s,n))a[k][j]=cur=f(cur,d[j]);
			}
		}
	}
	t get(int b,int e){
		assert(b<=e);
		if(b==e)return id;
		e--;
		if(b==e)return a[0][b];
		int k=__lg(b^e);
		return f(a[k][b],a[k][e]);
	}
};

int sum(int a,int b){return a+b;}
disjointtable<int,decltype(&sum)> t(a,sum,0);
