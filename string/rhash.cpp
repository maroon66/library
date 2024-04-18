
//ABC135F
//Yukicoder No.263
//https://codeforces.com/gym/103698/problem/H
namespace rhash{
	const int k=2;
	vc<array<mint,k>> a,b;
	void init(int n){
		a.resize(n+1);
		b.resize(n+1);
		rep(i,k){
			a[0][i]=1;
			a[1][i]=rand_int(1,mint::mod-1);
			b[0][i]=1;
			b[1][i]=a[1][i].inv();
		}
		rng(i,1,n)
			rep(j,k){
				a[i+1][j]=a[i][j]*a[1][j];
				b[i+1][j]=b[i][j]*b[1][j];
			}
	}
	using P=pair<array<mint,k>,int>;
	P mrg(P x,const P&y){
		rep(i,k)
			x.a[i]+=y.a[i]*a[x.b][i];
		x.b+=y.b;
		return x;
	}
	template<class...Args>
	P mrg(const P&x,const P&y,Args...args){
		return mrg(mrg(x,y),forward<Args>(args)...);
	}
	//x is a prefix of y
	P dif(const P&x,P y){
		rep(i,k){
			y.a[i]-=x.a[i];
			y.a[i]*=b[x.b][i];
		}
		y.b-=x.b;
		return y;
	}
	template<class t>
	P single(t c){
		P res;
		res.b=1;
		rep(i,k)res.a[i]=c;
		return res;
	}
	P hpow(P p,int n){
		P res;
		while(n){
			if(n&1)res=mrg(res,p);
			p=mrg(p,p);
			n>>=1;
		}
		return res;
	}
	struct gen{
		gen(){}
		vc<array<mint,k>> c;
		template<class S>
		gen(S s):c(s.size()+1){
			rep(i,s.size())
				rep(j,k)
					c[i+1][j]=c[i][j]+a[i][j]*s[i];
		}
		P get(int l,int r){
			P res;
			rep(i,k)
				res.a[i]=(c[r][i]-c[l][i])*b[l][i];
			res.b=r-l;
			return res;
		}
	};
}
