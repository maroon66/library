//original version: https://github.com/koosaga/DeobureoMinkyuParty

/*
	LP
	max cx
	s.t. ax <= b, x >= 0

	time complexity: exponential. fast $O(hw^2)$ in experiment. dependent on the modeling.
*/
struct simplex{
	int h,w,type;
	vi lf,dw;   //h,w
	vc<ld> sol;   //w
	ld cx;

	// 0: found solution, 1: infeasible, 2: unbounded
	simplex(vvc<ld> a, vc<ld> b, vc<ld> c):h(b.size()),w(c.size()),
		lf(h),dw(w),sol(w),cx(0){
		assert((int)a.size() == h);
		assert((int)a[0].size() == w);

		auto pivot = [&](int x, int y) {
			swap(lf[x], dw[y]);
			ld k = a[x][y];
			a[x][y] = 1;
			vi nz;
			rep(i,w){
				a[x][i] /= k;
				if (!eq(a[x][i], 0)) nz.push_back(i);
			}
			b[x] /= k;

			rep(i,h){
				if (i == x || eq(a[i][y], 0)) continue;
				k = a[i][y];
				a[i][y] = 0;
				b[i] -= k * b[x];
				for (int j : nz) a[i][j] -= k * a[x][j];
			}
			if (eq(c[y], 0)) return;
			k = c[y];
			c[y] = 0;
			cx += k * b[x];
			for (int i : nz) c[i] -= k * a[x][i];
		};

		rep(i,w) dw[i] = i;
		rep(i,h) lf[i] = w + i;
		while (1) { // Eliminating negative b[i]
			int x = -1, y = -1;
			rep(i,h) if(ls(b[i], 0) && (x == -1 || b[i] < b[x])) x = i;
			if (x == -1) break;
			rep(i,w) if(ls(a[x][i], 0) && (y == -1 || a[x][i] < a[x][y])) y = i;
			if (y == -1){
				type = 1;   //infeasible
				cx = -1e100;
				return;
			}
			pivot(x, y);
		}
		while (1) {
			int x = -1, y = -1;
			rep(i,w) if(ls(0, c[i]) && (y == -1 || c[i] > c[y])) y = i;
			if (y == -1) break;
			rep(i,h) if(ls(0, a[i][y]) && (x == -1 || b[i] / a[i][y] < b[x] / a[x][y])) x = i;
			if (x == -1){
				type = 2;   //unbounded
				cx = 1e100;
				return;
			}
			pivot(x, y);
		}
		rep(i,h) if (lf[i] < w) sol[lf[i]] = b[i];
		type = 0;
	}

	bool eq(ld a, ld b) { return fabs(a - b) < eps; }
	bool ls(ld a, ld b) { return a < b && !eq(a, b); }
};
