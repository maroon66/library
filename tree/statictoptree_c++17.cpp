//俺のライブラリの定数倍カス過ぎる
//yosupo に頼ろう
//https://atcoder.jp/contests/abc351/submissions/52958214
//MITIT 2025 Jan Round 2 C (C++17 しか使えなかったので泣きながら直した)
template <class TreeDP> struct StaticTopTree {
	using Path = typename TreeDP::Path;
	using Point = typename TreeDP::Point;

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
				b.push_back({botbit(minp2(mask[u])),
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

		array<vc<Node<Point>>, 65> q;
		//for (int u : topo | std::ranges::views::reverse) {
		per(uid,si(topo)){
			const auto&u=topo[uid];
			ull sum_rake = 0;
			for (int v : tree.at(u)) {
				sum_rake += minp2(mask[v]) << 1;
			}
			mask[u] = minp2(sum_rake);
			for (int v : tree.at(u)) {
				int d =
					botbit(minp2(sum_rake - (minp2(mask[v]) << 1)));
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
				int d = botbit(has);
				auto x = q[d].back();
				q[d].pop_back();
				if (q[d].empty()) has &= ~(1ull << d);
				return x;
			};
			for (int v : tree.at(u)) {
				if (v == heavy_child[u]) continue;
				num_light++;
				int d =
					botbit(minp2(sum_rake - (minp2(mask[v]) << 1)));
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
		points[n]=dp.to_point(build_compress(r));
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
