const int V=1000000010;
using B=bitset<V/2>;
B cmp;
vi ps;
void linear_sieve(int n){
	if(2<=n)ps.pb(2);
	for(int i=3;i<=n;i+=2){
		if(!cmp[i/2])ps.pb(i);
		for(int j=1;i*ps[j]<=n;j++){
			cmp[i*ps[j]/2]=1;
			if(i%ps[j]==0)break;
		}
	}
}

const int vmax=ten(7)+10;
bool pri[vmax];
vi ps;
int sf[vmax]; //smallest factor, sf[1] is undefined(0)
void linear_sieve(){
	rng(i,2,vmax)
		pri[i]=1;
	rng(i,2,vmax){
		if(pri[i]){
			ps.pb(i);
			sf[i]=i;
		}
		for(int j=0;i*ps[j]<vmax;j++){
			pri[i*ps[j]]=0;
			sf[i*ps[j]]=ps[j];
			if(i%ps[j]==0)break;
		}
	}
}
vi get_primes(int v){
	assert(1<=v&&v<vmax);
	vi res;
	int pre=-1;
	while(v>1){
		int p=sf[v];
		if(p!=pre){
			res.pb(p);
		}
		pre=p;
		v/=p;
	}
	return res;
}
int lf[vmax]; //largest factor
void initlf(){
	rng(i,2,vmax)if(pri[i])lf[i]=i;
	else lf[i]=lf[i/sf[i]];
}
//not verified
//vector of (prime,multiplicity)
//increasing order of primes
vc<pi> fs[vmax];
void initfs(){
	rng(i,2,vmax){
		if(pri[i]){
			fs[i]={pi(i,1)};
		}else{
			fs[i]=fs[i/lf[i]];
			if(fs[i].back().a<lf[i])fs[i].eb(lf[i],0);
			fs[i].back().b++;
		}
	}
}
int mu[vmax];
vi ms;//all m s.t. mu[m] is non-zero
void initmu(){
	mu[1]=1;
	ms.pb(1);
	rng(i,2,vmax){
		if(i%(sf[i]*sf[i])==0)continue;
		mu[i]=-mu[i/sf[i]];
		if(mu[i])ms.pb(i);
	}
}
int phi[vmax];
void initphi(){
	phi[1]=1;
	rng(i,2,vmax){
		if(i%(sf[i]*sf[i])==0){
			phi[i]=phi[i/sf[i]]*sf[i];
		}else{
			phi[i]=phi[i/sf[i]]*(sf[i]-1);
		}
	}
}

vc<mint> getpowers(int n,int p){
	vc<mint> res(1+n);
	res[1]=1;
	rng(i,2,n+1){
		if(pri[i])res[i]=mint(i).pow(p);
		else res[i]=res[i/sf[i]]*res[sf[i]];
	}
	return res;
}
vc<mint> getinvpowers(int n,int p){
	vc<mint> res(1+n);
	res[1]=1;
	rng(i,2,n+1){
		if(pri[i])res[i]=mint(i).pow(p).inv();
		else res[i]=res[i/sf[i]]*res[sf[i]];
	}
	return res;
}

void load_factors(int n,vi&fs){
	fs.clear();
	int pre=-1;
	while(n>1){
		int f=sf[n];
		if(f!=pre)fs.pb(f);
		pre=f;
		n/=f;
	}
}

void load_factors_freq(int n,vc<pi>&res){
	res.clear();
	while(n>1){
		int v=sf[n];
		if(res.empty()||res.back().a!=v)res.eb(v,0);
		res.back().b++;
		n/=v;
	}
}

void load_divisors(int n,vi&res){
	res.clear();
	static vc<pi> fs;
	load_factors_freq(n,fs);
	auto dfs=[&](auto self,int i,int d)->void{
		if(i==si(fs)){
			res.pb(d);
		}else{
			rep(j,fs[i].b+1){
				self(self,i+1,d);
				d*=fs[i].a;
			}
		}
	};
	dfs(dfs,0,1);
}
