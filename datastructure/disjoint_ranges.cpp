//N: 区間の座標
//clear: 今まで追加した区間全部なしにする
//add(l,r): [l,r) を追加
//stress-tested
//JOISC 2025 Exhibition
struct disjoint_ranges{
	struct N{
		int val,sum;
		N(int v=0):val(v),sum(v){}
		void reverse(){}
		void pushl(N&){}
		void pushr(N&){}
		void clear(){}
		void single(){
			sum=val;
		}
		void updatel(const N&x){
			sum+=x.sum;
		}
		void updater(const N&x){
			sum+=x.sum;
		}
	};
	int n;
	linkcut<N> lc;
	map<int,int> s;
	disjoint_ranges(int nn):n(nn),lc(n+1){
		rep(i,n+1)lc[i]=N(0);
		rep(i,n)lc.link(i,i+1);
	}
	void change(int i,int j){
		lc.cutpar(i);
		lc.link(i,j);
	}
	void reset(int i){
		assert(inc(0,i,n-1));
		lc.expose(i);
		lc[i].val=0;
		lc.x[i].update();
		change(i,i+1);
	}
	void set(int i,int j){
		assert(i<j);
		assert(j<=n);
		lc.expose(i);
		lc[i].val=1;
		lc.x[i].update();
		change(i,j);
	}
	void clear(){
		for(auto [l,r]:s){
			reset(l);
		}
		s.clear();
	}
	int getans(){
		lc.expose(0);
		return lc[0].sum;
	}
	void add(int l,int r){
		assert(0<=l&&l<r&&r<=n);
		{
			auto itr=s.lower_bound(l);
			if(itr!=s.ed&&itr->b<=r)return;
		}
		auto itr=s.lower_bound(l+1);
		while(itr!=s.bg){
			--itr;
			if(itr->b<r)break;
			reset(itr->a);
			itr=s.erase(itr);
		}
		s[l]=r;
		set(l,r);
	}
};
