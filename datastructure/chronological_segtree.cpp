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
