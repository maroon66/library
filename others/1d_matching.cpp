//a を全部使うマッチング
//AGC066Dの別解で使用
//UCUP3-0-E
//極力左と match していると思われる
int mincost_matching_value(vi a,vi b){
	assert(is_sorted(all(a)));
	assert(is_sorted(all(b)));
	
	int n=si(a),m=si(b);
	
	assert(n<=m);
	
	vi dp(n+m+1,inf);
	vi pre(n+m+1,-1);
	vi ab(n+m+1);
	vi from(n+m+1,-1);
	dp[0]=0;
	
	int i=0,j=0;
	rep(step,n+m+1){
		if(int k=pre[i-j+m];k!=-1){
			if(chmin(dp[step],dp[k]+abs(ab[k]-ab[step])))
				from[step]=k;
		}
		pre[i-j+m]=step;
		
		if(step<n+m){
			bool useA=true;
			if(i==n||(j<m&&a[i]>b[j]))useA=false;
			if(useA){
				ab[step+1]=ab[step]+a[i];
				i++;
			}else{
				ab[step+1]=ab[step]-b[j];
				if(chmin(dp[step+1],dp[step]))
					from[step+1]=step;
				j++;
			}
		}
	}
	return dp.back();
}
//高さが同じところで matching しています
vi mincost_matching_solution(vi a,vi b){
	assert(is_sorted(all(a)));
	assert(is_sorted(all(b)));
	
	int n=si(a),m=si(b);
	
	assert(n<=m);
	
	vi dp(n+m+1,inf);
	vi pre(n+m+1,-1);
	vi ab(n+m+1);
	vi from(n+m+1,-1);
	dp[0]=0;
	
	{
		int i=0,j=0;
		rep(step,n+m+1){
			if(int k=pre[i-j+m];k!=-1){
				if(chmin(dp[step],dp[k]+abs(ab[k]-ab[step])))
					from[step]=k;
			}
			pre[i-j+m]=step;
			
			if(step<n+m){
				bool useA=true;
				if(i==n||(j<m&&a[i]>b[j]))useA=false;
				if(useA){
					ab[step+1]=ab[step]+a[i];
					i++;
				}else{
					ab[step+1]=ab[step]-b[j];
					if(chmin(dp[step+1],dp[step]))
						from[step+1]=step;
					j++;
				}
			}
		}
	}
	vi sol(n);
	auto match=[&](int i,int j,int len){
		static vi is,js;
		is.clear();
		js.clear();
		int iend=i+len,jend=j+len;
		rep(step,len*2){
			bool useA=true;
			if(i==iend||(j<jend&&a[i]>b[j]))useA=false;
			if(useA){
				is.pb(i++);
			}else{
				js.pb(j++);
			}
			if(si(is)&&si(js)){
				sol[gpp(is)]=gpp(js);
			}
		}
		assert(i==iend);
		assert(j==jend);
	};
	{
		int i=n,j=m;
		while(i>0||j>0){
			int k=from[i+j];
			if(k==i+j-1){
				j--;
			}else{
				int len=i+j-k;
				assert(len%2==0);
				len/=2;
				i-=len;
				j-=len;
				match(i,j,len);
			}
		}
	}
	return sol;
}
mint mincost_matching_count(vi a,vi b){
	assert(is_sorted(all(a)));
	assert(is_sorted(all(b)));
	
	int n=si(a),m=si(b);
	
	assert(n<=m);
	
	vi dp(n+m+1,inf);
	vi pre(n+m+1,-1);
	vi ab(n+m+1);
	vi from(n+m+1,-1);
	dp[0]=0;
	vc<mint> way(n+m+1);
	way[0]=1;
	
	auto upd=[&](int s,int t,int add){
		if(chmin(dp[t],dp[s]+add)){
			from[t]=s;
			way[t]=0;
		}
		if(dp[t]==dp[s]+add)
			way[t]+=way[s];
	};
	
	{
		int i=0,j=0;
		rep(step,n+m+1){
			if(int k=pre[i-j+m];k!=-1){
				//if(chmin(dp[step],dp[k]+abs(ab[k]-ab[step])))
				//	from[step]=k;
				upd(k,step,abs(ab[k]-ab[step]));
			}
			pre[i-j+m]=step;
			
			if(step<n+m){
				bool useA=true;
				if(i==n||(j<m&&a[i]>b[j]))useA=false;
				if(useA){
					ab[step+1]=ab[step]+a[i];
					i++;
				}else{
					ab[step+1]=ab[step]-b[j];
					//if(chmin(dp[step+1],dp[step]))
					//	from[step+1]=step;
					upd(step,step+1,0);
					j++;
				}
			}
		}
	}
	
	return way.back();
}
