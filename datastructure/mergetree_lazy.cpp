//psegtree を元に作成
//CF ThinkCell1 G(MLE)
//頂点 0,..,L が無のノード
//根→葉の順番
//こいつらに push されることがあるかもしれない
//無に push は無の効果しかない，ならいいんだけど・・・
template<class N>
struct mergetree{
	struct N2{
		N v;
		int l,r;
	};
	vc<N2> x;
	int n,L,s;
	mergetree(int foresight=0):x(foresight){}
	int init(int n_){
		n=n_;
		x.clear();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		int a=nl(N());
		rep(i,L)a=nn(a,a);
		assert(si(x)==L+1);
		rein(x);
		return 0;
	}
	int nl(const N&val){
		x.pb({val,-1,-1});
		return si(x)-1;
	}
	int nn(int l,int r){
		x.pb({N::merge(x[l].v,x[r].v),l,r});
		return si(x)-1;
	}
	void nli(const N&val,int i){
		x[i]={val,-1,-1};
	}
	void nni(int l,int r,int i){
		x[i]={N::merge(x[l].v,x[r].v),l,r};
	}
	int single(int pos,const N&val){
		int res=nl(val);
		per(i,L){
			if(pos&1<<(L-1-i))
				res=nn(i+1,res);
			else
				res=nn(res,i+1);
		}
		return res;
	}
	void push(int i){
		assert(x[i].l!=-1&&x[i].r!=-1);
		x[i].v.push(x[x[i].l].v,x[x[i].r].v);
	}
	N compositedfs(int root,int l,int r,int b,int e){
		if(e<=l||r<=b)return N();
		if(b<=l&&r<=e)return x[root].v;
		push(root);
		int mid=(l+r)/2;
		return N::merge(compositedfs(x[root].l,l,mid,b,e),compositedfs(x[root].r,mid,r,b,e));
	}
	N composite(int root,int b,int e){
		assert(0<=b&&b<=e&&e<=n);
		if(b==e)return N();
		return compositedfs(root,0,s,b,e);
	}
	int melddfs(int a,int b,int lv){
		if(a<=L)return b;
		if(b<=L)return a;
		if(lv==L){
			nli(N::meld(x[a].v,x[b].v),a);
			return a;
		}else{
			push(a);
			push(b);
			nni(
				melddfs(x[a].l,x[b].l,lv+1),
				melddfs(x[a].r,x[b].r,lv+1),
				a
			);
			return a;
		}
	}
	int meld(int a,int b){
		return melddfs(a,b,0);
	}
	N point_get(int root,int i){
		int cur=root;
		per(lv,L){
			push(cur);
			if(i&1<<lv)cur=x[cur].r;
			else cur=x[cur].l;
		}
		return x[cur].v;
	}
	//ここより下，未verify
	pi splitdfs(int root,int lv,int cut){
		if(root<=L)return pi(root,root);
		int len=1<<(L-lv);
		if(cut==0){
			return pi(lv,root);
		}else if(cut==len){
			return pi(root,lv);
		}
		push(root);
		if(cut<=len/2){
			auto [a,b]=splitdfs(x[root].l,lv+1,cut);
			a=nn(a,lv+1);
			nni(b,x[root].r,root); b=root;
			return pi(a,b);
		}else{
			auto [a,b]=splitdfs(x[root].r,lv+1,cut-len/2);
			b=nn(lv+1,b);
			nni(x[root].l,a,root); a=root;
			return pi(a,b);
		}
	}
	pi split(int root,int cut){
		assert(inc(0,cut,1<<L));
		return splitdfs(root,0,cut);
	}
	template<class F,class... Args>
	void chall(int root,F f,Args&&... args){
		if(root<=L)return;
		(x[root].v.*f)(forward<Args>(args)...);
	}
	template<class F>
	void enumerate_dfs(int root,int l,int r,F f){
		if(root<=L)return;
		if(r-l==1){
			f(l,x[root].v);
		}else{
			enumerate_dfs(x[root].l,l,(l+r)/2,f);
			enumerate_dfs(x[root].r,(l+r)/2,r,f);
		}
	}
	template<class F>
	void enumerate(int root,F f){
		enumerate_dfs(root,0,s,f);
	}
};
