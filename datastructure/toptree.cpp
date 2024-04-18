//Ofast,unroll-loops でちょい速くなる

//VERIFY:
//spaceships
//do_use_segment_tree
//CF679D
//CF564E
//TTPC2019M

//splay tree の左側が根の方のノードに対応している
//頂点と辺がそれぞれノードを持つ
//light edge だけ特別扱いしている
//それ以外のやつは，普通の頂点倍加 LCT のような感じ (c[0],c[1])
//頂点由来のノードであれば，c[2] に light edges の情報が入る
//light edge の部分のやつは，light edge 内の splay tree をつくる (c[0],c[2])
//ただし c[1] にはぶら下がるパス部分の情報が入る
template<class N>
struct toptree{
	struct node{
		//t==0 vertex
		//t==1 path edge
		//t==2 light edge
		N d;
		bool rev;
		int c[3],p;
		node(char tt=-1):rev(false),c{-1,-1,-1},p(-1){d.t=tt;}
		void show(){
			cerr<<int(d.t)<<" "<<"("<<c[0]<<","<<c[1]<<","<<c[2]<<") "<<p<<endl;
		}
	};
	using np=node*;
	int n;
	vc<node> x;
	//unused に入ってるノードの情報は clean とは限らない
	vi unused;
	toptree(int nn):n(nn),x(n*2-1){
		gnr(i,n,n*2-1)unused.pb(i);
	}
	//reverse が呼ばれるのは path node のみ
	//light edge なら rev は常に false
	inline void reverse(int v){
		assert(v!=-1);
		assert(x[v].d.t<2);
		x[v].rev^=1;
		x[v].d.reverse();
		swap(x[v].c[0],x[v].c[1]);
	}
	//v は valid
	//link は張り替えるが update はしない
	inline void set_child(int v,int k,int c){
		assert(v!=-1);
		assert(inc(0,k,2));
		x[v].c[k]=c;
		if(c!=-1)x[c].p=v;
	}
	inline void set_light_left(int v,int c){set_child(v,0,c);}
	inline void set_light_right(int v,int c){set_child(v,2,c);}
	inline void set_path_left(int v,int c){set_child(v,0,c);}
	inline void set_path_right(int v,int c){set_child(v,1,c);}
	//v,c は valid
	//link は張り替えるが update はしない
	inline void insert_light(int v,int c){
		assert(v!=-1);
		assert(x[v].d.t==0);
		assert(c!=-1);
		assert(x[c].d.t==2);
		set_light_right(c,x[v].c[2]);
		x[v].c[2]=c;
		x[c].p=v;
	}
	inline void make_path_vertex(int v){assert(inc(0,v,n-1));x[v].d.t=0;}
	inline void make_path_edge(int v){assert(inc(n,v,n*2-2));x[v].d.t=1;}
	inline void make_light(int v){assert(inc(n,v,n*2-2));assert(!x[v].rev);x[v].d.t=2;}
	void update(int v){
		assert(v!=-1);
		assert(!x[v].rev);
		x[v].d.single();
		if(x[v].d.t<2){
			if(x[v].c[2]!=-1)x[v].d.update_light(x[x[v].c[2]].d);
			rep(k,2)if(x[v].c[k]!=-1)x[v].d.update_path(k,x[x[v].c[k]].d);
		}else if(x[v].d.t==2){
			assert(x[v].c[1]!=-1);
			x[v].d.update_path(1,x[x[v].c[1]].d);
			for(int k=0;k<3;k+=2)if(x[v].c[k]!=-1)x[v].d.update_light(x[x[v].c[k]].d);
		}else{
			assert(false);
		}
	}
	void downdate(int v){
		assert(v!=-1);
		if(x[v].d.t<2){
			if(x[v].rev){
				x[v].rev=false;
				rep(k,2)if(x[v].c[k]!=-1)reverse(x[v].c[k]);
			}
			if(x[v].c[2]!=-1)x[v].d.push_light(x[x[v].c[2]].d);
			rep(k,2)if(x[v].c[k]!=-1)x[v].d.push_path(k,x[x[v].c[k]].d);
		}else if(x[v].d.t==2){
			assert(!x[v].rev);
			assert(x[v].c[1]!=-1);
			x[v].d.push_path(1,x[x[v].c[1]].d);
			for(int k=0;k<3;k+=2)if(x[v].c[k]!=-1)x[v].d.push_light(x[x[v].c[k]].d);
		}else{
			assert(false);
		}
		x[v].d.clear_lazy();
	}
	//0,z の辺を使って rotate
	void rotate(int v,int z){
		assert(v!=-1);
		int p=x[v].p,c;
		assert(p!=-1);
		if(x[p].c[0]==v){
			c=x[v].c[z];
			x[v].c[z]=p;
			x[p].c[0]=c;
		}else{
			c=x[v].c[0];
			x[v].c[0]=p;
			x[p].c[z]=c;
		}
		if(c!=-1)x[c].p=p;
		x[v].p=x[p].p;
		x[p].p=v;
		if(x[v].p!=-1){
			rep(k,3)if(x[x[v].p].c[k]==p)
				x[x[v].p].c[k]=v;
		}
		update(p);
		//cerr<<"After Rotate "<<v<<endl;
		//show();
	}
	//void rotate_path(int v){rotate(v,1);}
	//void rotate_light(int v){rotate(v,2);}
	//親を辿れるだけたどり，downdate する
	//このあと何らかの操作で頂点 v を上に持ってこないと，計算量が崩壊する
	void prepare(int v){
		assert(v!=-1);
		if(x[v].p!=-1)prepare(x[v].p);
		downdate(v);
	}
	//prepare されていることが前提
	//path/light 内で回せるだけ回す
	//違う種類の親との間のリンクに注意しようね
	void splay(int v){
		assert(v!=-1);
		int z=x[v].d.t<2?1:2;
		while(1){
			int p=x[v].p;
			if(p==-1||(x[p].d.t<2)^(x[v].d.t<2))break;
			int q=x[p].p;
			if(q==-1||(x[q].d.t<2)^(x[v].d.t<2)){
				rotate(v,z);
			}else{
				if((x[p].c[0]==v&&x[q].c[0]==p)||(x[p].c[z]==v&&x[q].c[z]==p))
					rotate(p,z);
				else
					rotate(v,z);
				rotate(v,z);
			}
		}
		update(v);
	}
	int go_deep(int v,int k){
		assert(v!=-1);
		while(1){
			downdate(v);
			if(x[v].c[k]!=-1)v=x[v].c[k];
			else return v;
		}
		assert(false);
	}
	void expose(int v){
		assert(v!=-1);
		prepare(v);
		for(int a=v;a!=-1;a=x[a].p)
			splay(a);
		for(int a=v;a!=-1;a=x[a].p){
			if(x[a].d.t==2){
				int b=x[a].p;
				assert(b!=-1);
				if(x[b].c[1]==-1){
					int l=x[a].c[0],r=x[a].c[2];
					if(l!=-1){
						x[l].p=-1;
						l=go_deep(l,2);
						splay(l);
						set_light_right(l,r);
						update(l);
					}else{
						l=r;
					}
					set_light_right(b,l);
				}else{
					int c=x[b].c[1];
					x[c].p=-1;
					c=go_deep(c,0);
					splay(c);
					assert(!x[c].rev);
					make_light(c);
					set_light_left(c,x[a].c[0]);
					set_light_right(c,x[a].c[2]);
					set_light_right(b,c);
					//b,c は子ノードの情報を反映していない
					update(c);
					//c は子ノードの情報を反映した
				}
				x[a].c[0]=x[a].c[2]=-1;
				make_path_edge(a);
				set_path_right(b,a);
			}
		}
		//cerr<<"Before Splay "<<v<<endl;
		//show();
		splay(v);
		//cerr<<"After Expose "<<v<<endl;
		//show();
	}
	template<class...Args>
	void init_vertex(int v,Args&&...args){
		assert(inc(0,v,n-1));
		x[v]=node(0);
		x[v].d.init_vertex(v,forward<Args>(args)...);
	}
	template<class...Args>
	void init_edge(int v,char t,int a,int b,Args&&...args){
		x[v]=node(t);
		x[v].d.init_edge(a,b,forward<Args>(args)...);
	}
	void evert(int v){
		assert(inc(0,v,n-1));
		assert(x[v].d.t==0);
		expose(v);
		if(x[v].c[0]!=-1){
			int c=x[v].c[0];
			x[v].c[0]=-1;
			x[c].p=-1;
			c=go_deep(c,1);
			splay(c);
			reverse(c);
			assert(x[c].rev);
			x[c].rev=false;
			reverse(x[c].c[1]);
			make_light(c);
			insert_light(v,c);
			//v,c は子ノードの情報を反映していない
			update(c);
			update(v);
			//v,c は子ノードの情報を反映した
		}
	}
	//a と b を辺で結び (a が親側)，辺の index を返す
	//もしもともと連結であれば，-1 を返す
	template<class...Args>
	int link(int a,int b,Args&&...args){
		assert(inc(0,a,n-1));
		assert(inc(0,b,n-1));
		if(a==b)return -1;
		expose(a);
		evert(b);
		if(x[a].p==-1){
			assert(si(unused));
			int e=unused.back();unused.pop_back();
			init_edge(e,2,a,b,forward<Args>(args)...);
			set_path_right(e,b);
			insert_light(a,e);
			//a,e は子ノードの情報を反映していない
			update(e);
			update(a);
			//a,e は子ノードの情報を反映した
			return e;
		}else{
			return -1;
		}
	}
	//cut したあとの木のノードのペア(親，子)を返す
	//頂点番号じゃない
	pi cut(int e){
		assert(inc(n,e,n*2-2));
		expose(e);
		pi res(x[e].c[0],x[e].c[1]);
		if(res.a!=-1)x[res.a].p=-1;
		if(res.b!=-1)x[res.b].p=-1;
		x[e].d.t=-1;
		unused.pb(e);
		return res;
	}
	int lca(int a,int b){
		assert(inc(0,a,n-1));
		assert(inc(0,b,n-1));
		if(a==b)return a;
		expose(a);
		expose(b);
		if(x[a].p==-1)return -1;
		int d=a;
		while(a!=b){
			int p=x[a].p;
			if(x[a].d.t==2&&x[p].d.t<2)d=p;
			else if(p==b&&x[p].c[1]==a)d=b;
			a=p;
		}
		assert(inc(0,d,n-1));
		return d;
	}
	int same(int a,int b){
		assert(inc(0,a,n-1));
		assert(inc(0,b,n-1));
		if(a==b)return true;
		expose(a);
		expose(b);
		return x[a].p!=-1;
	}
	void expose_path(int a,int b){
		assert(inc(0,a,n-1));
		assert(inc(0,b,n-1));
		evert(a);
		expose(b);
		int c=x[b].c[1];
		if(c!=-1){
			x[b].c[1]=-1;
			x[c].p=-1;
			c=go_deep(c,0);
			splay(c);
			assert(!x[c].rev);
			make_light(c);
			insert_light(b,c);
			update(c);
			update(b);
		}
	}
	//XXI Opencup GP of Korea A
	template<class F,class... Args>
	void subtree_update(int r,int v,F f,Args&&... args){
		assert(inc(0,r,n-1));
		assert(inc(0,v,n-1));
		evert(r);
		expose(v);
		int tmp=-1;
		assert(!x[v].rev);
		swap(x[v].c[0],tmp);
		(x[v].d.*f)(forward<Args>(args)...);
		downdate(v);
		swap(x[v].c[0],tmp);
		update(v);
	}
	//XXI Opencup GP of Korea A
	template<class F,class... Args>
	void path_update(int a,int b,F f,Args&&... args){
		expose_path(a,b);
		(x[b].d.*f)(forward<Args>(args)...);
	}
	N&operator[](int i){return x[i].d;}
	void show(){
		rep(i,n*2-1)if(x[i].d.t!=-1){
			cerr<<i<<" ";
			x[i].show();
		}
	}
	//init_vertex は終了しているとする
	//E.idx が必要．これに従って辺に id を振る
	template<class H> void init_from_hld(const H&hld){
		unused.clear();
		auto subtree=[&](auto subtree_self,int root)->int{
			auto path=[&](auto path_self,int l,int r)->int{
				if(r<l)return -1;
				const int m=(l+r)/2;
				int v=hld.ni[(m+1)/2],z;
				if(m%2==0){
					z=v;
					auto light=[&](auto light_self,int a,int b)->int{
						if(b<a)return -1;
						const int c=(a+b)/2;
						int u=hld.g[v][c],w=hld.pe[u].idx+n;
						init_edge(w,2,v,u,hld.pe[u]);
						set_path_right(w,subtree_self(subtree_self,u));
						set_light_left(w,light_self(light_self,a,c-1));
						set_light_right(w,light_self(light_self,c+1,b));
						update(w);
						return w;
					};
					set_light_right(z,light(light,1,si(hld.g[v])-1));
				}else{
					z=hld.pe[v].idx+n;
					init_edge(z,1,hld.par[v],v,hld.pe[v]);
				}
				set_path_left(z,path_self(path_self,l,m-1));
				set_path_right(z,path_self(path_self,m+1,r));
				update(z);
				return z;
			};
			return path(path,hld.in[root]*2,(hld.in[root]+hld.hei[root]-1)*2);
		};
		subtree(subtree,hld.rt);
	}
};

