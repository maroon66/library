//最も基本的な動的 segtree
//ByteCamp 2022 Day4 I
//IOI2022 1C
//UCUP 2-11-J
template<class N>
struct psegtree{
	struct N2{
		N v;
		int l,r;
	};
	vc<N2> x;
	int L,s;
	//数列から root を取る関数として作ったほうがよくね?
	/*template<class t>
	psegtree(const vc<t>&a,int foresight=0):x(foresight){
		x.clear();
		int n=a.size();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.resize(s*2,{N(),0,0});
		rep(i,n)x[s+i].v=N(a[i]);
		gnr(i,1,s){
			x[i].v=N::merge(x[i*2].v,x[i*2+1].v);
			x[i].l=i*2;
			x[i].r=i*2+1;
		}
	}*/
	psegtree(int foresight=0):x(foresight){}
	int init(int n){
		x.clear();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		int a=nl(N());
		rep(i,L)a=nn(a,a);
		return a;
	}
	int nl(const N&val){
		x.pb({val,-1,-1});
		return si(x)-1;
	}
	int nn(int l,int r){
		x.pb({N::merge(x[l].v,x[r].v),l,r});
		return si(x)-1;
	}
	int point_set(int root,int i,const N&val){
		static int buf[40];
		rep(lv,L){
			buf[lv]=root;
			if((i>>(L-1-lv))&1)root=x[root].r;
			else root=x[root].l;
		}
		int res=nl(val);
		per(lv,L){
			int j=buf[lv];
			if((i>>(L-1-lv))&1){
				res=nn(x[j].l,res);
			}else{
				res=nn(res,x[j].r);
			}
		}
		return res;
	}
	int point_merge(int root,int i,const N&val){
		static int buf[40];
		rep(lv,L){
			buf[lv]=root;
			if((i>>(L-1-lv))&1)root=x[root].r;
			else root=x[root].l;
		}
		int res=nl(N::merge(x[root].v,val));
		per(lv,L){
			int j=buf[lv];
			if((i>>(L-1-lv))&1){
				res=nn(x[j].l,res);
			}else{
				res=nn(res,x[j].r);
			}
		}
		return res;
	}
	N compositedfs(int root,int l,int r,int b,int e){
		if(e<=l||r<=b)return N();
		if(b<=l&&r<=e)return x[root].v;
		int mid=(l+r)/2;
		return N::merge(compositedfs(x[root].l,l,mid,b,e),compositedfs(x[root].r,mid,r,b,e));
	}
	N composite(int root,int b,int e){
		assert(b<=e);
		if(b==e)return N();
		return compositedfs(root,0,s,b,e);
	}
	N point_get(int root,int i){
		int cur=root;
		per(lv,L){
			if(i&1<<lv)cur=x[cur].r;
			else cur=x[cur].l;
		}
		return x[cur].v;
	}
};
