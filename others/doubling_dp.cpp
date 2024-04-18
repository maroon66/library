//SRM778
//不要な部分には -inf を突っ込むことで全てを解決したつもりになる
vc<ll> square_maxdp(const vc<ll>&a,int k){
	assert(si(a)==2*k-1);
	vc<ll> res(2*k-1,-inf);
	rep(i,k)rep(j,k)chmax(res[i],a[i+j]+a[k-1-j]);
	rep(i,k)rep(j,k)chmax(res[k-1+i],a[k-1+i-j]+a[k-1+j]);
	return res;
}
vc<ll> advance_maxdp(const vc<ll>&a,const vc<ll>&b){
	int k=si(b)-1;
	assert(si(a)==2*k-1);
	vc<ll> res(2*k-1,-inf);
	rep(i,2*k-2)res[i]=a[i+1];
	rng(i,1,k+1)chmax(res[2*k-2],a[2*k-1-i]+b[i]);
	return res;
}

//trans[i]=i 進むときの bonus
//trans[0] の値はなんでもいいのかな（未確認），一応 -inf とかだと安全
//dp[x-(k-1)] - dp[x+(k-1)] の入った vector を返す
vc<ll> doubling_dp(int x,const vc<ll>&trans){
	int k=si(trans)-1;
	if(x==0){
		vc<ll> res(2*k-1,-inf);
		rep(i,k)res[k-1+i]=trans[i];
		res[k-1]=0;
		return res;
	}
	int h=x/2;
	auto w=square_maxdp(doubling_dp(h,trans),k);
	if(x%2)return advance_maxdp(w,trans);
	else return w;
}
