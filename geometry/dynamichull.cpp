ll fdiv(ll a, ll b) { // floored division
	return a / b - ((a ^ b) < 0 && a % b); }
//O(N log^2 N)
//int=ld=llでしか verify してない
//https://judge.yosupo.jp/problem/convex_layers
//https://codeforces.com/blog/entry/75929
//stress-tested
//座標幅 10^9 で動くつもりだが，10^6 でしか verify してない
//10^6 のときは割り算を回避できるため定数倍が少しよくなる
//UCUP 2023 5 (long double)
struct DynamicUpperHull{
	static const pt none;
	const vc<pt> ps;
	vc<bool> avail;
	const bool onedge;
	struct N{
		ld mn;
		pt a,b;
		N(const pt&p=none):mn(p.x),a(p),b(p){}
		bool empty(){
			return mn==none.x;
		}
		bool isleaf(){
			return a==b;
		}
	};
	int n,s;
	vc<N> x;
	//allow onedge?
	DynamicUpperHull(const vc<pt>&pp,vc<bool> aa,bool oo):ps(pp),avail(aa),onedge(oo){
		n=si(ps);
		s=1;while(s<n)s*=2;
		assert(is_sorted(all(ps)));
		x.resize(s*2);
		rep(i,n)if(avail[i])x[s+i]=ps[i];
		gnr(i,1,s)upd(i);
	}
	void descend(int&i){
		while(i<s){
			if(x[i*2].empty())i=i*2+1;
			else if(x[i*2+1].empty())i=i*2;
			else break;
		}
	}
	void upd(int i){
		assert(inc(1,i,s-1));
		if(x[i*2].empty())x[i]=x[i*2+1];
		else if(x[i*2+1].empty())x[i]=x[i*2];
		else{
			x[i].mn=x[i*2].mn;
			int p=i*2,q=i*2+1;
			while(1){
				descend(p);
				descend(q);
				bool pl=x[p].isleaf();
				bool ql=x[q].isleaf();
				if(pl&&ql)break;
				if(!pl&&ccw(x[p].a,x[p].b,x[q].a)>=(onedge?1:0)){
					p=p*2;
				}else if(!ql&&ccw(x[p].b,x[q].a,x[q].b)>=(onedge?1:0)){
					q=q*2+1;
				}else if(pl){
					q=q*2;
				}else if(ql){
					p=p*2+1;
				}else{
					auto [numl,denl]=xcut(x[p].a,x[p].b,x[q].mn);
					auto [numr,denr]=xcut(x[q].a,x[q].b,x[q].mn);
					if(sgn(denl)==0||sgn(denr)==0){
						assert(sgn(denl)==0);
						if(!onedge&&x[p].a.x==x[q].mn)p=p*2;
						else p=p*2+1;
					}else{
						assert(denl>0);
						assert(denr>0);
						/*ll fl=fdiv(numl,denl);
						ll fr=fdiv(numr,denr);
						if(fl>fr){
							p=p*2+1;
						}else if(fl<fr){
							q=q*2;
						}else if((numl-fl*denl)*denr>(numr-fr*denr)*denl){
							p=p*2+1;
						}else{
							q=q*2;
						}*/
						if(numl*denr>numr*denl){
							p=p*2+1;
						}else{
							q=q*2;
						}
					}
				}
			}
			x[i].a=x[p].a;
			x[i].b=x[q].a;
		}
	}
	void toggle(int i){
		assert(inc(0,i,n-1));
		if(avail[i]){
			x[s+i]=N();
			avail[i]=false;
		}else{
			x[s+i]=ps[i];
			avail[i]=true;
		}
		i+=s;
		while(i>>=1)upd(i);
	}
	void dfs(int i,const pt&l,const pt&r,vc<pt>&res){
		//assume at least one point is included
		descend(i);
		if(x[i].isleaf()){
			res.pb(x[i].a);
		}else if(x[i].a<l){
			dfs(i*2+1,l,r,res);
		}else if(r<x[i].b){
			dfs(i*2,l,r,res);
		}else{
			dfs(i*2,l,x[i].a,res);
			dfs(i*2+1,x[i].b,r,res);
		}
	}
	vc<pt> getlist(){
		vc<pt> res;
		if(!x[1].empty())dfs(1,pt(-inf,-inf),pt(inf,-inf),res);
		return res;
	}
};
const pt DynamicUpperHull::none(inf,inf);