struct E{
	int to,idx;
	operator int()const{return to;}
};
//init_from_hld(HLD<E>(t,0)) とかにつかう

//まず rep(i,n)t.init_vertex(i) を最初にやる
//t[i].init_vertex(i) などとしないように
//cut の戻り値の頂点は expose されているものとする．
//link,lca,same についてはその限りでない！

//init_vertex(v,...): 頂点に対応するノードを初期化する
//init_edge(a,b,...): 辺に対応するノードを初期化する，a が親側
//single(): そのノード単体だけの情報に更新する(部分木の情報を忘れる)
//reverse(): 反転
//update_path(k,N): path 上で，(k==0?左:右)に path ノードを追加する
//=compress
//update_light: light ツリー上でノードを追加する（原理的に左右の区別がない）
//=rake
//これは，頂点単体のノードにその部分木の情報を伝える際にも呼ばれる
//これで実用上は困らないはずだけど，もしそうでなければ，ごめん
//push_path(k,N), push_light(N): push する，update と同様
//push_path パスではあるが，部分木情報も push することを忘れずに
//パス情報と部分木情報が両方あるときに忘れそう（忘れた）
//clear_lazy(): lazy 系のデータを clear する 

//木が固定されている場合
//ノードのマージされ方が限定され，実装が楽になるケースがある
//vertex は, light -> path の順でマージしてる
//light edge は，path -> light の順でマージしてる
//木が固定されてる必要，本当にあるのか？

