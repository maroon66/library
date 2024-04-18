//N(len) 長さ len の空ノード
//len は多分 0 (単位元) と 2 冪しか来ない
//lazyなし
//動的 segtree　としても使える
//root は最初 -1 あたりを使おう
//Petrozavodsk 2020S Day3D
template<class N>
struct psegtree{
	struct Node{
		N v;
		int l,r;
		Node(int len):v(len),l(-1),r(-1){}
	};
	vc<Node> x;
	int s;
	psegtree(int n){
		s=1;
		while(s<n)s*=2;
	}
	int nn(int len){
		int i=si(x);x.eb(len);
		return i;
	}
	int cn(int i,int len){
		if(i==-1){
			return nn(len);
		}else{
			int a=si(x);
			x.pb(x[i]);
			return a;
		}
	}
	void upd(int i,int len){
		x[i].v=N::merge(
			x[i].l!=-1?x[x[i].l].v:N(len/2),
			x[i].r!=-1?x[x[i].r].v:N(len/2)
		);
	}
	template<class F,class...Args>
	int chr(int i,int l,int r,int p,F f,Args&&...args){
		int a=cn(i,r-l);
		if(l+1==r){
			(x[a].v.*f)(forward<Args>(args)...);
		}else{
			int m=(l+r)/2;
			if(p<m){
				int tmp=chr(x[a].l,l,m,p,f,forward<Args>(args)...);
				x[a].l=tmp;
			}else{
				int tmp=chr(x[a].r,m,r,p,f,forward<Args>(args)...);
				x[a].r=tmp;
			}
			upd(a,r-l);
		}
		return a;
	}
	template<class F,class...Args>
	int ch(int i,int p,F f,Args&&...args){
		return chr(i,0,s,p,f,forward<Args>(args)...);
	}
	N compositer(int b,int e,int i,int l,int r){
		if(e<=l||r<=b){return N(0);}
		if(b<=l&&r<=e){return i==-1?N(r-l):x[i].v;}
		int p=-1,q=-1;
		if(i!=-1)p=x[i].l,q=x[i].r;
		return N::merge(compositer(b,e,p,l,(l+r)/2),compositer(b,e,q,(l+r)/2,r));
	}
	N composite(int root,int b,int e){
		assert(b<=e);
		return compositer(b,e,root,0,s);
	}
};

//RMQ
struct N{
	pi mx;
	N(int):mx(-inf,-1){}
	void setval(pi v){
		mx=v;
	}
	static N merge(const N&x,const N&y){
		N res(0);
		res.mx=max(x.mx,y.mx);
		return res;
	}
};

void show(const psegtree<N>&seg,int i){
	vi content;
	auto rec=[&](auto self,int x,int l,int r){
		if(x==-1)return;
		if(l+1==r){
			//rep(_,seg.x[x].v.a[0].v)
			//	content.pb(l);
		}else{
			self(self,seg.x[x].l,l,(l+r)/2);
			self(self,seg.x[x].r,(l+r)/2,r);
		}
	};
	rec(rec,i,0,seg.s);
	dmp2(content);
};

//Petrozavodsk 2020S Day3D
//数列の辞書順比較（ただしでかいものから優先して比較）を hash とかで実現している

const int K=2;

using A=array<mint,K>;
const int smax=1<<18;
A pw[smax+1];
void initpw(A w){
	pw[0]={1,1};
	rep(i,smax)rep(j,K)pw[i+1][j]=pw[i][j]*w[j];
}

struct N{
	A a;
	int len;
	N(int l):len(l){}
	void add(int v){
		assert(len==1);
		rep(i,K)a[i]+=v;
	}
	static N merge(const N&a,const N&b){
		N res(a.len+b.len);
		rep(i,K)
			res.a[i]=a.a[i]+b.a[i]*pw[a.len][i];
		return res;
	}
};

void unko(A&a,const A&b){
	rep(i,K)a[i]+=b[i];
}

template<int S>
int cmp(const psegtree<N>&seg,int len,array<int,S>a,array<int,S>b){
	using B=array<int,S>;
	auto load=[&](B c,A&res,B&l,B&r){
		rep(i,S){
			if(c[i]!=-1){
				unko(res,seg.x[c[i]].v.a);
				l[i]=seg.x[c[i]].l;
				r[i]=seg.x[c[i]].r;
			}else{
				l[i]=-1;
				r[i]=-1;
			}
		}
		return res;
	};
	A x{},y{};
	B la,ra,lb,rb;
	load(a,x,la,ra);
	load(b,y,lb,rb);
	if(x==y)return 0;
	if(len==1)return x<y?-1:1;
	else{
		int z=cmp<S>(seg,len/2,ra,rb);
		if(z)return z;
		else return cmp<S>(seg,len/2,la,lb);
	}
}
