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