//t がノードの状態を表す変数で，絶対必要
//これをうっかり書き換えるなどのミスが予想される
//t==0 vertex
//t==1 path edge
//t==2 light edge

//Codechef January Long 2021
//頂点に値
//木全体の max
//木のサイズもわかる
//change_rw のあとは update 呼ぼうね
struct N{
	char t;
	int rw,mx,s;
	void init_vertex(int){
		rw=-1;
		single();
	}
	void init_edge(int,int){
		rw=-1;
		single();
	}
	void init_edge(int a,int b,const E&){
		init_edge(a,b);
	}
	void single(){
		mx=rw;
		if(t==0)s=1;
		else s=0;
	}
	void reverse(){}
	void update_path(int,const N&x){
		chmax(mx,x.mx);
		s+=x.s;
	}
	void update_light(const N&x){
		chmax(mx,x.mx);
		s+=x.s;
	}
	void push_path(int,N&){}
	void push_light(N&){}
	void clear_lazy(){}
	
	void change_rw(int v){
		rw=v;
	}
};

//CF691F
//activeな頂点への距離の総和を求めるやつ
//木が static だと思いこんでいる
//add のあと update しないとね
struct N{
	char t;
	int rw,cnt,len[2];
	ll ans,s[2];
	void init_vertex(int){
		rw=0;
		single();
	}
	void init_edge(int,int){
		rw=0;
		single();
	}
	void init_edge(int a,int b,const E&){
		init_edge(a,b);
	}
	void single(){
		if(t==0){
			cnt=rw;
			len[0]=len[1]=0;
			ans=0;
			s[0]=s[1]=0;
		}
		else{
			cnt=rw;
			len[0]=len[1]=1;
			ans=0;
			s[0]=s[1]=cnt;
		}
	}
	void reverse(){assert(false);}
	void update_path(int dir,const N&x_){
		N x[2]{x_,*this};
		if(dir)swap(x[0],x[1]);
		cnt=x[0].cnt+x[1].cnt;
		rep(k,2)len[k]=x[k].len[k];
		len[dir]+=x[0].len[1]+x[1].len[0];
		ans+=x[dir].s[dir^1]+ll(x[dir^1].len[dir])*x[dir].cnt;
		rep(k,2)s[k]=x[k].s[k]+x[k^1].s[k]+ll(x[k].len[0]+x[k].len[1])*x[k^1].cnt;
	}
	void update_light(const N& x){
		cnt+=x.cnt;
		ans+=x.s[0]+ll(len[0])*x.cnt;
		s[0]+=x.s[0];
		s[1]+=x.s[0]+ll(len[0]+len[1])*x.cnt;
	}
	void push_path(int,N&){
	}
	void push_light(N&){
	}
	void clear_lazy(){
	}
	
