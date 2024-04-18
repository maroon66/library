//UCUP 2023-3 F
template<class F>
void segeasy(int s,int l,int r,F f){
	for(l+=s,r+=s;l<r;l>>=1,r>>=1){
		if(l&1)f(l);
		if(r&1)f(r-1);
		l++; //ceil l
	}
}
//include geo/pi
//点集合 (ソート済み) で init
//query(l,r,p,q)
//[l,r) 内で (p,q) との内積最大の点を返す
//内積最大の中でどの点を返しているかは謎です!すまん
//q>=0 が必要
//query をまとめて処理することでオーダーを落としている
//stress-tested
//EC Final 2021 K
struct CHT_Query{
	int n,s;
	vc<pi> rw;
	vvc<pi> z;
	vi head;
	CHT_Query(const vc<pi>&ps){
		assert(is_sorted(all(ps)));
		n=si(ps);
		s=minp2(n);
		rw=ps;
		z.resize(s*2);
		rep(i,n)z[s+i].pb(ps[i]);
		gnr(i,1,s)z[i]=cat_convex_upper(z[i*2],z[i*2+1]);
		head.resize(2*s);
	}
	struct Query{
		int l,r;
		pi dir;
	};
	vc<Query> qs;
	void add_query(int l,int r,int p,int q){
		assert(q>=0);
		assert(0<=l&&l<r&&r<=n);
		qs.pb({l,r,pi(p,q)});
	}
	vc<pi> calc(){
		vc<pi> ans(si(qs));
		vi idx;
		rep(qid,si(qs)){
			auto [l,r,dir]=qs[qid];
			if(dir.b==0){
				if(dir.a<=0)ans[qid]=rw[l];
				else ans[qid]=rw[r-1];
			}else{
				idx.pb(qid);
			}
		}
		soin(idx,[&](int i,int j){return ccw(qs[i].dir,qs[j].dir)<0;});
		const pi none=pi(-inf,-inf);
		fila(head,0);
		for(auto qid:idx){
			auto [l,r,dir]=qs[qid];
			auto eval=[&](pi v){return dot(v,dir);};
			pi cur=none;
			auto upd=[&](pi v){
				if(cur==none||dot(cur,dir)<dot(v,dir))cur=v;
			};
			segeasy(s,l,r,[&](int i){
				if(z[i].empty())return;
				int&j=head[i];
				while(j+1<si(z[i])&&eval(z[i][j])<eval(z[i][j+1]))j++;
				upd(z[i][j]);
			});
			ans[qid]=cur;
		}
		qs.clear();
		return ans;
	}
};
