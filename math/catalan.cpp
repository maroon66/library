//(0,h),(w,0)から伸びる直線で inclusive に囲まれる領域内を進み，x,y に至る方法)
//(x,y) が領域からはみ出ていた場合はもちろん 0
mint catalan_super(int h,int w,int x,int y){
	if(inc(-w,y-x,h)){
		int s=x+y,m=h+w+2;
		mint res=0;
		for(int z=y%m;z<=s;z+=m)res+=choose(s,z);
		for(int z=(x+(h+1))%m;z<=s;z+=m)res-=choose(s,z);
		return res;
	}else{
		return 0;
	}
}

//整数 x=0 を持っている
//1 ステップごとに x +-= 1 する
//0<=x<=k が常に成立
//i (0<=i<=n) ステップ後に x=k となる操作方法は何通り?
//up,down の重みが a,b になる
//stress-tested
vc<mint> clamped_path_top(int n,int k,mint a,mint b){
	++n;
	Poly<mint> x(k+1);
	x[k]=a.pow(k);
	Poly<mint> y(k+2);
	y[0]=1;
	mint w=1;
	for(int i=1;i*2-1<=k;i++){
		w*=a*b;
		y[i*2]=-choose(k-(i-1),i)*w*parity(i-1);
	}
	auto z=x*y;z.resize(k+1);
	auto ans=z*y.inv(n);ans.resize(n);
	return ans;
}
//整数 x=0 を持っている
//1 ステップごとに x +-= 1 する
//0<=x<=k が常に成立
//i (0<=i<=n) ステップの操作方法は何通り?
//up,down の重みが a,b になる
//stress-tested
vc<mint> clamped_path(int n,int k,mint a,mint b){
	++n;
	Poly<mint> x(k+1);
	x[0]=1;
	{
		mint w=b;
		rep(i,k){
			x[i+1]=x[i]*(a+b);
			if(i%2==0){
				x[i+1]-=catalan(i/2)*w;
				w*=a*b;
			}
		}
	}
	Poly<mint> y(k+2);
	y[0]=1;
	mint w=1;
	for(int i=1;i*2-1<=k;i++){
		w*=a*b;
		y[i*2]=-choose(k-(i-1),i)*w*parity(i-1);
	}
	auto z=x*y;z.resize(k+1);
	auto ans=z*y.inv(n);ans.resize(n);
	return ans;
}
