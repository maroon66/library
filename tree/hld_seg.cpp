//Codechef 2021 August Lunchtime Bunny Hops
//MHC2023 Practice D
//UCUP 3-8-J
template<class E,class N>
struct HLD_seg{
	HLD<E>*hld;
	segtree<N> seg;
	vc<N> acc;
	HLD_seg():hld(nullptr){}
	template<class t>
	HLD_seg(HLD<E>&hh,const vc<t>&rw){init(hh,rw);}
	template<class t>
	void init(HLD<E>&hh,const vc<t>&rw){
		hld=&hh;
		seg=segtree<N>(perm(rw));
		acc.resize(si(rw));
		for(auto i:hld->ni){
			acc[i]=rw[i];
			if(hld->head[i]!=i)
				acc[i]=N::merge(acc[hld->par[i]],acc[i]);
		}
	}
	template<class t>
	vc<t> perm(const vc<t>&rw){
		int n=hld->n;
		vc<t> res(n);
		rep(i,n)res[hld->in[i]]=rw[i];
		return res;
	}
	void set(int i,const N&x){
		seg.point_set(hld->in[i],x);
	}
	//assume a is desdendant of b
	//ex=true <=> exclude b
	N subpath(int a,int b,bool ex){
		N res;
		while(1){
			if(hld->head[a]==hld->head[b]){
				return N::merge(res,seg.composite(hld->in[b]+ex,hld->in[a]+1));
			}else{
				int h=hld->head[a];
				res=N::merge(res,seg.composite(hld->in[h],hld->in[a]+1));
				a=hld->par[h];
			}
		}
	}
	//N が可環とかでないと全て壊れる
	N get(int a,int b,bool ex){
		int c=hld->lca(a,b);
		return N::merge(subpath(a,c,ex),subpath(b,c,true));
	}
	//a-root (CF906F)
	N get(int a){
		return subpath(a,hld->rt,false);
	}
	N getall(){
		return seg.getall();
	}
	//UCUP 2-22-K
	template <class F,class... Args> 
	pair<int,N> find_lowest_false(int v,F f,Args&&... args){
		//v より strict に下なノードの sum
		N w;
		while(v!=-1){
			N tmp=N::merge(acc[v],w);
			if((tmp.*f)(forward<Args>(args)...)){
				w=tmp;
				v=hld->par[hld->head[v]];
			}else{
				auto [i,z]=seg.min_left_withinit(hld->in[v]+1,w,f,forward<Args>(args)...);
				assert(hld->in[hld->head[v]]<i);
				return mp(hld->ni[i-1],z);
			}
		}
		return mp(v,w);
	}
};

//path add/sum が Codechef 2022 July Lunchtime F にある
//https://www.codechef.com/viewsolution/69254089

//Yosupo: Global Minimum Cut of Dynamic Star Augmented Graph
template<class E,class N>
struct HLD_seglazy{
	HLD<E>&hld;
	seglazy<N> seg;
	//ctor に渡す rw は素直に渡す
	//勝手に行きがけ順に並び替えてくれる
	template<class t>
	HLD_seglazy(HLD<E>&hh,const vc<t>&rw):hld(hh),seg(perm(rw)){}
	template<class t>
	vc<t> perm(const vc<t>&rw){
		int n=hld.n;
		vc<t> res(n);
		rep(i,n)res[hld.in[i]]=rw[i];
		return res;
	}
	N point_get(int i){
		return seg.point_get(hld.in[i]);
	}
	void point_set(int i,const N&x){
		seg.point_set(hld.in[i],x);
	}
	N getall(){
		return seg.getall();
	}
	//a-root
	template<class F,class...Args>
	void ch_root(int a,F f,Args&&...args){
		hld.subpath_work(a,hld.rt,false,[&](int l,int r){
			seg.ch(l,r,f,forward<Args>(args)...);
		});
	}
	template<class F,class...Args>
	void ch_path(int a,int b,bool ex,F f,Args&&...args){
		int c=hld.lca(a,b);
		hld.subpath_work(a,c,ex,[&](int l,int r){
			seg.ch(l,r,f,forward<Args>(args)...);
		});
		hld.subpath_work(b,c,true,[&](int l,int r){
			seg.ch(l,r,f,forward<Args>(args)...);
		});
	}
};

//Online TreeDP
//With Lazy Propagation
//Multiuni2023-2 M
template<class E,class N,class M>
struct HLD_Online_TreeDP{
	const HLD<E>&hld;
	const int n;
	vi lid;
	vc<seglazy<N>> seg;
	vc<segtree<M>> sub;
	template<class T>
	HLD_Online_TreeDP(HLD<E>&hh,const vc<T>&rw):hld(hh),n(hld.n),lid(n,-1),seg(n),sub(n){
		rep(v,n)if(hld.head[v]==v){
			int s=hld.hei[v];
			vc<T> ls(s);
			rep(i,s)ls[i]=rw[hld.adv(v,i)];
			seg[v]=seglazy<N>(ls);
		}
		for(auto v:revv(hld.ni)){
			int s=si(hld.g[v])-1;
			if(s>=1){
				vc<M> ls(s);
				rep(i,s){
					int to=hld.g[v][i+1];
					lid[to]=i;
					ls[i]=seg[to].getall().gen_light();
				}
				sub[v]=segtree<M>(ls);
				upd_light(v);
			}
		}
	}
	pi getij(int v){
		int h=hld.head[v];
		return pi(h,hld.dep[v]-hld.dep[h]);
	}
	void upd_light(int v){
		auto [i,j]=getij(v);
		seg[i].point_change(j,&N::upd_light,sub[v].getall());
	}
	//assume a is desdendant of b
	//ex=true <=> exclude b
	template<class F,class... Args>
	void subpath(int a,int b,bool ex,F f,Args&&... args){
		while(a>=0){
			if(hld.dep[a]>=hld.dep[b]){
				auto [h,r]=getij(a);
				r++;
				int l=0;
				if(hld.head[a]==hld.head[b]){
					l=getij(b).b;
					if(ex)l++;
				}
				seg[h].ch(l,r,f,forward<Args>(args)...);
			}
			a=hld.head[a];
			int p=hld.par[a];
			if(p!=-1){
				sub[p].point_set(lid[a],seg[a].getall().gen_light());
				upd_light(p);
			}
			a=p;
		}
	}
	template<class F,class... Args>
	void ch(int a,int b,F f,Args&&... args){
		int c=hld.lca(a,b);
		subpath(a,c,false,f,forward<Args>(args)...);
		subpath(b,c,true,f,forward<Args>(args)...);
	}
	N get_subtree(int v){
		auto [i,j]=getij(v);
		return seg[i].composite(j,seg[i].n);
	}
};
//M: 木DPみたいなやつ
//頂点 x から伸びる light subtree の集合をデータに持つ
//(M 自身は x のデータを持ってない)
//merge subtree を並べる
//N: hld の heavy-path + そこから伸びる light child の部分木すべて みたいなノード
//constructor N() 空ノード
//push segtree で push
//merge(x,y) heavy path のマージ x が親側
//gen_light この heavy path を struct M に変換する
//upd_light (segtree の leaf に対応するノードでのみ呼ばれる)
//自分(1頂点)-m という木を考えて N を初期化する

//UCUP 2-24-H
//CF CodeTon 8-H
//segtree<M> をより一般の形にしたバージョン
