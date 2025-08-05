template<class Query>
struct chronological_segtree{
	int n,s;
	vvc<Query> qs;
	chronological_segtree(int nn){
		n=nn;
		s=1;
		while(s<n)s*=2;
		qs.resize(s*2);
	}
	void add(int l,int r,Query q){
		for(l+=s,r+=s;l<r;l>>=1,r>>=1){
			if (l&1){
				qs[l].pb(q);
				l++;
			}
			if (r&1){
				r--;
				qs[r].pb(q);
			}
		}
	}
	template<class F,class G,class H,class I>
	void solve(F f,G snapshot,H rollback,I getans){
		auto rec=[&](auto self,int i,int l,int r)->void{
			auto ss=snapshot();
			for(const auto&q:qs[i])
				f(q);
			if(i<s){
				self(self,i*2,l,(l+r)/2);
				self(self,i*2+1,(l+r)/2,r);
			}else if(i-s<n){
				getans(i-s);
			}
			rollback(ss);
		};
		rec(rec,1,0,s);
	};
};


//時系列segtree のテンプレート
//vvc を vec2d にしてメモリがちょっとよい
//7th Turing Cup C
template<class Query>
struct chronological_segtree_onlyadd{
	private:
	int n,s,L;
	vec2d<Query> qs;
	vi qcnt;
	void init(){
		soin(buf); //ソート！クエリ処理のやり方によっては必要！
		for(auto [v,l,r]:buf){
			segeasy(s,l,r,[&](int i){
				qcnt[i]++;
			});
		}
		qs.reserve(SUM(qcnt));
		rep(i,2*s)qs.init(i,qcnt[i]);
		fila(qcnt,0);
		for(auto [v,l,r]:buf){
			segeasy(s,l,r,[&](int i){
				qs[i][qcnt[i]++]=v;
			});
		}
	}
	vc<tuple<Query,int,int>> buf;
	public:
	chronological_segtree_onlyadd(int nn):n(nn),s(minp2(n)),L(topbit(s)),
		qs(2*s),qcnt(2*s){}
	void add(int l,int r,Query v){
		if(l<r)buf.eb(v,l,r);
	}
	vc<ll> calc(){
		init();
		//↓ここから書き換える
		
		vi mx=qcnt;
		gnr(i,1,s)mx[i]+=max(mx[i*2],mx[i*2+1]);
		
		vc<ll> ans(n);
		auto dfs=[&](auto self,int node,int l,int r,vi dif,ll curans)->void{
			if(n<=l)return;
			dif.resize(mx[node]);
			int head=0,tail=0;
			rep(qq,qcnt[node]){
				auto v=qs[node][qq];
				
				while(tail<si(dif)&&dif[tail]<v){
					dif[head++]=dif[tail++];
				}
				
				if(tail==si(dif)){
					head--;
					tail--;
					dif.pop_back();
				}else{
					int w=dif[tail]-v;
					if(head==0){
						tail++;
						curans+=w;
					}else{
						dif[tail]=dif[--head]+w;
					}
				}
			}
			
			while(tail<si(dif)){
				dif[head++]=dif[tail++];
			}
			dif.resize(head);
			
			if(r-l==1){
				ans[l]=curans;
			}else{
				self(self,node*2+0,l,(l+r)/2,dif,curans);
				self(self,node*2+1,(l+r)/2,r,dif,curans);
			}
		};
		vi inidif=vid(mx[1])*2;
		dfs(dfs,1,0,s,inidif,0);
		
		return ans;
	}
};
