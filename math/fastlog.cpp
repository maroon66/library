//参考
//https://maspypy.com/o1-mod-inv-mod-pow#toc7
//https://judge.yosupo.jp/submission/348836
//verify: https://judge.yosupo.jp/problem/discrete_logarithm_fixed_mod
template<class mint>
struct fastlog{
	const int p;
	const mint root;
	int s;
	vi sf;
	void linear_sieve(){
		vi ps;
		sf.resize(s+1,-1);
		rng(i,2,s+1){
			if(sf[i]==-1){
				ps.pb(i);
				sf[i]=i;
			}
			for(int j=0;i*ps[j]<=s;j++){
				sf[i*ps[j]]=ps[j];
				if(i%ps[j]==0)break;
			}
		}
	}
	int B;
	vc<mint> baby,giant;
	void init_baby_giant(){
		B=min(s*5,p-1);
		baby.resize(B);
		mint z=1;
		rep(i,B){
			baby[i]=z;
			z*=root;
		}
		int C=(p-1+B-1)/B;
		giant.resize(C);
		mint w=1;
		rep(i,C){
			giant[i]=w;
			w*=z;
		}
	}
	mint pow_root(int e){
		assert(inc(0,e,p-2));
		int u=e/B,v=e%B;
		return giant[u]*baby[v];
	}
	vi logbuf;
	void init_small_logs(){
		unordered_map<uint,int> table;
		table.reserve(B);
		rep(i,B)table[baby[i].v]=i;
		mint bigstep=root.pow(-B);
		
		logbuf.resize(s+1);
		rng(i,2,s+1){
			if(sf[i]<i){
				logbuf[i]=logbuf[sf[i]]+logbuf[i/sf[i]];
				if(logbuf[i]>=(p-1))logbuf[i]-=(p-1);
			}else if(i<100){
				mint cur=i;
				while(1){
					auto itr=table.find(cur.v);
					if(itr==table.ed){
						cur*=bigstep;
						logbuf[i]+=B;
					}else{
						logbuf[i]+=itr->b;
						break;
					}
				}
			}else{
				int k=0;
				mint start=i;
				while(1){
					k++;
					start*=root;
					
					mint cur=start;
					ll val=p-1-k;
					for(int d:{2,3,5,7,11,13,17,19,23,29}){
						while(cur.v%d==0){
							cur.v/=d;
							val+=logbuf[d];
						}
					}
					if((int)cur.v>s)continue;
					bool bad=false;
					while(i<=(int)cur.v){
						if(sf[cur.v]==(int)cur.v){
							bad=true;
							break;
						}
						val+=logbuf[sf[cur.v]];
						cur.v/=sf[cur.v];
					}
					if(bad)continue;
					val+=logbuf[cur.v];
					logbuf[i]=val%(p-1);
					break;
				}
			}
		}
	}
	fastlog():p(mint::mod),root(mint::root()),s(sqrtl(p)){
		linear_sieve();
		init_baby_giant();
		init_small_logs();
	}
	int log(mint x){
		assert(inc(1,x.v,p-1));
		ll ans=0;
		while((int)x.v>s){
			int a=p/x.v,b=p%x.v;
			ans+=(p-1)/2;
			ans+=(p-1)-logbuf[a];
			x=b;
		}
		ans+=logbuf[x.v];
		return ans%(p-1);
	}
	mint inv(mint x){
		if(x.v==1)return 1;
		else return pow_root(p-1-log(x));
	}
	mint pow(mint a,int b){
		assert(inc(0,b,p-2));
		if(a==0){
			return b==0?1:0;
		}else{
			return pow_root((ll)log(a)*b%(p-1));
		}
	}
};
