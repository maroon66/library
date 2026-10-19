//自家製bitset利用
//UCUP4-Semifinal-C
struct SubsetSum{
	vi a;
	int s;
	BS has,none,dif;
	vi use,rawcnt,cnt;
	SubsetSum(vi aa):a(aa),s(SUM(a)),has(s+1),none(s+1,true),dif(s+1),use(s+1,-1),rawcnt(s+1){
		has.set(0);
		none.reset(0);

		for(auto v:a)rawcnt[v]++;
		cnt=rawcnt;
		rng(i,1,s+1){
			while(cnt[i]>=3){
				cnt[i]-=2;
				cnt[i*2]++;
			}
		}

		int tot=0;
		auto advance=[&](int v){
			has.window_assign(0,tot+1,dif,0);
			none.window_and(v,tot+v+1,dif,0);
			
			int cur=0;
			while(1){
				cur=dif._Find_next(cur);
				if(cur<=tot){
					has.set(cur+v);
					none.reset(cur+v);
					dif.reset(cur);
					use[cur+v]=v;
				}else break;
			}

			tot+=v;
		};

		rng(i,1,s+1)rep(_,cnt[i])advance(i);
	}
	bool ask(int tar){
		assert(0<=tar);
		if(tar>s)return false;
		else return has.get(tar);
	}
	vi restore(int tar){
		vi f(s+1);
		while(tar>0){
			int u=use[tar];
			f[u]++;
			tar-=u;
		}
		for(int v=s/2*2;v>=2;v-=2){
			while(f[v]>rawcnt[v]){
				f[v]--;
				f[v/2]+=2;
			}
		}

		vi idx;
		rep(i,si(a)){
			if(f[a[i]]>0){
				idx.pb(i);
				f[a[i]]--;
			}
		}

		return idx;
	}
};