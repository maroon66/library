//TCO2021 Semifinal 2
//https://core.ac.uk/download/pdf/82222988.pdf
//Algorithm G
//https://mathoverflow.net/questions/125819/generalized-sprague-grundy-theorem
//vs がラベル
//idx が counter function (vs[i]>=0 でしか意味を持たない)
struct generalized_grundy{
	const vvc<int>&g;
	vi vs,idx;
	//-1 inf
	//-2 undefined
	generalized_grundy(const vvc<int>&gg):g(gg),vs(si(g),-2),idx(si(g),-1){
		int n=si(g);
		vvc<int> rg(n);
		rep(i,n)for(auto j:g[i])rg[j].pb(i);
		vc<bool> hasnext(n);
		vi cnt(n),buf(n);
		int ord=0;
		rep(label,n){
			rep(i,n)hasnext[i]=vs[i]>=0;
			int s=0;
			rep(i,n)if(vs[i]==-2){
				cnt[i]=0;
				for(auto j:g[i])if(!hasnext[j])cnt[i]++;
				if(cnt[i]==0)buf[s++]=i;
			}
			rep(head,s){
				int v=buf[head];
				bool ok=true;
				for(auto j:g[v])if(vs[j]==label){
					ok=false;
					break;
				}
				if(!ok)continue;
				vs[v]=label;
				for(auto j:rg[v])if(!hasnext[j]){
					hasnext[j]=true;
					for(auto k:rg[j])if(vs[k]==-2){
						if(--cnt[k]==0)buf[s++]=k;
					}
				}
				idx[v]=ord++;
			}
			bool rem=false;
			rep(i,n)if(vs[i]==-2){
				bool ok=true;
				for(auto j:g[i])if(vs[j]==label){
					ok=false;
					break;
				}
				if(!ok){
					rem=true;
					continue;
				}
				vs[i]=-1;
			}
			if(!rem)break;
		}
	}
	pair<int,vi> getgrundy(int v){
		if(vs[v]==-1){
			vi ls;
			for(auto to:g[v])if(vs[to]!=-1)ls.pb(vs[to]);
			mkuni(ls);
			return mp(-1,ls);
		}else{
			return mp(vs[v],vi{});
		}
	}
};

using GG=pair<int,vi>;
vi modified(vi a,int x){
	for(auto&v:a)v^=x;
	sort(all(a));
	return a;
}
GG mergegrundy(const GG&a,const GG&b){
	if(a.a==-1){
		if(b.a==-1)return GG(-1,{});
		else return GG(-1,modified(a.b,b.a));
	}else{
		if(b.a==-1)return GG(-1,modified(b.b,a.a));
		else return GG(a.a^b.a,{});
	}
}
//-1:lose, 0:draw, 1:win
int getresult(const GG&a){
	if(a.a==-1){
		if(si(a.b)&&a.b[0]==0)return 1;
		else return 0;
	}else{
		return a.a==0?-1:1;
	}
}
