//copy-constructor，使えません
//find は lazy と組み合わせても動く (Petrozavodsk 2020w Day9 C)
//reverse は未知数
//erase,insert は動く
//split_cmp も動く
//max_right も動く

//AOJ DSL2G
template<class N>
struct splaytree{
	struct Node{
		using np=Node*;
		np l,r,p;
		bool rev;
		N x;
		Node():l(0),r(0),p(0),rev(false),x(){}
		void clear(){
			l=0;
			r=0;
			p=0;
			rev=0;
		}
		void reverse(){
			rev^=true;
			swap(l,r);
			x.reverse();
		}
		void push(){
			if(rev){
				if(l)l->reverse();
				if(r)r->reverse();
				rev=false;
			}
			if(l)x.pushl(l->x);
			if(r)x.pushr(r->x);
			x.clear();
		}
		np update(){
			x.single();
			if(l)x.updatel(l->x);
			if(r)x.updater(r->x);
			return this;
		}
		int pos(){
			if(p&&p->l==this)return -1;
			if(p&&p->r==this)return 1;
			return 0;
		}
		void prepare(){
			if(pos())
				p->prepare();
			push();
		}
		void rotate(){
			np q=p,c;
			if(pos()==1){
				c=l;
				l=p;
				p->r=c;
			}else{
				c=r;
				r=p;
				p->l=c;
			}
			if(c)c->p=p;
			p=p->p;
			q->p=this;
			if(p&&p->l==q)p->l=this;
			if(p&&p->r==q)p->r=this;
			q->update();
		}
		np splay(){
			prepare();
			while(pos()){
				int a=pos(),b=p->pos();
				if(b&&a==b)p->rotate();
				if(b&&a!=b)rotate();
				rotate();
			}
			return update();
		}
		/* deprecated
		template<class F,class...Args>
		np find(F f,Args&&...args){
			if(!(x.*f)(forward<Args>(args)...))return 0;
			push();
			x.single();
			np a=0;
			if(l)a=l->find(f,forward<Args>(args)...);
			if(a)return a;
			if((x.*f)(forward<Args>(args)...))return splay();
			return r->find(f,forward<Args>(args)...);
		}*/
		np left(){
			if(l)return l->left();
			else return splay();
		}
		np right(){
			if(r)return r->right();
			else return splay();
		}
		/*np root(){
			if(p)return p->root();
			else return this;
		}*/
		void chpar(np c){
			if(c){
				assert(!(c->p));
				c->p=this;
			}
		}
		np linkl(np c){
			assert(!l);
			chpar(l=c);
			return update();
		}
		np linkr(np c){
			assert(!r);
			chpar(r=c);
			return update();
		}
		np linklr(np c,np d){
			assert(!l&&!r);
			chpar(l=c);
			chpar(r=d);
			return update();
		}
		np cutl(){
			if(l){
				l->p=0;
				l=0;
			}
			return update();
		}
		np cutr(){
			if(r){
				r->p=0;
				r=0;
			}
			return update();
		}
		//XIX Opencup GP of Zhejiang A
		np get_next(){
			splay();
			if(!r)return 0;
			else return r->left();
		}
		//linkcut 用
		void expose(){
			for(np a=this;a;a=a->p)a->splay();
			for(np a=this;a->p;a=a->p){
				a->p->r=a;
				a->p->update();
			}
			splay();
		}
		void evert(){
			expose();
			if(l){
				l->reverse();
				l=0;
				update();
			}
		}
		void LClink(np a){
			evert();
			p=a;
		}
		void LCcutchild(){
			expose();
			r=0;
			update();
		}
		void LCcutpar() {
			expose();
			assert(l);
			l->p=0;
			l=0;
			update();
		}
	};
	using np=Node*;
	np head;
	int alloc_total=0;
	splaytree(const splaytree&)=delete;
	void operator=(const splaytree&)=delete;
	splaytree():head(0){}
	~splaytree(){
		int del_total=0;
		while(head){
			del_total++;
			np nx=head->p;
			delete head;
			head=nx;
		}
		assert(alloc_total==del_total);
	}
	//TL,MLが厳しいとき
	//FHC2022 Final E
	//もともと 118 秒くらいのコードが 98 秒になったが…
	/*splaytree(){
		const int S=20000000;
		np buf=new Node[S];
		buf[0].p=0;
		rng(i,1,S)buf[i].p=buf+(i-1);
		head=buf+(S-1);
	}
	~splaytree(){}*/
	np allocnode(){
		if(head){
			np res=head;
			head=res->p;
			return res;
		}else{
			alloc_total++;
			return new Node();
		}
	}
	//読んだ直後，x->p 以外の情報は生きている
	void freenode(np x){
		x->p=head;
		head=x;
	}
	/*vc<np> ps;
	unique_ptr<Node[]> buf;
	splaytree(int n):buf(new Node[n]),ps(n){
		rep(i,n)ps[i]=buf.get()+i;
	}
	//alloc/free 系書いてないわ
	*/
	//FHC2022 Final E/range_set/rect_set
	//assume no duplicate free
	void freetree(np x){
		if(!x)return;
		freetree(x->l);
		freetree(x->r);
		freenode(x);
		//ps.pb(x);
	}
	template<class...Args>
	np nn(Args&&...args){
		np a=allocnode();
		a->l=0;
		a->r=0;
		a->p=0;
		a->x=N(forward<Args>(args)...);
		return a;
	}
	np merge(np a,np b){
		if(!a)return b;
		if(!b)return a;
		return b->splay()->left()->linkl(a->splay());
	}
	//GCJ2022 Round2 D
	template<class...Args>
	np merge(np a,np b,Args...args){
		return merge(merge(a,b),args...);
	}
	template<bool C,class F,class...Args>
	static pair<np,np> max_right_sub(np a,F f,Args&&...args){
		N cur;
		np x=0,y=0;
		while(a){
			a->push();
			N nx=a->x;nx.single();
			if(a->l)nx.updatel(a->l->x);
			nx.updatel(cur);
			if((nx.*f)(forward<Args>(args)...)){
				cur=nx;
				x=a;
				a=a->r;
			}else{
				y=a;
				a=a->l;
			}
		}
		if(x){
			x->splay();
			if constexpr(C)x->cutr();
		}
		if(y)y->splay();
		return mp(x,y);
	}
	//max_right で失敗する最初のノードを根にする
	//そのようなものがない場合は false が返ってくる
	//UCUP3-17-L
	template<class F,class...Args>
	static bool max_right_splay(np&a,F f,Args&&...args){
		auto [x,y]=max_right_sub<false>(a,f,forward<Args>(args)...);
		if(y){
			a=y;
			return true;
		}else{
			a=x;
			return false;
		}
	}
	//分けた列の右端と左端が返ってくる
	//CF Dytechlab Cup 2022 G (lazy あり)
	template<class F,class...Args>
	static pair<np,np> max_right_split(np a,F f,Args&&...args){
		return max_right_sub<true>(a,f,forward<Args>(args)...);
	}
	//XX Opencup GP of Wroclaw D
	//分けた列の右端と左端が返ってくる
	//CF740 D (lazy あり)
	//CF Dytechlab Cup 2022 G (lazy あり)
	template<bool C,class F,class T>
	static pair<np,np> split_sub(np a,F cmp,T v){
		np x=0,y=0;
		while(a){
			a->push();
			if(cmp(a->x,v)){
				x=a;
				a=a->r;
			}else{
				y=a;
				a=a->l;
			}
		}
		if(x){
			x->splay();
			if constexpr(C)x->cutr();
		}
		if(y)y->splay();
		return mp(x,y);
	}
	template<class F,class T>
	pair<np,np> split_cmp(np a,F cmp,T v){
		return split_sub<true>(a,cmp,v);
	}
	//cmp(x,v)=false になる最初の x を根にして返す
	//そのようなものがない場合は false が返ってくる
	//FHC2022 Final E/range_set/rect_set
	template<class F,class T>
	bool lower_bound_cmp(np&a,F cmp,T v){
		auto [x,y]=split_sub<false>(a,cmp,v);
		if(y){
			a=y;
			return true;
		}else{
			//not verified?
			a=x;
			return false;
		}
	}
	//XIX Opencup GP of Zhejiang A
	//FHC2022 Final E/range_set/rect_set
	template<class F>
	void insert_cmp(np&x,F cmp,np v){
		assert(!v->l&&!v->r&&!v->p&&!v->rev);
		//auto [a,b]=split_cmp(x,f,v->x);
		//x=v->linklr(a,b);
		if(!x){
			x=v;
			return;
		}else{
			np p=0;
			bool l;
			while(x){
				x->push();
				p=x;
				if(cmp(x->x,v->x)){
					l=false;
					x=x->r;
				}else{
					l=true;
					x=x->l;
				}
			}
			if(l){
				p->linkl(v);
			}else{
				p->linkr(v);
			}
			x=v->splay();
		}
	}
	//XX Opencup GP of Wroclaw D
	//FHC2022 Final E/range_set/rect_set
	template<class F,class...Args>
	void emplace_cmp(np&x,F f,Args&&...args){
		insert_cmp(x,f,nn(forward<Args>(args)...));
	}
	//FHC2022 Final E/range_set/rect_set
	template<class...Args>
	void emplace(np&x,Args&&...args){
		emplace_cmp(x,less<N>(),forward<Args>(args)...);
	}
	//XX Opencup GP of Wroclaw D
	pair<np,np> erase_sub(np x){
		x->splay();
		if(x->l)x->l->p=0;
		if(x->r)x->r->p=0;
		freenode(x);
		return mp(x->l,x->r);
	}
	//CF Dytechlab Cup 2022 G
	void erase(np&x){
		auto [a,b]=erase_sub(x);
		x=merge(a,b);
	}
	//FHC2022 Final E/range_set/rect_set
	//less,eq,
	template<class F,class T,class G>
	bool erase_cmp_eq(np&x,F f,G g,T v){
		if(lower_bound_cmp(x,f,v)&&g(x->x,v)){
			erase(x);
			return true;
		}
		return false;
	}
	//FHC2022 Final E/range_set/rect_set
	template<class T>
	bool erase(np&x,T v){
		return erase_cmp_eq(x,less<N>(),equal_to<N>(),v);
	}
	//Petrozavodsk 2020w Day9 H
	np isolate(np x){
		x->splay();
		if(x->l)x->l->p=0;
		if(x->r)x->r->p=0;
		np res=merge(x->l,x->r);
		x->x.single();
		x->clear();
		return res;
	}
	template<class t>
	np build(vc<t> v){
		vc<np> cur;
		for(auto z:v)cur.pb(nn(z));
		while(cur.size()>1){
			int s=cur.size();
			vc<np> nx((s+1)/2);
			for(int i=0;i<s;i+=2){
				if(i+1<s)nx[i/2]=merge(cur[i],cur[i+1]);
				else nx[i/2]=cur[i];
			}
			swap(nx,cur);
		}
		return cur[0];
	}
	/*
	//NOT VERIFIED
	//lower_bound したものを根に移して node を返す．
	//lower_bound の結果が end だと空が返ってくるらしい
	//USACO2021 USOPEN Platinum A
	template<class F>
	np lower_bound_cmp(np a,F cmp,N v){
		auto [x,y]=split_cmp(a,cmp,v);
		np res=nullptr;
		if(y)res=y->left();
		merge(x,res);
		if(res)res->splay();
		return res;
	}
	*/
	//XIX Opencup GP of Zhejiang A
	//a-b なる部分を取り出し，x,y(a-b),z を返す
	//a と b が異なる木に属する，また，a と b の順序がおかしい場合，何が起きるかは不明
	tuple<np,np,np> split_range(np a,np b){
		{//debug
			a->splay();
			b->splay();
			int lastpos;
			auto c=a;
			while(c&&c!=b){
				lastpos=c->pos();
				c=c->p;
			}
			assert(c==b);
			assert(lastpos==-1);
		}
		a->splay();
		np x=a->l;
		a->cutl();
		b->splay();
		np z=b->r;
		b->cutr();
		return mt(x,b,z);
	}
	//CF743F(TLE)
	//The 2022 ICPC Asia Nanjing Regional Contest H
	template<class F>
	void weighted_merge_rec_cmp(np&tar,F f,np x){
		if(!x)return;
		x->push();
		np l=x->l,r=x->r;
		x->x.single();
		x->clear();
		weighted_merge_rec_cmp(tar,f,l);
		insert_cmp(tar,f,x);
		weighted_merge_rec_cmp(tar,f,r);
	}
	template<class F>
	np weighted_merge_cmp(np a,F f,np b){
		if(!a)return b;
		if(!b)return a;
		if(a->x.len<b->x.len)swap(a,b);
		weighted_merge_rec_cmp(a,f,b);
		return a;
	}
	//Petrozavodsk 2020w Day9 C
	void enumerate(np x,vc<N>&dst){
		if(!x)return;
		x->push();
		enumerate(x->l,dst);
		dst.pb(x->x);
		enumerate(x->r,dst);
	}
	/* deprecated
	//Petrozavodsk 2020w Day9 H
	template<class F>
	np insert(np r,np x,F f){
		np a,b;tie(a,b)=split(r,f,x->x);
		return merge(merge(a,x),b);
	}
	template<class F,class...Args>
	np insert(np r,F f,Args&&...args){
		np x=nn(forward<Args>(args)...);
		np a,b;tie(a,b)=split(r,f,x->x);
		return merge(merge(a,x),b);
	}
	//左の列の根は右端とは限らない！
	//右の列の根は左端だと思う
	template<class F,class...Args>
	pair<np,np> split(np a,F f,Args&&...args){
		if(!a)return {0,0};
		np b=a->find(f,forward<Args>(args)...);
		if(b){
			np c=b->l;
			return {c,b->cutl()};
		}
		return {a,0};
	}
	//GCJ2022 Round2 D
	tuple<np> split_by_len(np a){
		return a;
	}
	template<class...Args>
	auto split_by_len(np a,int len,Args...args){
		assert((!a&&len==0)||inc(0,len,a->x.len));
		auto [b,c]=split(a,&N::find_by_len,len);
		assert(len==0);
		return tuple_cat(tuple{b},split_by_len(c,args...));
	}
	//Petrozavodsk 2020w Day9 C
	template<class F>
	np weighted_merge_rec(np x,np tar,F f){
		if(!x)return tar;
		x->push();
		tar=weighted_merge_rec(x->l,tar,f);
		tar=weighted_merge_rec(x->r,tar,f);
		x->x.single();
		x->clear();
		return insert(tar,x,f);
	}
	//Petrozavodsk 2020w Day9 C
	template<class F>
	np weighted_merge(np a,np b,F f){
		if(!a)return b;
		if(!b)return a;
		if(a->x.sz<b->x.sz)swap(a,b);
		return weighted_merge_rec(b,a,f);
	}
	*/
};
//デフォルトコンストラクターが null node になっているべき (例えば len=0)
//N::reverse
//N::push→ pushl,pushr
//副作用なし,１個の子にpush
//N::clear
//lazy tagを消去
//N::single
//ノード単体の情報を元に部分木情報を初期化
//N::updatel,updater
//N::find
//find はその部分木内に目標とするノードがあるかどうかを返す
//split のやり方を変えたい
//max_right のノリで split をする
//split_cmp は cmp(x,v) が true になる最大の x を見つけてそれで分離
//↓deprecared
//split は find で見つけたものが右の部分木の left になるように分離する
//普通に a<b を渡すと lower_bound と同じ効果が得られる
//split_by_len で左 len 個とそれ以外に分離する
//find_by_len という比較関数が定義されていないと破壊
