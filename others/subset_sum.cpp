const int nmax=2010;
const int smax=2010;
int dp[nmax][smax*2];

//ABC221G
//使うアイテムの番号の最大値を最小化（未Verify）
pair<bool,vc<bool>> subset_sum(vi a,int tar){
	if(tar<0||accumulate(all(a),0)<tar)return mp(false,vc<bool>());
	int n=si(a);assert(n<=nmax);
	int s=*max_element(all(a));assert(s<=smax);
	vi x,y;
	{
		int tot=0,tmp=tar;
		rep(i,n){
			tot+=a[i];
			if(tot<=tar){
				x.pb(a[i]);
				tmp=tar-tot;
			}else y.pb(a[i]);
		}
		tar=tmp;
	}
	int p=si(x),q=si(y);
	rep(i,p+1)rep(j,2*s)
		dp[i][j]=q+1;
	dp[0][0]=0;
	rep(i,p+1){
		rep(j,2*s){
			int mx=q;if(i)chmin(mx,dp[i-1][j]);
			rng(u,dp[i][j],mx){
				if(j+y[u]<2*s){
					chmin(dp[i][j+y[u]],u+1);
				}
			}
		}
		if(i<p){
			rep(j,2*s)dp[i+1][j]=dp[i][j];
			rng(j,x[i],2*s)chmin(dp[i+1][j-x[i]],dp[i][j]);
		}
	}
	vc<bool> used(n);
	rep(i,p)used[i]=true;
	int i=p,j=tar;
	if(dp[i][j]==q+1)return mp(false,vc<bool>());
	while(i||j){
		if(dp[i][j]>0){
			int u=dp[i][j]-1;
			if(j>=y[u]&&dp[i][j-y[u]]<=u){
				used[p+u]=true;
				j-=y[u];
				continue;
			}
		}
		if(i>0){
			if(dp[i][j]==dp[i-1][j]){
				i--;
				continue;
			}
			if(j+x[i-1]<2*s&&dp[i][j]==dp[i-1][j+x[i-1]]){
				used[--i]=false;
				j+=x[i];
				continue;
			}
		}
		assert(false);
	}
	return mp(true,used);
}

//tar 以上  tar+2 max(a) -1 以下の値について可能かどうかのテーブルを返す
//Ptz 2022 W Day4 E (nearest_half)
//全 false のケースとかは verify してない
vc<bool> subset_sum(vi a,int tar){
	sort(all(a));
	int s=*max_element(all(a));
	vi x,y;
	int tot=0;
	for(auto v:a){
		if(tot<tar){
			x.pb(v);
			tot+=v;
		}else y.pb(v);
	}
	int ini=tot-tar;
	if(!inc(0,ini,2*s-1))return vc<bool>(2*s,0);
	int p=si(x),q=si(y);
	vi dp(2*s,q+1),pre(2*s,q+1);
	dp[ini]=0;
	rep(i,p+1){
		rep(j,2*s){
			int mx=min(pre[j],q);
			rng(k,dp[j],mx){
				int to=j+y[k];
				if(to<2*s)chmin(dp[to],k+1);
				else break;
			}
			pre[j]=dp[j];
		}
		if(i<p)rng(j,x[i],2*s)chmin(dp[j-x[i]],pre[j]);
	}
	vc<bool> res(2*s);
	rep(i,2*s)if(dp[i]<=q)res[i]=true;
	return res;
}

//a を 2 つに分割することを考えて，できるだけ和が近くなるようにする．
//大きい方の和を返す
//Ptz 2022 W Day4 E
int nearest_half(vi a){
	int tot=accumulate(all(a),int(0));
	int h=(tot+1)/2;
	auto res=subset_sum(a,h);
	rep(i,si(res))if(res[i])return h+i;
	assert(false);
}
