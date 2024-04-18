//min-25's blog Polynomial Sum

//VERIFTY:yosupo
// f は k 次多項式を 0 から k で評価した値が入る
//sum {0<=i<=n-1} a^i f[i]
//O(N)
mint exp_poly_sum(const mint a,const vc<mint>&f,const int n){
	if(n==0)return 0;
	int k=si(f)-1;
	if(a==0){
		return f[0];
	}else if(a==1){
		vc<mint> g(k+2);
		rep(i,k+1)g[i+1]=g[i]+f[i];
		return interpolate(g,n);
	}else{
		vc<mint> g(k+1);
		{
			mint r=1;
			rep(i,k+1){
				g[i]=f[i]*r;
				r*=a;
			}
		}
		mint c;
		{
			mint w,r=1;
			rep(i,k+1){
				w+=choose(k+1,i)*r;
				r*=-a;
				c+=g[k-i]*w;
			}
			c/=mint(1-a).pow(k+1);
		}
		rep(i,k)g[i+1]+=g[i];
		mint ainv=a.inv(),w=1;
		rep(i,k+1){
			g[i]=(g[i]-c)*w;
			w*=ainv;
		}
		return interpolate(g,n-1)*a.pow(n-1)+c;
	}
}

// f は k 次多項式を 0 から k で評価した値が入る
//sum {0<=i<=inf} a^i f[i]
//O(N)
mint exp_poly_sum_limit(const mint a,const vc<mint>&f){
	assert(a!=1);
	int k=si(f)-1;
	if(a==0){
		return f[0];
	}else{
		vc<mint> g(k+1);
		{
			mint r=1;
			rep(i,k+1){
				g[i]=f[i]*r;
				r*=a;
			}
		}
		mint c;
		{
			mint w,r=1;
			rep(i,k+1){
				w+=choose(k+1,i)*r;
				r*=-a;
				c+=g[k-i]*w;
			}
			c/=mint(1-a).pow(k+1);
		}
		return c;
	}
}

//exp(ax)*f の k 次の係数に k! をかけたもの
//の sum が求まってしまう
//O(N)
//ちなみに exp(ax)*x^k は k!/(1-ax)^(k+1) になる
mint exp_gen_coeff_sum(mint a,vc<mint> f){
	mint ans;
	mint w=(1-a).inv(),cur=1;
	rep(k,si(f)){
		cur*=w;
		ans+=fact[k]*cur*f[k];
	}
	return ans;
}

//CF 789 F
// f は k 次多項式を 0 から k で評価した値が入る
//sum {0<=i<=n-1} a^i f[i]
//for all n in xs
//O(Klog^2K + N log^2 N)
vc<mint> exp_poly_sum_many(const mint a,const vc<mint>&f,vi xs){
	int k=si(f)-1;
	if(a==0){
		assert(false);
	}else if(a==1){
		assert(false);
	}else{
		vc<mint> g(k+1);
		{
			mint r=1;
			rep(i,k+1){
				g[i]=f[i]*r;
				r*=a;
			}
		}
		mint c;
		{
			mint w,r=1;
			rep(i,k+1){
				w+=choose(k+1,i)*r;
				r*=-a;
				c+=g[k-i]*w;
			}
			c/=mint(1-a).pow(k+1);
		}
		rep(i,k)g[i+1]+=g[i];
		mint ainv=a.inv(),w=1;
		rep(i,k+1){
			g[i]=(g[i]-c)*w;
			w*=ainv;
		}
		vc<mint> tmp(k+1);
		rep(i,k+1)tmp[i]=i;
		Poly z=interpolate(tmp,g);
		vc<mint> use(si(xs));
		rep(i,si(xs))use[i]=xs[i]-1;
		vc<mint> res=multieval(z,use);
		rep(i,si(xs)){
			res[i]=res[i]*a.pow(xs[i]-1)+c;
		}
		return res;
	}
}

//https://codeforces.com/blog/entry/102631?#comment-910422
//CF 789 F (a>=2)
//stress-tested (a=1)
// f は k 次多項式の係数列
//sum {0<=i<=n-1} a^i f(i)
//for all n in xs
//O(K log K + N log^2 N)
vc<mint> exp_poly_sum_many_faster(const mint a,const vc<mint>&f,vi xs){
	int k=si(f)-1;
	if(a==0){
		assert(false);
	}else if(a==1){
		Poly<mint> num(k+1);
		rep(i,k+1)num[i]=fact[k-i]*f[k-i];
		Poly<mint> den(k+2);
		rep(i,k+2)den[i]+=finv[i+1];
		Poly<mint> w=(num*den.inv(k+2)).low(k+2);
		reverse(all(w));
		w[0]=0;
		rng(i,1,k+2)w[i]*=finv[i];
		return multieval(w,vc<mint>(all(xs)));
	}else{
		Poly<mint> num(k+1);
		rep(i,k+1)num[i]=fact[k-i]*f[k-i];
		Poly<mint> den(k+1);
		den[0]=-1;
		rep(i,k+1)den[i]+=finv[i]*a;
		Poly<mint> w=(num*den.inv(k+1)).low(k+1);
		reverse(all(w));
		rep(i,k+1)w[i]*=finv[i];
		Poly<mint> res=multieval(w,vc<mint>(all(xs)));
		rep(i,si(res))res[i]=res[i]*a.pow(xs[i])-w[0];
		return res;
	}
}
