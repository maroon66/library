//永続用のコードを適当にコピーしてきました！
//壊れても知りません！
//UCUP 1-16 C (lazyなし)
//コメントアウトしてあるところは全く試してない
//CF887F (lazy)
//TOKI35G (lazy)
//#define AVLLAZY
//https://qiita.com/QCFium/items/3cf26a6dc2d49ef490d7
template<class N>
struct avl{
	struct N2{
		using np=N2*;
		N v;
		np l,r,p;
		int h;
		//h は葉までの最長パスのノード数
		//null node で 0
		//v has no lazy data
		np upd(){
			v.single();
			h=0;
			if(l){
				l->p=this;
				v.updatel(l->v);
				chmax(h,l->h);
			}
			if(r){
				r->p=this;
				v.updater(r->v);
				chmax(h,r->h);
			}
			h++;
			p=nullptr;
			return this;
		}
		np L(np x){l=x;return upd();}
		np R(np x){r=x;return upd();}
		np LR(np x,np y){l=x;r=y;return upd();}
	};
	using np=N2*;
	const int S;
	unique_ptr<N2[]> buf;
	avl(int ss):S(ss),buf(new N2[S]){}
	int used=0;
	np nn(N v){
		assert(used<S);
		buf[used]=N2{v,0,0,0,1};
		return &buf[used++];
	}
	void emplace_new(np x,N v){
		*x=N2{v,0,0,0,1};
	}
	/*void checkvalidity(){
		rep(i,used){
			np x=&buf[i];
			if(x->p!=(np)-1){
				if(x->p){
					int j=x->p-buf.get();
					dmp2(i,j);
					assert(x->p->p!=np(-1));
					assert(x->p->l==x||x->p->r==x);
				}
			}
		}
	}*/
	//a is not null
	np push(np x){
		#ifdef AVLLAZY
		if(x->l)x->v.pushl(x->l->v);
		if(x->r)x->v.pushr(x->r->v);
		x->v.clear();
		#endif
		return x;
	}
	int hei(np x){return x?x->h:0;}
	N val(np x){return x?x->v:N();}
	int ok(int a,int b){return abs(a-b)<=1;}
	//x is already cloned
	//x needs upd
	//hei(res)==max(hei(x->l),hei(x->r))+(0 or 1)
	//O(|hei(x->r)-hei(x->r)|)
	np balance(np x){
		np a=x->l;
		np b=x->r;
		int dif=hei(a)-hei(b);
		if(inc(-1,dif,1)){
			return x->upd();
		}else if(dif<0){
			push(b);
			if(dif<-2){
				return Lb(b,Rb(x,b->l));
			}else if(ok(hei(b->l)+1,hei(b->r))){
				return b->L(x->R(b->l));
			}else{
				np y=push(b->l);
				return y->LR(x->R(y->l),b->L(y->r));
			}
		}else if(dif>0){
			push(a);
			if(dif>2){
				return Rb(a,Lb(x,a->r));
			}else if(ok(hei(a->l),hei(a->r)+1)){
				return a->R(x->L(a->r));
			}else{
				np y=push(a->r);
				return y->LR(a->R(y->l),x->L(y->r));
			}
		}else assert(false);
	}
	np Lb(np x,np a){x->l=a;return balance(x);}
	np Rb(np x,np a){x->r=a;return balance(x);}
	//hei(res)==max(hei(a),hei(b))+(0 or 1)
	//O(hei(a)+hei(b))
	//a,b は基本的にはちゃんとした木なのだが，p に変な情報が入っているかもしれないので消しておく
	//TOKI35G
	np mergedfs(np a,np b){
		if(a==0&&b==0)return nullptr;
		if(a==0){
			b->p=nullptr;
			return b;
		}
		if(b==0){
			a->p=nullptr;
			return a;
		}
		if(hei(a)<hei(b)){
			push(b);
			return Lb(b,mergedfs(a,b->l));
		}else{
			push(a);
			return Rb(a,mergedfs(a->r,b));
		}
	}
	template<class...Args>
	np merge(Args...args){
		np res=0;
		for(auto x:{args...})res=mergedfs(res,x);
		return res;
	}
	/*template <class F,class... Args> 
	pair<np,np> max_right(np x,N cur,F f,Args&&... args){
		if(x==0)return mp(np(0),np(0));
		push(x);
		x->v.single();
		N nx=cur;
		nx.updater(val(x->l));
		nx.updater(val(x));
		if((nx.*f)(forward<Args>(args)...)){
			auto [a,b]=max_right(x->r,nx,f,forward<Args>(args)...);
			return mp(Rb(x,a),b);
		}else{
			auto [a,b]=max_right(x->l,cur,f,forward<Args>(args)...);
			return mp(a,Lb(x,b));
		}
	}
	template <class F,class... Args> 
	pair<np,np> max_right(np x,F f,Args&&... args){
		return max_right(x,N(),f,forward<Args>(args)...);
	}*/
	//f(v,args)=true が左，false が右
	//not verified
	template <class F,class... Args>
	pair<np,np> split_by_cmp(np x,F f,Args&&... args){
		if(x==0)return mp(np(0),np(0));
		push(x);
		x->v.single();
		if(f(x->v,forward<Args>(args)...)){
			auto [a,b]=split_by_cmp(x->r,f,forward<Args>(args)...);
			return mp(Rb(x,a),b);
		}else{
			auto [a,b]=split_by_cmp(x->l,f,forward<Args>(args)...);
			return mp(a,Lb(x,b));
		}
	}
	//lower_bound の位置に insert
	//FHC 2022 Round3 E2 (without LAZY)
	//CF887F (insert した結果のノードも返す)
	template <class F>
	void insert_cmp(np&x,F f,np v){
		assert(!v->l&&!v->r&&!v->p);
		if(x==0){
			x=v;
			return;
		}
		push(x);
		x->v.single();
		if(f(x->v,v->v))
			insert_cmp(x->r,f,v);
		else
			insert_cmp(x->l,f,v);
		x=balance(x);
	}
	//emplace に書き換えないと
	//insert_cmp ように書き換えないと
	/*template <class F>
	np insert_by_cmp(np&x,F f,const N&v){
		auto [newx,ins]=insertdfs(x,f,v);
		x=newx;
		return ins;
	}*/
	//x のデータをこちょこちょ書き換えたので upd を反映させて root を返す
	//CF887F
	void reload(np x){
		while(x){
			np y=x->p;
			x->upd();
			x=y;
		}
	}
	static np vs[];
	//x から親までの path を vs に保存
	//ついでに push もしておく
	//CF887F
	int prep(np x){
		int s=0;
		vs[s++]=x;
		while(vs[s-1]->p){
			vs[s]=vs[s-1]->p;
			s++;
		}
		per(i,s)push(vs[i]);
		return s;
	}
	//CF887F
	//TOKI35G erase した直後に x->v を見ると正しいデータが入っている
	np erase(np x){
		int s=prep(x);
		//delete x
		x->p=(np)-1;// (p から delete 済みかどうかをチェックできるようにする)
		np cur=mergedfs(x->l,x->r);
		rng(i,1,s){
			if(vs[i]->l==vs[i-1]){
				cur=Lb(vs[i],cur);
			}else{
				cur=Rb(vs[i],cur);
			}
		}
		return cur;
	}
	//x,res は push されていて即使用可能になっている
	//CF887F
	np next(np x){
		int s=prep(x);
		if(x->r){
			np cur=x->r;
			while(cur->l){
				push(cur);
				cur=cur->l;
			}
			push(cur);
			return cur;
		}
		rng(i,1,s)if(vs[i]->l==vs[i-1])return vs[i];
		return nullptr;
	}
	//CF887F
	np prev(np x){
		int s=prep(x);
		if(x->l){
			np cur=x->l;
			while(cur->r){
				push(cur);
				cur=cur->r;
			}
			push(cur);
			return cur;
		}
		rng(i,1,s)if(vs[i]->r==vs[i-1])return vs[i];
		return nullptr;
	}
	//CF887F
	//TOKI35G
	template <class F,class... Args>
	void chroot(np x,F f,Args&&... args){
		if(!x)return;
		assert(x->p==nullptr);
		(x->v.*f)(forward<Args>(args)...);
	}
	/*//lower_bound の位置を erase
	//erase が実際に走ったかどうかが bool で返る
	//FHC 2022 Round3 E2 (without LAZY, all true)
	template <class F>
	pair<bool,np> erasedfs(np x,F f,const N&v){
		if(x==0)return mp(false,np(0));
		push(x);
		x->v.single();
		if(f(x->v,v)){
			auto [done,y]=erasedfs(x->r,f,v);
			return mp(done,Rb(x,y));
		}else{
			auto [done,y]=erasedfs(x->l,f,v);
			if(done)return mp(true,Lb(x,y));
			else return mp(true,merge(y,x->r));
		}
	}
	template <class F>
	pair<bool,np> erase_by_cmp(np x,F f,const N&v){
		return erasedfs(x,f,v);
	}
	//lower_bound が存在するかどうかを bool で返す
	//存在するならノード情報も返す
	//FHC 2022 Round3 E2 (without LAZY)
	template <class F>
	pair<bool,N> lower_bound_by_cmp(np x,F f,N v){
		np res=0;
		while(x){
			push(x);
			x->v.single();
			if(f(x->v,v)){
				x=x->r;
			}else{
				res=x;
				x=x->l;
			}
		}
		if(res)return mp(true,res->v);
		else return mp(false,N());
	}*/
	//TOKI35G
	pair<np,np> split(np x,const N&v){return split_by_cmp(x,less<N>(),v);}
	//TOKI35G
	//np insert(np&x,const N&v){return insert_by_cmp(x,less<N>(),v);}
	//UCUP 2-24-H
	void insert(np&x,np v){return insert_cmp(x,less<N>(),v);}
	//pair<bool,np> erase(np x,const N&v){return erase_by_cmp(x,less<N>(),v);}
	//pair<bool,N> lower_bound(np x,const N&v){return lower_bound_by_cmp(x,less<N>(),v);}
	/*template<class T>
	np build(const T&rw){
		auto dfs=[&](auto self,int l,int r)->np{
			if(l==r)return 0;
			int m=(l+r)/2;
			np x=nn(rw[m]);
			return x->LR(self(self,l,m),self(self,m+1,r));
		};
		return dfs(dfs,0,si(rw));
	}*/
	//CF887F
	vc<N> listup(np root){
		vc<N> res;
		auto dfs=[&](auto self,np x)->void{
			if(x==0)return;
			push(x);
			self(self,x->l);
			res.pb(x->v);
			self(self,x->r);
		};
		dfs(dfs,root);
		return res;
	}
};
template<class N>typename avl<N>::np avl<N>::vs[100];
//デフォルトコンストラクターが null node になっているべき (例えば len=0)
//reverse なし
//N::push
//副作用なし,１個の子にpush
//N::clear
//lazy tagを消去
//push/lazy は AVLLAZY がdefineされてなければ呼ばれない（これ定数倍変わる？）
//N::single
//ノード単体の情報を元に部分木情報を初期化
//N::updatel,updater
//N::ok
//max_right のノリで split をする

//CF887F
//set::set iterator の要領で各ノードから erase/next/prev が呼べるようになった
//p==(np)-1 で delete 済みかどうか確認できる
//itr を保存しておいて後で呼び出すときは prep を忘れないように
//p のリンクをどうやって管理してしまうか考えたときに,upd を利用することにした
//特に upd 直後は p==nullptr であることに注意!

//TOKI35G
//beats っぽい怪しい updateを走らせた
//ある部分木全体を更新できないとき，もちろんその下に潜るのだが，
//根のノード単体に対する更新を忘れないようにする

//UCUP 2-24-H
//emplace_new, insert などを更新
//insert_cmp も更新，balance(x) の戻り値は x とは限らないね

//ノード同士の比較関数でも split 可能(TODO)
//TODO 細かい実装の違いによる定数倍を検証
