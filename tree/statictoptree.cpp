//俺のライブラリの定数倍カス過ぎる
//yosupo に頼ろう
//https://atcoder.jp/contests/abc351/submissions/52958214
//C++20 くらいないと動きません
//C++17 ように書き換えたものも作ったがそっちのほうが微小に遅い
template <class TreeDP> struct StaticTopTree {
	using Path = TreeDP::Path;
	using Point = TreeDP::Point;

	int n;
	TreeDP& dp;

	template<class E>
	StaticTopTree(const vvc<E>&torg, TreeDP& _dp,int r=0)
		: n(si(torg)),
		  dp(_dp),
		  points(n + 1, dp.id()),
		  node_ids(n, {n, -1, -1}) {
		
		vvc<int> tree(n);
		vc<int> topo;
		{
			auto dfs=[&](auto self,int v,int p)->void{
				topo.pb(v);
				for(auto to:torg[v])if(to!=p){
					tree[v].pb(to);
					self(self,to,v);
				}
			};
			dfs(dfs,r,-1);
		}

		vc<int> heavy_child(n, -1);
		vc<ull> mask(n);

		vc<Node<Path>> b;
		b.reserve(n);
		auto build_compress = [&](int u) {
			b.clear();
			auto merge_last = [&]() {
				auto y = b.back();
				b.pop_back();
				auto x = b.back();
				b.pop_back();
				b.push_back(merge(x, y, compressed, [&](auto lhs, auto rhs) {
					return dp.compress(lhs, rhs);
				}));
			};
			while (u != -1) {
				b.push_back({countr_zero(bit_ceil(mask[u])),
							 dp.add_vertex(points[u], u), &node_ids[u].c_id});
				while (true) {
					int len = int(b.size());
					if (len >= 3 && (b[len - 3].d == b[len - 2].d ||
									 b[len - 3].d <= b[len - 1].d)) {
						auto last = b.back();
						b.pop_back();
						merge_last();
						b.push_back(last);
					} else if (len >= 2 && b[len - 2].d <= b[len - 1].d) {
						merge_last();
					} else {
						break;
					}
				}
				u = heavy_child[u];
			}
			while (b.size() != 1) {
				merge_last();
			}
			return b.back().p;
		};

		compressed.reserve(n - 1);
		raked.reserve(n - 1);

		array<vc<Node<Point>>, 64> q;
		for (int u : topo | std::ranges::views::reverse) {
			ull sum_rake = 0;
			for (int v : tree.at(u)) {
				sum_rake += bit_ceil(mask[v]) << 1;
			}
			mask[u] = bit_ceil(sum_rake);
			for (int v : tree.at(u)) {
				int d =
					countr_zero(bit_ceil(sum_rake - (bit_ceil(mask[v]) << 1)));
				ull s = ((mask[v] + (1ull << d) - 1) >> d << d) + (1ull << d);
				if (s <= mask[u]) {
					mask[u] = s;
					heavy_child[u] = v;
				}
			}

			int num_light = 0;
			ull has = 0;
			auto push = [&](Node<Point> data) {
				q[data.d].push_back(data);
				has |= (1ull << data.d);
			};
			auto pop = [&]() {
				int d = countr_zero(has);
				auto x = q[d].back();
				q[d].pop_back();
				if (q[d].empty()) has &= ~(1ull << d);
				return x;
			};
			for (int v : tree.at(u)) {
				if (v == heavy_child[u]) continue;
				num_light++;
				int d =
					countr_zero(bit_ceil(sum_rake - (bit_ceil(mask[v]) << 1)));
				Point point = dp.to_point(build_compress(v));
				push({d, point, &node_ids[v].r_id});
			}
			if (num_light == 0) continue;

			for (int ph = 0; ph < num_light - 1; ph++) {
				auto x = pop();
				auto y = pop();
				push(merge(x, y, raked, [&](auto lhs, auto rhs) {
					return dp.rake(lhs, rhs);
				}));
			}
			points[u] = pop().p;

			for (int v0 : tree.at(u)) {
				if (v0 == heavy_child[u]) continue;
				int v = v0;
				while (v != -1) {
					node_ids[v].h_par = u;
					node_ids[v].r_id = node_ids[v0].r_id;
					v = heavy_child[v];
				}
			}
		}
		build_compress(r);
	}

	// compress / rake
	template <class D> struct Inner {
		pair<D, D> d;
		int par;
	};
	vc<Inner<Path>> compressed;
	vc<Inner<Point>> raked;

	// compress / rake / leaf
	template <class D> struct Node {
		int d;
		D p;
		int* par;
	};
	template <class D>
	Node<D> merge(const Node<D>& l, const Node<D>& r, vc<Inner<D>>& nodes, auto op) {
		int id = int(nodes.size());
		assert(nodes.size() < nodes.capacity());
		*l.par = 2 * id;
		*r.par = 2 * id + 1;
		nodes.push_back({{l.p, r.p}, -1});
		assert(max(l.d, r.d) <= 60);
		return {max(l.d, r.d) + 1, op(l.p, r.p), &nodes[id].par};
	}

	vc<Point> points;
	struct ID {
		int h_par, c_id, r_id;
	};
	vc<ID> node_ids;

	Point update(int u) {
		Point p = points[u];
		while (u != n) {
			Path path = dp.add_vertex(p, u);
			auto [h_par, c_id, r_id] = node_ids[u];
			while (c_id >= 0) {
				if (c_id % 2 == 0) {
					compressed[c_id / 2].d.first = path;
				} else {
					compressed[c_id / 2].d.second = path;
				}
				path = dp.compress(compressed[c_id / 2].d.first,
								   compressed[c_id / 2].d.second);
				c_id = compressed[c_id / 2].par;
			}
			p = dp.to_point(path);
			while (r_id >= 0) {
				if (r_id % 2 == 0) {
					raked[r_id / 2].d.first = p;
				} else {
					raked[r_id / 2].d.second = p;
				}
				p = dp.rake(raked[r_id / 2].d.first, raked[r_id / 2].d.second);
				r_id = raked[r_id / 2].par;
			}
			points[h_par] = p;
			u = h_par;
		}
		return p;
	}
	
	Point getall(){
		return points[n];
	}
};

