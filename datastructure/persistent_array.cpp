//永続配列
//UCUP 2-11-J
template<class N>
struct persistent_array{
	union N2{
		N v;
		struct{int l,r;};
		N2(const N&vv):v(vv){}
		N2(int a,int b):l(a),r(b){}
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
	persistent_array(int foresight=0){x.reserve(foresight);}
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
		x.eb(val);
		return si(x)-1;
	}
	int nn(int l,int r){
		x.eb(l,r);
		return si(x)-1;
	}
	int set(int root,int i,const N&val){
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
	/*int point_merge(int root,int i,const N&val){
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
	}*/
	N get(int root,int i){
		int cur=root;
		per(lv,L){
			if(i&1<<lv)cur=x[cur].r;
			else cur=x[cur].l;
		}
		return x[cur].v;
	}
};

//UCUP 2-11-J
struct persistent_aho{
	persistent_array<int> seg;
	struct N{
		map<int,int> nx;
		int root,fail;
		N():root(-1),fail(-1){}
	};
	vc<N> x;
	persistent_aho(int alphabet):x(1){
		x[0].root=seg.init(alphabet);
	}
	int add(int v,int c){
		if(x[v].nx.contains(c))return x[v].nx[c];
		int to=si(x);
		x.eb();
		x[v].nx[c]=to;
		return to;
	}
	vi idx;
	void bfs(){
		idx.pb(0);
		rep(h,si(idx)){
			int i=idx[h],f=x[i].fail;
			if(f!=-1){
				x[i].root=x[f].root;
			}
			for(auto [c,j]:x[i].nx){
				x[i].root=seg.set(x[i].root,c,j);
				if(f==-1){
					x[j].fail=0;
				}else{
					x[j].fail=trans(f,c);
				}
				idx.pb(j);
			}
		}
	}
	int trans(int v,int c){
		return seg.get(x[v].root,c);
	}
	N& operator[](int i){return x[i];}
};