	void add(int v){
		rw+=v;
	}
};

//TTPC2019M
//active な頂点数カウント + 部分木全体に add
struct N{
	char t;
	bool act;
	ll inv;
	int actsum;
	ll lz;
	//t==0
	//act,inv,actsum,lz
	//t==1
	//actsum,lz
	//t==2
	//actsum,lz
	void init_vertex(int){
		act=false;
		inv=0;
		single();
	}
	void init_edge(int,int){
		single();
	}
	void init_edge(int a,int b,const E&){
		init_edge(a,b);
	}
	void single(){
		if(t==0){
			actsum=act;
			lz=0;
		}
		else{
			actsum=0;
			lz=0;
		}
	}
	void reverse(){}
	void update_path(int,const N&x){
		actsum+=x.actsum;
	}
	void update_light(const N& x){
		actsum+=x.actsum;
	}
	void push_path(int,N&x){
		x.add_lazy(lz);
	}
	void push_light(N&x){
		x.add_lazy(lz);
	}
	void clear_lazy(){
		lz=0;
	}
	
	void add_lazy(ll v){
		inv+=v;
		lz+=v;
	}
};

//UOJ Long Round 1 C


//active な頂点間の距離の最大を返す
struct N{
	char t;
	bool act;
	int p,len[2],mx;
	void init_vertex(int){
		act=false;
		single();
	}
	void init_edge(int,int){
		single();
	}
	void init_edge(int a,int b,const E&){
		init_edge(a,b);
	}
	void single(){
		if(t==0){
			p=0;
			if(act){
				len[0]=len[1]=0;
			}else{
				len[0]=len[1]=-inf;
			}
		}else{
			p=1;
			len[0]=len[1]=-inf;
		}
		mx=0;
	}
	void reverse(){assert(false);}
	void update_path(int dir,const N&x_){
		N x[2]{x_,*this};
		if(dir)swap(x[0],x[1]);
		p=x[0].p+x[1].p;
		rep(k,2)len[k]=max(x[k].len[k],x[k].p+x[k^1].len[k]);
		mx=max({x[0].mx,x[1].mx,x[0].len[1]+x[1].len[0]});
	}
	void update_light(const N&x){
		chmax(mx,x.mx);
		chmax(mx,len[0]+x.len[0]);
		chmax(len[0],x.len[0]);
		chmax(len[1],p+x.len[0]);
	}
	void push_path(int,N&){}
	void push_light(N&){}
	void clear_lazy(){}
	
