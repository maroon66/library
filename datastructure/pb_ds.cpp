#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

//not a multiset
//find_by_order(k) -> itr of k-th(0-based) element
//order_of_key(k) -> index of lower_bound(k)
template<class t>
using ordered_set=tree<
t,
null_type,
less<t>,
rb_tree_tag,
tree_order_statistics_node_update>;

//https://codeforces.com/blog/entry/62393
struct custom_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
	
	size_t operator()(pair<int,int> x)const{
		return operator()(uint64_t(x.first)<<32|x.second);
	}
};

template<class t,class u>
using hash_table=gp_hash_table<t,u,custom_hash>;

//count が使えない
//find を，しようね！
//move コンストラクタがないのでマージテクで壊れる バカ
//value を null_type にすることで unordered_set が実現できる
//insert などの関数が存在する (ドキュメント，どこ？)
//https://gcc.gnu.org/onlinedocs/libstdc++/manual/policy_data_structures.html (長すぎ，HELP)
//要素へのポインターが rehash の際に破壊される
//普通の unordered_map だと破壊されない
ull toull(int x,int y){
	return ull(x)<<32|(uint(y));
}

template<class key,class N>
struct SuperMap_Type{
	template<
		typename Citr,
		typename Itr,
		typename Cmp,
		typename Alloc>
	struct Upd{
		virtual Citr node_begin()const=0;
		virtual Citr node_end()const=0;
		virtual Itr node_begin()=0;
		virtual Itr node_end()=0;
		virtual ~Upd(){}
		using metadata_type=N;
		void operator()(Itr itr,Citr none){
			Itr l=itr.get_l_child(),r=itr.get_r_child();
			N res=(*itr)->second;
			if(l!=none)res=N::merge(l.get_metadata(),res);
			if(r!=none)res=N::merge(res,r.get_metadata());
			const_cast<N&>(itr.get_metadata())=res;
		}
		N composite(key l,key r)const{
			Citr itr=node_begin();
			Citr none=node_end();
			while(itr!=none){
				if(Cmp()((*itr)->first,l)){//go right
					itr=itr.get_r_child();
				}else if(Cmp()((*itr)->first,r)){
					N res=(*itr)->second;
					{//left part
						auto cur=itr.get_l_child();
						while(cur!=none){
							if(Cmp()((*cur)->first,l)){//go right
								cur=cur.get_r_child();
							}else{//go left
								auto z=cur.get_r_child();
								if(z!=none)res=N::merge(z.get_metadata(),res);
								res=N::merge((*cur)->second,res);
								cur=cur.get_l_child();
							}
						}
					}
					{//right part
						auto cur=itr.get_r_child();
						while(cur!=none){
							if(Cmp()((*cur)->first,r)){//go right
								auto z=cur.get_l_child();
								if(z!=none)res=N::merge(res,z.get_metadata());
								res=N::merge(res,(*cur)->second);
								cur=cur.get_r_child();
							}else{//go left
								cur=cur.get_l_child();
							}
						}
					}
					return res;
				}else{//go left
					itr=itr.get_l_child();
				}
			}
			return N();
		}
		void dfs_update(key x,Itr cur,Citr none){
			if(cur==none)return;
			if(Cmp()(x,(*cur)->first)){
				dfs_update(x,cur.get_l_child(),none);
			}else if(Cmp()((*cur)->first,x)){
				dfs_update(x,cur.get_r_child(),none);
			}
			operator()(cur,none);
		}
		void update(key x){
			dfs_update(x,node_begin(),node_end());
		}
	};
	using type=tree<key,N,less<key>,rb_tree_tag,Upd>;
};
template<class key,class N>
using supermap=typename SuperMap_Type<key,N>::type;

//Example
struct SumNode{
	int v,c;
	SumNode(int vv=0,int cc=0):v(vv),c(cc){}
	static SumNode merge(const SumNode&a,const SumNode&b){
		SumNode res;
		res.v=a.v+b.v;
		res.c=a.c+b.c;
		return res;
	}
	void meld(int a,int b){
		v+=a*b;
		c+=b;
	}
};
supermap<int,SumNode> z;
z[v].meld(v,c);
z.update(v);
//単純に z[v] にアクセスするとデフォルトコンストラクタで作成された N で update が走る
//なので明示的に update 関数を呼んだわけなんだけど，このせいで余計な update が走ると思われる
