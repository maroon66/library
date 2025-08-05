//最も基本的な動的 segtree
//ByteCamp 2022 Day4 I
//IOI2022 1C
//UCUP 2-11-J
//UCUP 3-39-P
//meld,max_right_simple
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
		int a=nl(N(0));
		rep(i,L)a=nn(a,a);
		return a;
	}
	//assume initialized
	//not verified
	/*int load(const vc<N>&raw){
		vi ls(si(raw));
		rep(i,si(raw))ls[i]=nl(raw[i]);
		rep(lv,L){
			if(si(ls)%2==1)ls.pb(lv);
			int len=si(ls);
			rep(i,len/2){
				ls[i]=nn(ls[i*2],ls[i*2+1])
			}
			ls.resize(len/2);
		}
		assert(si(ls)==1);
		return ls[0];
	}*/
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
	template<class F,class...Args>
	int point_change(int root,int i,F f,Args&&...args){
		static int buf[40];
		rep(lv,L){
			buf[lv]=root;
			if((i>>(L-1-lv))&1)root=x[root].r;
			else root=x[root].l;
		}
		N tmp=x[root].v;
		(tmp.*f)(forward<Args>(args)...);
		int res=nl(tmp);
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
	int melddfs(int a,int b,int lv){
		if(a<=L)return b;
		if(b<=L)return a;
		if(lv==L)return nl(N::meld_leaf(x[a].v,x[b].v));
		return nn(melddfs(x[a].l,x[b].l,lv+1),melddfs(x[a].r,x[b].r,lv+1));
	}
	int meld(int a,int b){
		return melddfs(a,b,0);
	}
	int max_right_simple(int root){
		N w=N();
		assert(N().ok());
		assert(!x[root].v.ok());
		int res=0;
		rep(lv,L){
			int l=x[root].l,r=x[root].r;
			N tmp=N::merge(w,x[l].v);
			if(tmp.ok()){
				res+=1<<(L-1-lv);
				root=r;
				w=tmp;
			}else{
				root=l;
			}
		}
		return res;
	}
};
//init の初期値注意!
