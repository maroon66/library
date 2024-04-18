//https://codeforces.com/gym/421334/problem/A
template<int nmax>
struct permutation_basis{
	using A=array<int,nmax>;
	const int n;
	//(p[q[i]])
	void cp(int lv,const A&p,A&q){
		rng(i,lv,n)q[i]=p[i];
	}
	void cmp(int lv,const A&p,const A&q,A&r){
		rng(i,lv,n)r[i]=p[q[i]];
	}
	void inv(int lv,const A&p,A&q){
		rng(i,lv,n)q[p[i]]=i;
	}
	A tmp,tmp2;
	int cnt[2];
	A ls[2][nmax*nmax];
	A sbuf[2][nmax][nmax],sinvbuf[2][nmax][nmax];
	bool shas[2][nmax][nmax];
	void sclear(int lv){
		const int k=lv&1;
		cnt[k]=0;
		rng(i,lv,n)rng(j,i,n)shas[k][i][j]=false;
	}
	void sims(int lv,A&p){
		const int k=lv&1;
		rng(i,lv,n)if(p[i]!=i){
			auto&x=sbuf[k][i];
			auto&y=sinvbuf[k][i];
			auto&h=shas[k][i];
			if(h[p[i]]){
				cmp(i,y[p[i]],p,tmp);
				cp(i,tmp,p);
			}else{
				cp(i,p,x[p[i]]);
				inv(i,p,y[p[i]]);
				cp(lv,p,ls[k][cnt[k]++]);
				h[p[i]]=true;
				break;
			}
		}
	}
	A obuf[nmax][nmax],oinvbuf[nmax][nmax];
	bool ohas[nmax][nmax];
	//行列で言うところの基底が，順列の集合みたいになっている
	//lv 要素目の基底について
	//ohas[lv][v] x[lv]=v となる順列 x があるか？
	//obuf[lv][v] あるならその順列
	//oinvbuf[lv][v] ↑の inv
	permutation_basis(int nn,const vvc<int>&rw):n(nn),ohas{}{
		assert(inc(1,n,nmax));
		sclear(0);
		for(const auto&p:rw){
			rep(i,n)tmp2[i]=p[i];
			sims(0,tmp2);
		}
		rep(lv,n){
			const int k=lv&1;
			auto&x=obuf[lv];
			auto&y=oinvbuf[lv];
			auto&h=ohas[lv];
			{
				auto dfs=[&](auto self,int v)->void{
					assert(x[v][lv]==v);
					assert(h[v]);
					inv(lv,x[v],y[v]);
					rep(i,cnt[k]){
						const auto&p=ls[k][i];
						int to=p[v];
						if(!h[to]){
							cmp(lv,p,x[v],x[to]);
							h[to]=true;
							self(self,to);
						}
					}
				};
				rng(i,lv,n)x[lv][i]=i;
				h[lv]=true;
				dfs(dfs,lv);
			}
			if(lv+1<n){
				sclear(lv+1);
				rng(v,lv,n)if(h[v]){
					rep(i,cnt[k]){
						const auto&p=ls[k][i];
						cmp(lv,p,x[v],tmp);
						cmp(lv,y[p[v]],tmp,tmp2);
						sims(lv+1,tmp2);
					}
				}
			}
		}
	}
	template<class t>
	t cardinality(){
		t ans=1;
		rep(i,n){
			int num=0;
			rng(j,i,n)if(ohas[i][j])num++;
			ans*=num;
		}
		return ans;
	}
};