//UCUP 2023 5 (long double)
vvc<pt> convex_layer(vc<pt> ps){
	int n=si(ps);assert(is_sorted(all(ps)));
	DynamicUpperHull adh(ps,vc<bool>(n,true),true);
	for(auto&p:ps)p=-p;
	reverse(all(ps));
	DynamicUpperHull bdh(ps,vc<bool>(n,true),true);
	for(auto&p:ps)p=-p;
	reverse(all(ps));
	
	vc<bool> used(n);
	vvc<pt> res;
	int done=0;
	rng(step,1,n+1){
		if(done==n)break;
		vi ls;
		for(auto p:adh.getlist()){
			int i=lwb(ps,p);
			if(!used[i])ls.pb(i);
		}
		for(auto p:bdh.getlist()){
			int i=lwb(ps,-p);
			if(!used[i])ls.pb(i);
		}
		mkuni(ls);
		vc<pt> s;
		for(auto i:ls){
			used[i]=true;
			s.pb(ps[i]);
			adh.toggle(i);
			bdh.toggle(n-1-i);
			done++;
		}
		sort(all(s));
		sort(s.bg+1,s.ed,[&](pt a,pt b){
			int c=ccw(s[0],a,b);
			if(c)return c==1;
			else{
				if(sgn(s[0].x-a.x)==0)
					return a.y>b.y;
				return bet(s[0],a,b)==2;
			}
		});
		res.pb(s);
	}
	
	return res;
}

//CF875F にTLEしたコードがある
//全点の x 座標が strictly increasing だとして
//区間を選んでその区間内の点の y 座標に add ができる
//あと query で dot 最大化ができるが，これは明らかに偏角 [0,pi] のクエリじゃないとだめ

