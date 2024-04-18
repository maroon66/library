//https://judge.yosupo.jp/problem/mul_mod2n_convolution
//O(N log N)
vc<mint> convolute_multiplicative_2n(const vc<mint>&a,const vc<mint>&b){
	int s=si(a);assert(si(b)==s);
	int n=topbit(s);assert(s==(1<<n));
	const int m=mask(n);
	auto nx=[&](int v){
		return (v*5)&m;
	};
	auto head=[&](int ord){
		return m^mask(n-ord);
	};
	vc<mint> c(s/4),d(s/4);
	
	//x の要素を ord(2) で分解する
	// 0*2^(n-1)
	// 1*2^(n-2)
	// :
	// inf*1
	//とデータを整列しておく
	//各系列では，-1,5 での巡回を 2 次元 FFT で見たときのデータが入っている
	//5,-1 の順でバタフライして，あと適切にデータを並び替えておく(fmt が bit-reverse 必須)
	//畳み込みの際，2 の位数の関係で 5,-1 の情報が潰れることがあるが，
	//ちょうど先頭 2^k 個を見れば済むような形にデータがまとまる
	auto butterfly_forward=[&](const vc<mint>&x){
		vc<mint> y(s);
		rep(ord,n-1){
			int len=s>>(ord+2);
			c.resize(len);
			d.resize(len);
			int p=1<<ord;
			rep(i,len){
				c[i]=x[p];
				d[i]=x[s-p];
				p=nx(p);
			}
			inplace_fmt(c,false);
			inplace_fmt(d,false);
			rep(i,len){
				mint v=c[i].v,w=d[i].v;
				y[head(ord)+i*2]=v+w;
				y[head(ord)+i*2+1]=v-w;
			}
		}
		if(n>0)y[head(n-1)]=x[1<<(n-1)];
		y[head(n)]=x[0];
		return y;
	};
	auto butterfly_inverse=[&](const vc<mint>&y){
		const mint inv2=mint(2).inv();
		vc<mint> x(s);
		rep(ord,n-1){
			int len=s>>(ord+2);
			c.resize(len);
			d.resize(len);
			rep(i,len){
				mint v=y[head(ord)+i*2],w=y[head(ord)+i*2+1];
				c[i]=(v+w)*inv2;
				d[i]=(v-w)*inv2;
			}
			inplace_fmt(c,true);
			inplace_fmt(d,true);
			int p=1<<ord;
			rep(i,len){
				x[p]=c[i];
				x[s-p]=d[i];
				p=nx(p);
			}
		}
		if(n>0)x[1<<(n-1)]=y[head(n-1)];
		x[0]=y[head(n)];
		return x;
	};
	
	auto e=butterfly_forward(a);
	auto f=butterfly_forward(b);
	vc<mint> g(s);
	
	rep(x,n+1)rep(y,n+1){
		if(x+y>=n){
			g[head(n)]+=e[head(x)]*f[head(y)];
		}else{
			int len=s>>(x+y+1);
			rep(i,len)g[head(x+y)+i]+=e[head(x)+i]*f[head(y)+i];
		}
	}
	
	return butterfly_inverse(g);
}
