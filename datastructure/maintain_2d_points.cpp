//SRM600.5 Med
struct maintain_2d_point{
	vi xs;
	int s;
	vc<set<pi>> z;
	void addx(int x){
		xs.pb(x);
	}
	void init(){
		mkuni(xs);
		s=1;
		while(s<si(xs))s*=2;
		z.resize(s*2);
	}
	void addp(int x,int y,int idx){
		int i=lwb(xs,x);
		i+=s;
		while(i){
			z[i].emplace(y,idx);
			i>>=1;
		}
	}
	//[l,r), [lw,up)
	//maybe duplicate
	vi getrect(int l,int r,int lw,int up){
		l=lwb(xs,l);
		r=lwb(xs,r);
		vi res;
		auto sub=[&](set<pi>&w){
			auto itr=w.lower_bound(pi(lw,-1));
			while(itr!=w.ed&&itr->a<up){
				res.pb(itr->b);
				itr=w.erase(itr);
			}
		};
		for(l+=s,r+=s;l<r;l>>=1,r>>=1){
			if (l&1){
				sub(z[l]);
				l++;
			}
			if (r&1){
				r--;
				sub(z[r]);
			}
		}
		return res;
	}
};

struct maintain_1d_point{
	vc<pi> xi;
	BIT<int> bit;
	void addp(int x,int i){
		xi.eb(x,i);
	}
	void init(){
		sort(all(xi));
		bit=BIT<int>(si(xi));
		rep(i,si(xi))bit.add(i,1);
	}
	void work(int lw,int up,vi&res){
		lw=lwb(xi,pi(lw,-inf));
		int tmp=bit.get(lw-1);
		while(1){
			int i=bit.kth(tmp);
			if(i<si(xi)&&xi[i].a<up){
				res.pb(xi[i].b);
				bit.add(i,-1);
			}else break;
		}
	}
};

//CF712D
struct maintain_2d_point{
	vi xs;
	int s;
	vc<maintain_1d_point> z;
	void init(vc<pi> xy){
		for(auto [x,y]:xy)xs.pb(x);
		
		mkuni(xs);
		s=1;
		while(s<si(xs))s*=2;
		z.resize(s*2);
		
		int idx=0;
		for(auto [x,y]:xy){
			int i=lwb(xs,x);
			i+=s;
			while(i){
				z[i].addp(y,idx);
				i>>=1;
			}
			idx++;
		}
		
		rep(i,2*s)z[i].init();
	}
	//[l,r), [lw,up)
	//maybe duplicate
	vi getrect(int l,int r,int lw,int up){
		l=lwb(xs,l);
		r=lwb(xs,r);
		vi res;
		auto sub=[&](maintain_1d_point&w){
			w.work(lw,up,res);
		};
		for(l+=s,r+=s;l<r;l>>=1,r>>=1){
			if (l&1){
				sub(z[l]);
				l++;
			}
			if (r&1){
				r--;
				sub(z[r]);
			}
		}
		return res;
	}
};
