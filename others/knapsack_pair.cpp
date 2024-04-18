//(a,b) ペアについては，a を使ったら b が使用可能になる
//c は自由に使用可能
//という条件の元で価値最大化 knapsack
//res[i] = i 個使うときの max value
//stress-tested
vi knapsack_pair_max(const vc<pi>&ab,const vi&c){
	const int none=-inf;
	vc<pi> ls;
	for(auto [a,b]:ab){
		if(a<b)ls.eb(a,b);
		else{
			ls.eb(a,none);
			ls.eb(b,none);
		}
	}
	for(auto v:c){
		ls.eb(v,none);
	}
	auto eval=[&](pi x){
		if(x.b==none)return x.a*2;
		else return x.a+x.b;
	};
	sort(all(ls),[&](pi x,pi y){
		return eval(x)>eval(y);
	});
	int n=si(ls);
	vi mn(n+1,inf),mx(n+1,-inf);
	rep(i,n){
		mn[i+1]=mn[i];
		if(ls[i].b==none)chmin(mn[i+1],ls[i].a);
		else chmin(mn[i+1],ls[i].b);
	}
	per(i,n){
		mx[i]=mx[i+1];
		if(ls[i].b==none)chmax(mx[i],ls[i].a);
		else chmax(mx[i],ls[i].a);
	}
	vi res{0};
	rep(i,n){
		auto [a,b]=ls[i];
		if(b==none){
			res.pb(res.back()+a);
		}else{
			int x=res.back(),y=x+a+b;
			res.pb(max(x+mx[i],y-mn[i+1]));
			res.pb(y);
		}
	}
	return res;
}
//min ver
//stress-tested
//IOI2023 1A
vi knapsack_pair_min(vc<pi> ab,const vi&c){
	int sum=SUM(c);
	for(auto&[a,b]:ab){
		sum+=a+b;
		swap(a,b);
	}
	vi res=knapsack_pair_max(ab,c);
	rein(res);
	for(auto&v:res)v=sum-v;
	return res;
}
