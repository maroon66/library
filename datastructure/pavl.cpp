//#define PAVLLAZY
//S_ bytes のバッファを確保
//ARC030D
//Copy and Paste
//FHC 2022 Round3 E2
//https://qiita.com/QCFium/items/3cf26a6dc2d49ef490d7
template<class N,int S_>
struct pavl{
	struct N2{
		using np=N2*;
		N v;
		np l,r;
		int h;
		//h は葉までの最長パスのノード数
		//null node で 0
		//v has no lazy data
		np upd(){
			v.single();
			h=0;
			if(l){
				v.updatel(l->v);
				chmax(h,l->h);
			}
			if(r){
				v.updater(r->v);
				chmax(h,r->h);
			}
			h++;
			return this;
		}
		np L(np x){l=x;return upd();}
		np R(np x){r=x;return upd();}
		np LR(np x,np y){l=x;r=y;return upd();}
	};
	static constexpr int S=S_/sizeof(N2);
	N2 buf[S];
	using np=N2*;
	int fix=0,used=0;
	np nn(N v){
		assert(used<S);
		buf[used]=N2{v,0,0,1};
		return buf+used++;
	}
	//a is not null
	np clone(np&a){
		if(a-buf<fix){
			assert(used<S);
			buf[used]=*a;
			a=buf+used++;
		}
		return a;
	}
	//a is not null
	np push(np&x){
		clone(x);
		#ifdef PAVLLAZY
		if(x->l)x->v.push(clone(x->l)->v);
		if(x->r)x->v.push(clone(x->r)->v);
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
	np mergedfs(np a,np b){
		if(a==0)return b;
		if(b==0)return a;
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
		fix=used;
		np res=0;
		for(auto x:{args...})res=mergedfs(res,x);
		return res;
	}
	template <class F,class... Args> 
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
		fix=used;
		return max_right(x,N(),f,forward<Args>(args)...);
	}
	//f(v,args)=true が左，false が右
	//not verified
	/*template <class F,class... Args>
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
	}*/
	//lower_bound の位置に insert
	//FHC 2022 Round3 E2 (without LAZY)
	template <class F>
	np insertdfs(np x,F f,const N&v){
		if(x==0)return nn(v);
		push(x);
		x->v.single();
		if(f(x->v,v)){
			return Rb(x,insertdfs(x->r,f,v));
		}else{
			return Lb(x,insertdfs(x->l,f,v));
		}
	}
	template <class F>
	np insert_by_cmp(np x,F f,const N&v){
		fix=used;
		return insertdfs(x,f,v);
	}
	//lower_bound の位置を erase
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
		fix=used;
		return erasedfs(x,f,v);
	}
	//lower_bound が存在するかどうかを bool で返す
	//存在するならノード情報も返す
	//FHC 2022 Round3 E2 (without LAZY)
	template <class F>
	pair<bool,N> lower_bound_by_cmp(np x,F f,N v){
		fix=used;
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
		used=fix;
		if(res)return mp(true,res->v);
		else return mp(false,N());
	}
	np insert(np x,const N&v){return insert_by_cmp(x,less<N>(),v);}
	pair<bool,np> erase(np x,const N&v){return erase_by_cmp(x,less<N>(),v);}
	pair<bool,N> lower_bound(np x,const N&v){return lower_bound_by_cmp(x,less<N>(),v);}
	void clear(){fix=used=0;}
	template<class T>
	np build(const T&rw){
		fix=used;
		auto dfs=[&](auto self,int l,int r)->np{
			if(l==r)return 0;
			int m=(l+r)/2;
			np x=nn(rw[m]);
			return x->LR(self(self,l,m),self(self,m+1,r));
		};
		return dfs(dfs,0,si(rw));
	}
	vc<N> listup(np root){
		fix=used;
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
	void rebuild_if_needed(np&x,int maxlen){
		if(used+1000+maxlen>S){
			auto ls=listup(x);
			clear();
			x=build(ls);
		}
	}
};
//デフォルトコンストラクターが null node になっているべき (例えば len=0)
//reverse なし
//N::push
//副作用なし,１個の子にpush
//N::clear
//lazy tagを消去
//push/lazy は PAVLAZY がdefineされてなければ呼ばれない（これ定数倍変わる？）
//N::single
//ノード単体の情報を元に部分木情報を初期化
//N::updatel,updater
//N::ok
//max_right のノリで split をする
//ノード同士の比較関数でも split 可能(TODO)
//TODO 細かい実装の違いによる定数倍を検証

//Copy and Paste
template<class t>
struct SingleElement{
	t val;
	int len;
	SingleElement():val(0),len(0){}
	SingleElement(t v):val(v),len(1){}
	void single(){
		len=1;
	}
	void upd(const SingleElement&x){
		len+=x.len;
	}
	void updatel(const SingleElement&x){upd(x);}
	void updater(const SingleElement&x){upd(x);}
	bool ok_len(int x){
		return len<=x;
	}
};
//using N=SingleElement<char>;

//range add,sum
//ARC030D
struct N{
	int len;
	ll val,sum,lz;
	N():len(0),val(0),sum(0),lz(0){}
	N(ll v):len(1),val(v),sum(v),lz(0){}
	void add(ll v){
		lz+=v;
		val+=v;
		sum+=len*v;
	}
	void push(N&x){
		x.add(lz);
	}
	void clear(){
		lz=0;
	}
	void single(){
		assert(lz==0);
		len=1;
		sum=val;
	}
	void upd(const N&x){
		assert(lz==0);
		len+=x.len;
		sum+=x.sum;
	}
	void updatel(const N&x){
		upd(x);
	}
	void updater(const N&x){
		upd(x);
	}
	bool ok_len(int x){
		return len<=x;
	}
};

//FHC 2022 Round3 E2
template<class t>
struct SortedElement{
	t val;
	SortedElement(t v=t()):val(v){}
	void single(){}
	void updatel(const SortedElement&){}
	void updater(const SortedElement&){}
	bool operator<(const SortedElement&r)const{return val<r.val;}
};

constexpr int memmax=5*ten(8);
pavl<N,memmax> z;
using np=decltype(z)::np;

//https://stackoverflow.com/a/49091681
template<class Tuple>
auto tuple_pop_front(Tuple tuple) {
	return apply([](auto a,auto...rest){return mp(a,mt(rest...));},tuple);
}
tuple<np> cut_by_index(np a){
	return a;
}
template<class...Args>
auto cut_by_index(np x,int pos,Args...args){
	assert((!x&&pos==0)||inc(0,pos,x->v.len));
	auto [ab,c]=tuple_pop_front(cut_by_index(x,args...));
	return tuple_cat(z.max_right(ab,&N::ok_len,pos),c);
}
