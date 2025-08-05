vi factors(int x){
	vi res;
	for(int i=2;i*i<=x;i+=(i==2?1:2)){
		if(x%i==0){
			res.pb(i);
			while(x%i==0)
				x/=i;
		}
	}
	if(x>1)res.pb(x);
	sort(all(res));
	return res;
}

//extgcd を貼って modint の inv を書き換える!
//luogu MX-X10 E
const int vmax=2*ten(5)+10;
const vi fs=factors(base.mod);
const int L=5; //si(fs)
mint fact[vmax],finv[vmax];
int cnt[vmax][L],waf[vmax];
mint ps[L][vmax];
void initfact(){
	fact[0]=1;
	rng(i,1,vmax){
		int x=i;
		rep(j,si(fs)){
			cnt[i][j]=cnt[i-1][j];
			while(x%fs[j]==0){
				cnt[i][j]++;
				x/=fs[j];
			}
		}
		fact[i]=fact[i-1]*x;
		waf[i]=x;
	}
	finv[vmax-1]=fact[vmax-1].inv();
	for(int i=vmax-2;i>=0;i--){
		finv[i]=finv[i+1]*waf[i+1];
	}
	rep(i,si(fs)){
		ps[i][0]=1;
		rng(j,1,vmax)ps[i][j]=ps[i][j-1]*fs[i];
	}
}
mint choose(int n,int k){
	mint res=fact[n]*finv[n-k]*finv[k];
	rep(i,si(fs))res*=ps[i][cnt[n][i]-cnt[n-k][i]-cnt[k][i]];
	return res;
}
