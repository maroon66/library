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

const int vmax=ten(5)+10;
const int L=20;
vi fs;
mint fact[vmax],finv[vmax];
int cnt[vmax][L],waf[vmax];
mint ps[L][vmax*10];
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
		rng(j,1,vmax*10)ps[i][j]=ps[i][j-1]*fs[i];
	}
}
mint choose(int n,int k){
	mint res=fact[n]*finv[n-k]*finv[k];
	rep(i,si(fs))res*=ps[i][cnt[n][i]-cnt[n-k][i]-cnt[k][i]];
	return res;
}

ll fdiv(ll a, ll b) { // floored division
	return a / b - ((a ^ b) < 0 && a % b); }
		
mint slv(int n,int r){
	mint res;
	rep(i,n+1){
		mint w;
		int lim=(i%2==0)?min(r/2*2,i):min(fdiv(r-1,2)*2+1,i);
		w+=choose(i,i/2);
		if(lim<i){
			w-=choose(i,(i-lim)/2-1);
		}
		res+=choose(n,i)*w;
	}
	dmp(res);
	return res;
}

signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	
	int n;cin>>n;
	cin>>base.mod;
	fs=factors(base.mod);
	int l,r;cin>>l>>r;
	
	initfact();
	
	mint ans=slv(n,r);
	if(l)ans-=slv(n,l-1);
	print(ans);
}
