vvc<mint> stirling_first(int n){
	vvc<mint> x(n+1,vc<mint>(n+1));
	x[0][0]=1;
	rng(i,1,n+1){
		rng(j,1,i+1){
			x[i][j]=x[i-1][j-1]-x[i-1][j]*(i-1);
		}
	}
	return x;
}

vvc<mint> stirling_second(int n){
	vvc<mint> x(n+1,vc<mint>(n+1));
	x[0][0]=1;
	rng(i,1,n+1){
		rng(j,1,i+1){
			x[i][j]=x[i-1][j-1]+x[i-1][j]*j;
		}
	}
	return x;
}

//VERIFY: yosupo
//x(x-1)(x-2)...(x-(n-1))
//n の順列サイクルで，サイクル k 個
//上に書いてあるとおり符号がずれる
vc<mint> stirling_first(int n){
	if(n==0)return {1};
	vc<mint> a{0,1};
	int h=__lg(n),cur=1;
	per(lv,h){
		vc<mint> b(cur+1);
		b[cur]=1;
		per(i,cur)b[i]=b[i+1]*(-cur);
		rep(i,cur+1)b[i]*=finv[cur-i];
		vc<mint> c=a;
		rep(i,cur+1)c[i]*=fact[i];
		auto d=multiply(b,c);
		rep(i,cur+1)d[i]=d[cur+i]*finv[i];
		d.resize(cur+1);
		a=multiply(a,d);
		cur*=2;
		if(n&1<<lv){
			a.pb(0);
			gnr(i,1,si(a))
				a[i]=a[i-1]-a[i]*cur;
			cur++;
		}
	}
	return a;
}

//VERIFY: yosupo
//sum k {n,k} (x)_k = x^n
//n 個のものを k 個の箱にわける
//ものには番号がついているが箱にはついていない
//全部 non-empty なので箱の区別はついて，なので k! をかけてから計算，などがある
vc<mint> stirling_second(int n){
	vc<mint> a(n+1);
	rep(i,n+1)a[i]=finv[i]*(i%2?-1:1);
	vc<mint> b(n+1);
	rep(i,n+1)b[i]=mint(i).pow(n)*finv[i];
	auto c=multiply(a,b);
	c.resize(n+1);
	return c;
}