struct TreeDP{
	//UCUP 3-10-H
	//辺重み変更+直径計算
	struct N{
		ll path,ans,val[2];
		N(){
			path=-inf;
			ans=0;
			val[0]=val[1]=0;
		}
		static N merge(const N&a,const N&b){
			N res;
			res.path=max(-infLL,a.path+b.path);
			res.ans=max({a.ans,b.ans,a.val[1]+b.val[0]});
			res.val[0]=max(a.val[0],a.path+b.val[0]);
			res.val[1]=max(a.val[1]+b.path,b.val[1]);
			return res;
		}
		ll getans()const{
			return ans;
		}
	};

	struct M{
		ll ans,val;
		M(){
			ans=0;
			val=0;
		}
		M(N n){
			ans=n.ans;
			val=n.val[0];
		}
		static M merge(const M&a,const M&b){
			M res;
			res.ans=max({a.ans,b.ans,a.val+b.val});
			res.val=max(a.val,b.val);
			return res;
		}
		ll getans()const{
			return ans;
		}
	};
	vc<ll> wei;
	N add_vertex(const M&x,int v){
		N res;
		res.path=wei[v];
		res.ans=max(x.ans,wei[v]+x.val);
		res.val[0]=max<ll>(wei[v]+x.val,0);
		res.val[1]=max(wei[v],x.val);
		return res;
	}
	
	//多分普段はいじらない
	M id(){return M();}
	M rake(const M&x,const M&y){return M::merge(x,y);}
	N compress(const N&x,const N&y){return N::merge(x,y);}
	M to_point(const N&x){return M(x);}
};

	//根付きにして，頂点 i にその親の辺の重みを与えておく
	TreeDP dp;
	dp.wei=vc<ll>(n,-infLL);
	StaticTopTree<TreeDP> stt(t,dp,0);
	auto updwei=[&](int v,ll w){
		dp.wei[v]=w;
		stt.update(v);
	};


//CF969E
//各頂点に赤or青を設定して，赤と青の距離の最小値を求めている
//正確には inidist[v][col] が頂点 v から色 col の頂点までの距離みたいな感じ
//ansの計算のときに単独頂点で何かを達成するケースを忘れない
//MITIT 2025 Jan Round2 C (parlen で辺の重みに対応)
struct TreeDP{
	struct Path{
		int path,ans,val[2][2];
		Path(){
			path=inf;
			ans=inf;
			rep(i,2)rep(j,2)val[i][j]=inf;
		}
	};

	struct Point{
		int ans,val[2];
		Point(){
			ans=inf;
			rep(i,2)val[i]=inf;
		}
	};
	vi parlen;
	vc<array<int,2>> inidist;
	
	Point id(){return Point();}
	Point rake(const Point&x,const Point&y){
		Point res;
		res.ans=min(x.ans,y.ans);
		rep(i,2)chmin(res.ans,x.val[i]+y.val[i^1]);
		rep(i,2)res.val[i]=min(x.val[i],y.val[i]);
		return res;
	}
	Path compress(const Path&x,const Path&y){
		Path res;
		res.path=x.path+y.path;
		res.ans=min(x.ans,y.ans);
		rep(i,2)chmin(res.ans,x.val[i][1]+y.val[i^1][0]);
		rep(i,2)res.val[i][0]=min(x.val[i][0],x.path+y.val[i][0]);
		rep(i,2)res.val[i][1]=min(x.val[i][1]+y.path,y.val[i][1]);
		return res;
	}
	Path add_vertex(const Point&x,int v){
		Path res;
		res.path=parlen[v];
		res.ans=min(x.ans,inidist[v][0]+inidist[v][1]);
		rep(i,2)chmin(res.ans,inidist[v][i]+x.val[i^1]);
		rep(i,2)rep(j,2)res.val[i][j]=min(min(inidist[v][i],x.val[i])+(1-j)*parlen[v],inf);
		return res;
	}
	Point to_point(const Path&x){
		Point res;
		res.ans=x.ans;
		rep(i,2)res.val[i]=x.val[i][0];
		return res;
	}
};
