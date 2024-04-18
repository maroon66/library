//Codechef 2021 August Lunchtime SUMRECT
//return [x^k] a/b
//b[0]!=0
//こう，線形漸化式の気持ちになって berlekamp massey のときみたいにやる
//k=n,n+1,n+2...n+m について求める，等も同じ要領でできるはず
mint get_kth_div(const Poly<mint>&a,Poly<mint> b,int k){
	int s=max(si(a),si(b)-1);
	auto c=a*b.inv(s);c.resize(s);
	b.resize(s+1);
	reverse(all(b));
	auto d=Poly<mint>{0,1}.pow_mod(k,b);
	mint res;
	rep(i,si(d))res+=c[i]*d[i];
	return res;
}

//f(x)/g(x) = f(x)g(-x)/g(x)g(-x) で 偶奇でどんどん決定していく．
//Codechef 2021 August Lunchtime SUMRECT
//上と同じ，ただし定数倍は速い．
//そのかわりkをちょっとずらすとかはできない
mint get_kth_div_fast(Poly<mint> a,Poly<mint> b,int k){
	int s=1;while(s<max(si(a),si(b)))s*=2;
	a.resize(s*2);
	b.resize(s*2);
	Poly<mint> c(s*2);
	while(s<=k){
		fill(all(c),mint(0));
		rep(i,s){
			c[i]=b[i];
			if(i&1)c[i]=-c[i];
		}
		inplace_fmt(a,false);
		inplace_fmt(b,false);
		inplace_fmt(c,false);
		rep(i,2*s)a[i]*=c[i];
		rep(i,2*s)b[i]*=c[i];
		inplace_fmt(a,true);
		inplace_fmt(b,true);
		if(k%2==0){
			rep(i,s)a[i]=a[i*2];
			rng(i,s,2*s)a[i]=0;
		}else{
			rep(i,s)a[i]=a[i*2+1];
			rng(i,s,2*s)a[i]=0;
		}
		rep(i,s)b[i]=b[i*2];
		rng(i,s,s*2)b[i]=0;
		k/=2;
	}
	a.resize(s);
	b.resize(s);
	auto d=a*b.inv(s);
	return d[k];
}

//f(x)/g(x) = f(x)g(-x)/g(x)g(-x) で 偶奇でどんどん決定していく．
//Osijek 2023 Day2 B (mod 10^9+9)
//上と同じ，ただし定数倍は速い．
//そのかわりkをちょっとずらすとかはできない
mint get_kth_div_fast(Poly<mint> a,Poly<mint> b,int k){
	int s=max(si(a),si(b));
	a.resize(s);
	b.resize(s);
	while(s<=k){
		auto c=b;
		rep(i,s)if(i&1)c[i]=-c[i];
		chmult(a,c,2*s);
		chmult(b,c,2*s);
		if(k%2==0){
			rep(i,s)a[i]=a[i*2];
		}else{
			rep(i,s)a[i]=a[i*2+1];
		}
		rep(i,s)b[i]=b[i*2];
		k/=2;
		a.resize(s);
		b.resize(s);
	}
	auto d=a*b.inv(s);
	return d[k];
}
