template<class t,int L>
struct subset_series{
	using A=array<t,L+1>;
	A x[2][1<<L],y[1<<L];
	int getn(const vc<t>&a){
		int n=__lg(si(a));
		assert(si(a)==(1<<n));
		assert(n<=L);
		return n;
	}
	void sety(const vc<t>&g){
		int n=getn(g);
		rep(k,n){
			rep(bit,1<<k){
				rep(i,k+1)y[1<<k|bit][i]=0;
				y[1<<k|bit][popcount(bit)]=g[1<<k|bit];
			}
			rep(i,k)rep(bit,1<<k)if((bit&1<<i)==0){
				add(k,y[1<<k|bit|1<<i],y[1<<k|bit]);
			}
		}
	}
	void setx(const vc<t>&f){
		int n=getn(f),cur=n&1;
		rep(bit,1<<n)x[cur][bit]={};
		rep(bit,1<<n)x[cur][bit][popcount(bit)]=f[bit];
		rep(i,n)rep(bit,1<<n)if((bit&1<<i)==0){
			add(n,x[cur][bit|1<<i],x[cur][bit]);
		}
		rep(bit,1<<n){
			mint w=parity(popcount(mask(n)^bit));
			rep(i,n+1)x[cur][bit][i]*=w;
		}
	}
	vc<t> getx(int n){
		int cur=n&1;
		rep(i,n)rep(bit,1<<n)if((bit&1<<i)==0){
			sub(n,x[cur][bit|1<<i],x[cur][bit]);
		}
		vc<t> res(1<<n);
		rep(bit,1<<n)res[bit]=x[cur][bit][popcount(bit)];
		return res;
	}
	//a+=b
	void add(int k,A&a,const A& b){
		rep(i,k+1)a[i]+=b[i];
	}
	//a-=b
	void sub(int k,A&a,const A& b){
		rep(i,k+1)a[i]-=b[i];
	}
	//a+=b*c*x (deg(a)=deg(b)=K, deg(c)=K+1)
	void mult(int k,A&a,const A&b,const A&c){
		rep(i,k+1)rep(j,k-i+1)a[i+j+1]+=b[i]*c[j];
	}
	//deg(b)=k+1, b の 0 項目を削る
	void add2(int k,A&a,const A&b){
		rep(i,k+1)a[i]+=b[i+1];
	}
	//a+=b*c
	void mult2(int k,A&a,const A&b,const A&c){
		rep(i,k+1)rep(j,k-i+1)a[i+j]+=b[i]*c[j];
	}
	//f(x): 1 変数関数
	//g: set power series
	//f(g)
	//F(k,v)= f の k 回微分に v を代入した結果
	//stress-tested
	template<class F>
	vc<t> composite_func(F f,const vc<t>&g){
		int n=getn(g);
		sety(g);
		rep(i,n+1)x[0][i][0]=f(i,g[0]);
		rep(k,n){
			int cur=k&1,nx=cur^1;
			//memset(x[nx],0,(n-k)*(1<<(k+1))*(L+1)*sizeof(t));
			rep(i,(n-k)<<(k+1))x[nx][i]={};
			rep(p,n-k){
				rep(bit,1<<k){
					add(k,x[nx][p<<(k+1)|bit],x[cur][p<<k|bit]);
					add(k,x[nx][p<<(k+1)|1<<k|bit],x[cur][p<<k|bit]);
					mult(k,x[nx][p<<(k+1)|1<<k|bit],x[cur][(p+1)<<k|bit],y[1<<k|bit]);
				}
			}
		}
		return getx(n);
	}
	//stress-tested
	vc<t> composite_poly(const vc<t>&f,const vc<t>&g){
		return composite_func([&](int k,t v)->t{
			t sum=0;
			rng(i,k,si(f)){
				t w=f[i];
				rep(j,k)w*=i-j;
				rep(j,i-k)w*=v;
				sum+=w;
			}
			return sum;
		},g);
	}
	//stress-tested
	//N=20 850ms(local)
	vc<t> exp(const vc<t>&g){
		int n=getn(g);
		assert(g[0]==0);
		sety(g);
		x[0][0][0]=1;
		rep(k,n){
			int cur=k&1,nx=cur^1;
			rep(i,1<<(k+1))x[nx][i]={};
			rep(bit,1<<k){
				add(k,x[nx][bit],x[cur][bit]);
				add(k,x[nx][1<<k|bit],x[cur][bit]);
				mult(k,x[nx][1<<k|bit],x[cur][bit],y[1<<k|bit]);
			}
		}
		return getx(n);
	}
	//stress-tested
	//N=20, 1200ms(local)
	vc<t> log(const vc<t>&g){
		assert(g[0]==1);
		return composite_func([&](int k,t v)->t{
			assert(v==1);
			if(k==0)return 0;
			t res=1;
			rep(i,k-1)res=-res*(i+1);
			return res;
		},g);
	}
	//ABC306H
	vc<t> inv(const vc<t>&g){
		assert(g[0]);
		return composite_func([&](int k,t v)->t{
			return v.pow(-k)*parity(k)*fact[k];
		},g);
	}
	//[x1x2...xn] f*g^k
	//for all 0<=k<=n
	//explike の場合は 1/k! される (g[0]==0)
	//つまり，ちょうど K 個の partition に対応するような値を求めている
	//stress-tested
	//N=20 1240ms(local)
	vc<t> fgk(const vc<t>&f,const vc<t>&g,bool explike=false){
		int n=getn(f);
		vc<t> res(n+1);
		assert(si(g)==(1<<n));
		setx(f);
		sety(g);
		per(k,n){
			int cur=k&1,nx=cur^1;
			rep(i,(n-k+1)<<k)x[cur][i]={};
			rep(p,n-k){
				rep(bit,1<<k){
					add2(k,x[cur][p<<k|bit],x[nx][p<<(k+1)|bit]);
					add2(k,x[cur][p<<k|bit],x[nx][p<<(k+1)|1<<k|bit]);
					mult2(k,x[cur][(p+1)<<k|bit],x[nx][p<<(k+1)|1<<k|bit],y[1<<k|bit]);
				}
			}
		}
		if(explike){
			assert(g[0]==0);
			rep(k,n+1)res[k]=x[0][k][0];
		}else{
			rep(k,n+1){
				rng(i,k,n+1){
					t v=x[0][k][0];
					rep(j,k)v*=i-j;
					rep(j,i-k)v*=g[0];
					res[i]+=v;
				}
			}
		}
		return res;
	}
};
subset_series<mint,20> ss;
