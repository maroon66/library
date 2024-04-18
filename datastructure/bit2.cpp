//CF733H
const int nmax=15010;
const int L=14;

template<class N>
struct BIT{
	int n;
	N x[nmax];
	void init(int nn){
		n=nn;
		rep(i,n)x[i]=N();
	}
	void upd(int i,N val){
		for(;i<n;i+=(i+1)&(-i-1))x[i]=N::merge(x[i],val);
	}
	N get(int i){
		N res;
		for(;i>=0;i-=(i+1)&(-i-1))res=N::merge(res,x[i]);
		return res;
	}
	template <class F,class... Args> 
	int find(F f,Args&&... args){
		N cur;
		int res=0;
		per(i,L){
			int w=res+(1<<i);
			if(w<=n){
				N nx=N::merge(cur,x[w-1]);
				if((nx.*f)(forward<Args>(args)...)){
					cur=nx;
					res=w;
				}
			}
		}
		return res;
	}
};

struct MinNode{
	int v;
	MinNode(int vv=inf):v(vv){}
	static MinNode merge(const MinNode&a,const MinNode&b){
		return MinNode(min(a.v,b.v));
	}
	bool ok(int x){return x<=v;}
};

struct MaxNode{
	int v;
	MaxNode(int vv=-inf):v(vv){}
	static MaxNode merge(const MaxNode&a,const MaxNode&b){
		return MaxNode(max(a.v,b.v));
	}
	bool ok(int x){return v<=x;}
};
