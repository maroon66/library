//全体を通して，f(s,x) が呼ばれたとき，x のサイズは s/2 であると思われる
//a(s,x) の場合は s+1 くらいのサイズかもしれないが，気にする必要はない
//つまり，x をわざわざ s に resize する，みたいなことはしないでよい

//https://nyaannyaan.github.io/library/fps/formal-power-series.hpp.html
//ABC230H
//F(g(x))=H(x) をみたす g を求めたいとする
//g=g-(F(g)-H)/F'(g) を用いてダブリング
//f(s,p(x)) -> F(p(x))-H(x), F'(p(x)) のペアを返す
//[x^0] g=0 であることになっている
//[x^0] F'(x)!=0 も必要では？
//[x^1] g(x) をヒントとして与えることで，f の引数に 0 が入ることを防ぐ
//これをしないと F' とかを使うときに微分が 0 になって壊れた
template<class F>
Poly<mint> newton_hint(int s,F f,mint hint){
	assert(s>0);
	if(s==1)return {0};
	Poly<mint> g{0,hint};
	int t=topbit(s-1);
	rep(lv,t){
		int cur=((s-1)>>(t-lv))+1;
		int nx=((s-1)>>(t-lv-1))+1;
		assert(si(g)==cur);
		auto [x,y]=f(nx,g);
		assert(si(x)==nx);
		assert(si(y)==nx);
		x*=y.inv(nx);
		x.resize(nx);
		rep(i,cur)x[i]=g[i];
		rng(i,cur,nx)x[i]=-x[i];
		g.swap(x);
	}
	return g;
}

//g(f(x))=x なる g を求める
//g(f)-x=0 として newton に渡す
//左辺を f で微分するので -x の項は消える
//UOJ Round23 C
//F(x) は定数項が 0 で一次の項が非ゼロ
//f(s,p(x)) が O(M(s)) で実行できる時，全体で O(SlogS+M(S))
//g -> g - (f(g)-x)/f'(g)
//f[0]=0,f[1]!=0 が必要かなあ
//hint は result の [x^1] の項を与えている
template<class F>
Poly<mint> compositional_inverse_hint(int s,F f,mint hint){
	return newton_hint(s,[&](int n,const Poly<mint>&x){
		assert(n>=2);
		assert(si(x)>=2);
		auto [a,b]=f(n,x);
		a[1]-=1;
		return mp(a,b);
	},hint);
}

//f(a(x)),f'(a(x)) を返す
//a のサイズが 2 以上じゃないと壊れる
template<class F>
pair<Poly<mint>,Poly<mint>> composition_dif(int s,F f,const Poly<mint>&a){
	assert(si(a)>=2&&a[0]==0&&a[1]);
	Poly<mint> fx=f(s+1,a);
	auto dx=a.dif().inv(s)*fx.dif();
	fx.resize(s);
	dx.resize(s);
	return mp(fx,dx);
}

//f(s,p(x)) はただ合成して返すだけ
//微分等を勝手に計算して compositional_inverse_hint を呼び出して頑張る
template<class F>
Poly<mint> compositional_inverse(int s,F f){
	auto fs=f(2,{0,1});
	assert(fs[0]==0&&fs[1]);
	return compositional_inverse_hint(s,[&](int n,const Poly<mint>&a){
		return composition_dif(n,f,a);
	},fs[1].inv());
}

//UOJ Round23 C
//[x^n] A(x) F(x)^k for all 0<=k<=n
//a(s,p(x)) -> x の関数として返す，サイズは s
//f(s,p(x)) -> F(p(x)) を返す，サイズは s
//F(x) は定数項が 0 で一次の項が非ゼロ
//a(s,p(x)),f(s,p(x)) が O(M(s)) で実行できる時，全体で O(NlogN+M(N))
template<class A,class F>
vc<mint> lagrange(int n,A a,F f){
	auto g=compositional_inverse(n+2,f);
	auto z=a(n+1,g);
	assert(si(z)==n+1);
	z*=g.dif();
	z.resize(n+1);
	g.erase(g.bg);
	z*=g.pow(n+1,-(n+1),invs);
	z.resize(n+1);
	reverse(all(z));
	return z;
}

//hint なし
//使い勝手がちょい悪い