	void change_act(bool v){
		act=v;
	}
};
//usage
t.expose(i);
t[i].change_act(true);
t.update(i);
t[i].mx

//頂点 x から最も遠い頂点までの距離がわかる
//頂点数もわかる
//Osijek 2023 Day5 F
struct N{
	char t;
	int a,b,c,s;
	void init_vertex(int){
		single();
	}
	void init_edge(int,int){
		single();
	}
	void single(){
		if(t==0){
			a=b=c=0;
			s=1;
		}
		else{
			a=b=c=1;
			s=0;
		}
	}
	void reverse(){swap(c,b);}
	static N merge(const N&x,const N&y,char t){
		N res;
		res.t=t;
		res.a=x.a+y.a;
		res.b=max(x.b,x.a+y.b);
		res.c=max(x.c+y.a,y.c);
		res.s=x.s+y.s;
		return res;
	}
	void update_path(int k,const N&x){
		*this=k==0?merge(x,*this,t):merge(*this,x,t);
	}
	void update_light(const N&x){
		chmax(b,x.b);
		chmax(c,a+x.b);
		s+=x.s;
	}
	void push_path(int,N&){}
	void push_light(N&){}
	void clear_lazy(){}
};
//usage
t.evert(x);
int d=t[x].b;
t[x].s
