//あらゆるものが F2 で作られている
//qfact[k]=(2-1)(4-1)...(2^k-1)
mint qfact[vmax],qfinv[vmax];
void initqfact(){
	qfact[0]=1;
	rng(n,1,vmax)qfact[n]=qfact[n-1]*(p2[n]-1);// /(2-1)
	qfinv[vmax-1]=qfact[vmax-1].inv();
	gnr(n,1,vmax)qfinv[n-1]=qfinv[n]*(p2[n]-1);// /(2-1)
}
//2^n の部分空間であって，ランク k のものの個数
//ABC278H
mint subspace(int n,int k){
	if(n<k)return 0;
	return qfact[n]*qfinv[k]*qfinv[n-k];
}
//input[i]: i 次元ベクトル空間を固定したとする．
//そこに入るベクトルの集合であって何らかの条件を満たすもの，の個数
//output[i]: 上と同じだが，部分集合が i 次元をちゃんと span する，の個数
//ABC278H
vc<mint> getspanning(vc<mint> a){
	int n=si(a);
	rep(i,n)a[i]*=qfinv[i];
	vc<mint> w(n);
	rep(i,n)w[i]=parity(i)*mint(2).pow(i*(i-1)/2)*qfinv[i];
	auto b=multiply(move(a),w);
	b.resize(n);
	rep(i,n)b[i]*=qfact[i];
	return b;
}
//1st Ucup 13 A
//ある n 次元空間 V の中で条件を満たすベクトルの集合 X の個数を求めたい．
//a[k] は V の中の k 次元空間 S と，その中で条件を満たす X のペアの個数
//b[k] はペアをちゃんと span するものに限定したと考えたときの答え
//a[k] = sum_{0<=i<=k} subspace(n-i,k-i)*b[i] から逆算する
//b[n] を返す
mint getspanning(vc<mint> a){
	int n=si(a)-1;
	mint res=0;
	rep(i,n+1)res+=a[n-i]*parity(i)*mint(2).pow(i*(i-1)/2);
	return res;
}

//UCUP 1-18-E
//一般の q が置いてある
//uso subspace が置いてある
//0 の multiplicity が非常に怪しい
//→LTEマジック! n<mod のときはなんと正しい！
