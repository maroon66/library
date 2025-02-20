template<class N>
struct linkcut{
	using Node=typename splaytree<N>::Node;
	using np=typename Node::np;
	np x;
	linkcut(const linkcut&)=delete;
	void operator=(const linkcut&)=delete;
	linkcut(int n){x=new Node[n];}
	~linkcut(){delete[] x;}
	
	int lca(int aa,int bb){
		np a=x+aa,b=x+bb;
		a->expose();
		b->expose();
		if(!a->p)return -1;
		
		np d=a;
		while(a->p!=b){
			if(a->pos()==0)
				d=a->p;
			a=a->p;
		}
		if(a==b->l)return d-x;
		else return b-x;
	}
	//child->par
	void link(int a,int b){
		x[a].LClink(x+b);
	}
	void cutpar(int a){
		x[a].LCcutpar();
	}
	void expose(int a){
		x[a].expose();
	}
	void evert(int a){
		x[a].evert();
	}
	//これ使うと a-root path の情報が取れる
	void cutchild(int a){
		x[a].LCcutchild();
	}
	void expose_path(int a,int b){
		x[a].evert();
		x[b].LCcutchild();
	}
	N&operator[](int i){
		return x[i].x;
	}
};

struct N{
	int val,len;
	N(int v=0):val(v),len(1){}
	void reverse(){}
	void pushl(N&){}
	void pushr(N&){}
	void clear(){}
	void single(){
		len=1;
	}
	void updatel(const N&x){
		len+=x.len;
	}
	void updater(const N&x){
		len+=x.len;
	}
	bool operator<(const N&rhs)const{
		return val>rhs.val;
	}
};

//i<p[i] の木を管理する
struct pathfinder{
	linkcut<N> lc;
	pathfinder(vi ini):lc(si(ini)+1){
		dmp(ini);
		rep(i,si(ini)+1)lc[i]=N(i);
		rep(i,si(ini))lc.link(i,ini[i]);
	}
	void change(int i,int j){
		dmp2(i,j);
		lc.cutpar(i);
		lc.link(i,j);
	}
	//頂点 x から初めて親を探していく
	//r 未満である限り進むとして，最終的にいる頂点 v と何ステップかかったかを求める
	//LC の根のほうが index の大きい頂点なので，val が降順になっている
	pi query(int l,int r){
		lc.expose(l);
		lc.cutchild(l);
		auto [x,y]=splaytree<N>::split_sub<false>(&lc.x[l],less<N>(),r-1);
		assert(y);
		int tot=y->x.len;
		if(x)tot-=x->x.len;
		dmp2(l,r,y->x.val,tot-1);
		return pi(y->x.val,tot-1);
	}
};

//UCUP 3-27-B
//lc.lc.expose(i);
//lc.lc[i].val=0;
//lc.lc.x[i].update();
