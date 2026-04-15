//頂点 0,..,L が無のノード
//根→葉の順番
//永続ではない
//meldしたらmeld元は壊れると思いなさい
//JOISC2025 Migration
template<class N>
struct mergetree{
	struct N2{
		N v;
		int l,r;
	};
	vc<N2> x;
	int n,L,s;
	//root of empty tree is 0
	mergetree(int n_,N ini,int m){
		n=n_;
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.resize((L+1)*(m+1));x.clear();
		int a=nl(ini);
		rep(i,L)a=nn(a,a);
		assert(si(x)==L+1);
		rein(x);
	}
	int nl(const N&val){
		x.pb({val,-1,-1});
		return si(x)-1;
	}
	int nn(int l,int r){//l,r<=L のときに空ノードを返すとかするとメモリ節約できるシーンがあるかもしれないし，ないかもしれない
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
	N compositedfs(int root,int l,int r,int b,int e){
		if(e<=l||r<=b)return N();
		if(b<=l&&r<=e)return x[root].v;
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
			nli(N::merge(x[a].v,x[b].v),a);
			return a;
		}else{
			nni(
				melddfs(x[a].l,x[b].l,lv+1),
				melddfs(x[a].r,x[b].r,lv+1),
				a
			);
			return a;
		}
	}
	//make b empty
	void meld(int&a,int&b){
		a=melddfs(a,b,0);
		b=0;
	}
	void meld(int&a,int&&b){
		a=melddfs(a,b,0);
		b=0;
	}
	N point_get(int root,int i){
		int cur=root;
		per(lv,L){
			if(i&1<<lv)cur=x[cur].r;
			else cur=x[cur].l;
		}
		return x[cur].v;
	}
	//UOJ Easy 13 B
	template <class F,class... Args> 
	pi split(int root,F f,Args&&... args){
		if((x[root].v.*f)(forward<Args>(args)...)){
			return pi(root,0);
		}
		N sum;
		static bool goright[30];
		static int other[30];
		int i=root,done=0;
		rep(lv,L){
			if(i<=L)break;
			if(x[i].v.v==0)break; //tokusyu
			N tmp=N::merge(sum,x[x[i].l].v);
			if((tmp.*f)(forward<Args>(args)...)){
				goright[lv]=true;
				other[lv]=x[i].l;
				i=x[i].r;
				sum=tmp;
			}else{
				goright[lv]=false;
				other[lv]=x[i].r;
				i=x[i].l;
			}
			done++;
		}
		int a=done,b=i;
		per(lv,L){
			if(goright[lv]){
				a=nn(other[lv],a),b=nn(lv+1,b);
			}else{
				a=nn(a,lv+1),b=nn(b,other[lv]);
			}
		}
		
		return pi(a,b);
	}
	void dumpdfs(int i,int lv,int pos,vi&dst){
		if(x[i].v.v==0)return;
		if(lv==L)return dst.pb(pos);
		dumpdfs(x[i].l,lv+1,pos,dst);
		dumpdfs(x[i].r,lv+1,pos+(1<<(L-1-lv)),dst);
	}
	void dump(int root,vi&dst){
		dumpdfs(root,0,0,dst);
	}
};
