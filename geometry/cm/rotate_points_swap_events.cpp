//CF445D
signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	
	int n;cin>>n;
	if(n<=2){
		print(-1);
		return 0;
	}
	vc<cm> a(n);
	rep(i,n){
		int p,q;cin>>p>>q;
		a[i]=cm(p,q);
	}
	
	vi idx(n);iota(all(idx),0);
	sort(all(idx),[&](int i,int j){return cmless(a[i],a[j]);});
	reverse(all(idx));
	vi pos(n);
	rep(i,n)pos[idx[i]]=i;
	
	struct T{
		cm v;
		ll a,b;
		int i,j;
		bool operator<(const T&r)const{
			int s=argcmp(v,r.v);
			if(s)return s<0;
			if(a!=r.a)return a<r.a;
			return b<r.b;
		}
	};
	vc<T> evs;evs.reserve(n*(n-1));
	rep(i,n)rep(j,n)if(i!=j){
		cm dir=normdir(a[j]-a[i]);
		evs.pb({-rot90(dir),dot(a[i],dir),dot(a[j],dir),i,j});
	}
	
	sort(all(evs));
	evs.resize(n*(n-1)/2);
	
	ysp mk;
	
	vi now(n);
	auto add=[&](int i){
		chmin(i,n-1-i);
		if(now[i])return;
		mk.add(cm(a[idx[i]]+a[idx[n-1-i]]));
		now[i]=1;
	};
	auto rem=[&](int i){
		chmin(i,n-1-i);
		if(!now[i])return;
		mk.rem(cm(a[idx[i]]+a[idx[n-1-i]]));
		now[i]=0;
	};
	
	rep(i,n)add(i);
	
	vc<cm> ans;
	for(auto eve:evs){
		int i=eve.i,j=eve.j;
		i=pos[i],j=pos[j];
		assert(i-1==j);
		rem(i);
		rem(j);
		swap(idx[i],idx[j]);
		swap(pos[idx[i]],pos[idx[j]]);
		add(i);
		add(j);
		cm tmp;
		if(mk.val(tmp))ans.pb(tmp);
	}
	
	sort(all(ans),cmless);
	ans.erase(unique(all(ans),cmeq),ans.ed);
	print(si(ans));
}