//https://nyaannyaan.github.io/library/fps/formal-power-series.hpp.html
//ABC230H
//F(g(x))=H(x) をみたす g を求めたいとする
//g=g-(F(g)-H)/F'(g) を用いてダブリング
//f(s,p(x)) -> F(p(x))-H(x), F'(p(x)) のペアを返す
//[x^0] g=0 であることになっている
//[x^0] F'(x)!=0 も必要では？
template<class F>
Poly<mint> newton(int s,F f){
	Poly<mint> g{0};
	for(int n=1;n<s;n*=2){
		auto [x,y]=f(n*2,g);
		assert(si(x)==n*2);
		assert(si(y)==n*2);
		x*=y.inv(n*2);
		x.resize(n*2);
		rep(i,n)x[i]=g[i];
		rng(i,n,n*2)x[i]=-x[i];
		g.swap(x);
	}
	return g.low(s);
}

//g(f(x))=x なる g を求める
//g(f)-x=0 として newton に渡す
//左辺を f で微分するので -x の項は消える
//UOJ Round23 C
//F(x) は定数項が 0 で一次の項が非ゼロ
//f(s,p(x)) が O(M(s)) で実行できる時，全体で O(SlogS+M(S))
//g -> g - (f(g)-x)/f'(g)
//f[0]=0,f[1]!=0 が必要かなあ
template<class F>
Poly<mint> compositional_inverse(int s,F f){
	return newton(s,[&](int n,const Poly<mint>&x){
		assert(n>=2);
		auto [a,b]=f(n,x);
		a[1]-=1;
		return mp(a,b);
	});
}

//UOJ Round23 C
//[x^n] A(x) F(x)^k for all 0<=k<=n
//a(s,p(x)) -> x の関数として返す，サイズは s
//f(s,p(x)) -> F(p(x)),F'(p(x)) のペアを返す，サイズは s
//F(x) は定数項が 0 で一次の項が非ゼロ
//a(s,p(x)),f(s,p(x)) が O(M(s)) で実行できる時，全体で O(NlogN+M(N))
template<class A,class F>
vc<mint> lagrange(int n,A a,F f){
	auto g=compositional_inverse(n+2,f);
	auto z=a(n+1,g);
	assert(si(z)==n+1);
	z*=g.dif();
	z.resize(n+1);
	g.erase(g.bg);
	z*=g.pow(n+1,-(n+1),invs);
	z.resize(n+1);
	reverse(all(z));
	return z;
}

//NOT VERIFIED
/*
//compute H=F*H
//f(s,p(x)) -> F(p(x)),F'(p(x)) のペアを返す，サイズは s
//g(s,p(x)) -> G(p(x)),G'(p(x)) のペアを返す，サイズは s
//return g
//h(s,p(x)) -> H(p(x)),H'(p(x)) のペアを返す，サイズは s
template<class F,class G>
auto multiply_lambda(F f,G g){
	return [=](int s,const Poly<mint>&x){
		auto [y,z]=f(s,x);assert(si(y)==s);assert(si(z)==s);
		auto [u,v]=g(s,x);assert(si(u)==s);assert(si(v)==s);
		return mp((y*u).low(s),(y*v).low(s)+(z*u).low(s));
	};
}

//compute G=F^(-1)
//f(s,p(x)) -> F(p(x)),F'(p(x)) のペアを返す，サイズは s
//return g
//g(s,p(x)) -> G(p(x)),G'(p(x)) のペアを返す，サイズは s
template<class F>
auto inv_lambda(F f){
	return [=](int s,const Poly<mint>&x){
		auto [y,z]=f(s,x);assert(si(y)==s);assert(si(z)==s);
		auto w=y.inv(s);
		return mp(w,-(w.square().low(s)*z).low(s));
	};
}

//compute G=F^n
//f(s,p(x)) -> F(p(x)),F'(p(x)) のペアを返す，サイズは s
//return g
//g(s,p(x)) -> G(p(x)),G'(p(x)) のペアを返す，サイズは s
template<class F>
auto pow_lambda(F f,int n){
	return [=](int s,const Poly<mint>&x){
		auto [y,z]=f(s,x);assert(si(y)==s);assert(si(z)==s);
		auto w=y.pow(s,n-1,invs);
		return mp((w*y).low(s),(w*n*z).low(s));
	};
}
*/