//O(N log^2 N)
//int=ld=llでしか verify してない
//区間を shiftXY，区間 y+=x クエリを追加
//常に点が (x,y) でソートされている必要がある
//https://contest.xinyoudui.com/contest/426/problem/2191
//定数倍は相変わらずだめ
//サイズ 10^5 で 2*10^5 クエリ程度ですでに 2sec かかる
//|X|*|Y| <= 10^18 くらいを仮定
const pt none(inf,inf);
struct DynamicUpperHull{
	struct N{
		ld mn;
		pt a,b;
		ld lzt;
		pt lzxy;
		N(const pt&p=none):mn(p.x),a(p),b(p),lzt(0),lzxy(){}
		void addX2Y(ld t){
			if(empty())return;
			a.y+=a.x*t;
			b.y+=b.x*t;
			lzt+=t;
			lzxy.y+=lzxy.x*t;
		}
		void shiftXY(pt xy){
			if(empty())return;
			mn+=xy.x;
			a+=xy;
			b+=xy;
			lzxy+=xy;
		}
		bool empty()const{
			return mn==none.x;
		}
		bool isleaf()const{
			return a==b;
		}
		void push(N&l,N&r){
			l.addX2Y(lzt); r.addX2Y(lzt);
			lzt=0;
			l.shiftXY(lzxy); r.shiftXY(lzxy);
			lzxy=pt();
		}
		N clone()const{
			N res=*this;
			res.lzt=0;
			res.lzxy=pt();
			return res;
		}
	};
	int n,s,L;
	const bool onedge;
	vc<N> x;
	//allow onedge?
	DynamicUpperHull(const vc<pt>&ps,bool oo):n(si(ps)),s(minp2(n)),L(topbit(s)),
		onedge(oo),x(2*s){
		{
			pt pre(-inf,-inf);
			rep(i,n)if(ps[i]!=none){
				assert(pre<ps[i]);
				pre=ps[i];
			}
		}
		rep(i,n)if(ps[i]!=none)x[s+i]=ps[i];
		gnr(i,1,s)upd(i);
	}
	void push(int i){
		x[i].push(x[i*2],x[i*2+1]);
	}
	void descend(int&i){
		while(i<s){
			push(i);
			if(x[i*2].empty())i=i*2+1;
			else if(x[i*2+1].empty())i=i*2;
			else break;
		}
	}
	void upd(int i){
		assert(inc(1,i,s-1));
		assert(x[i].lzt==0);
		assert(x[i].lzxy==pt());
		if(x[i*2].empty())x[i]=x[i*2+1].clone();
		else if(x[i*2+1].empty())x[i]=x[i*2].clone();
		else{
			x[i].mn=x[i*2].mn;
			int p=i*2,q=i*2+1;
			while(1){
				descend(p);
				descend(q);
				bool pl=x[p].isleaf();
				bool ql=x[q].isleaf();
				if(pl&&ql)break;
				if(!pl&&ccw(x[p].a,x[p].b,x[q].a)>=(onedge?1:0)){
					p=p*2;
				}else if(!ql&&ccw(x[p].b,x[q].a,x[q].b)>=(onedge?1:0)){
					q=q*2+1;
				}else if(pl){
					q=q*2;
				}else if(ql){
					p=p*2+1;
				}else{
					auto [numl,denl]=xcut(x[p].a,x[p].b,x[q].mn);
					auto [numr,denr]=xcut(x[q].a,x[q].b,x[q].mn);
					if(sgn(denl)==0||sgn(denr)==0){
						assert(sgn(denl)==0);
						if(!onedge&&x[p].a.x==x[q].mn)p=p*2;
						else p=p*2+1;
					}else{
						assert(denl>0);
						assert(denr>0);
						if(__int128(numl)*denr>__int128(numr)*denl){
							p=p*2+1;
						}else{
							q=q*2;
						}
					}
				}
			}
			x[i].a=x[p].a;
			x[i].b=x[q].a;
		}
	}
	void point_set(int i,pt v){
		i+=s;
		per(lv,L)push(i>>(lv+1));
		x[i]=v;
		while(i>>=1)upd(i);
	}
	void shiftXYdfs(int i,int l,int r,int b,int e,pt v){
		if(e<=l||r<=b)return;
		if(b<=l&&r<=e)return x[i].shiftXY(v);
		push(i);
		shiftXYdfs(i*2+0,l,(l+r)/2,b,e,v);
		shiftXYdfs(i*2+1,(l+r)/2,r,b,e,v);
		upd(i);
	}
	void shiftXY(int b,int e,pt v){if(b<e)shiftXYdfs(1,0,s,b,e,v);}
	void addX2Ydfs(int i,int l,int r,int b,int e,ld v){
		if(e<=l||r<=b)return;
		if(b<=l&&r<=e)return x[i].addX2Y(v);
		push(i);
		addX2Ydfs(i*2+0,l,(l+r)/2,b,e,v);
		addX2Ydfs(i*2+1,(l+r)/2,r,b,e,v);
		upd(i);
	}
	void addX2Y(int b,int e,ld v){if(b<e)addX2Ydfs(1,0,s,b,e,v);}
	ld querydfs(int i,pt v){
		descend(i);
		if(x[i].isleaf())return dot(x[i].a,v);
		else if(dot(x[i].a,v)>dot(x[i].b,v))
			return querydfs(i*2,v);
		else
			return querydfs(i*2+1,v);
	}
	ld query(pt v){
		assert(!x[1].empty());
		ld res=querydfs(1,v);
		return res;
	}
};
