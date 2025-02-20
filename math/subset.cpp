//CF Refact1 H
//verifyコードもそこにある
namespace subset{
const int nmax=20;
int pcnt[1<<nmax];
struct pcntinit{
	pcntinit(){
		rep(i,1<<nmax)pcnt[i]=popcount(i);
	}
} pcntinit_;

//N=20 で手元 0.75 sec くらい
//Verify yosupo (https://judge.yosupo.jp/submission/252019)
template<class t>
vc<t> subset_convolution(const vc<t>&a,const vc<t>&b){
	static_assert(t::mod<(1<<30));
	int s=si(a);
	assert(si(b)==s);
	int n=__lg(s);
	assert(s==(1<<n));
	assert(n<=nmax);
	vc<array<t,nmax>> x(s),y(s);
	rep(i,s-1)x[i][pcnt[i]]=a[i];
	rep(k,n)rep(i,s)if((i&1<<k)==0)rep(j,pcnt[i]+1)x[i|1<<k][j]+=x[i][j];
	rep(i,s-1)y[i][pcnt[i]]=b[i];
	rep(k,n)rep(i,s)if((i&1<<k)==0)rep(j,pcnt[i]+1)y[i|1<<k][j]+=y[i][j];
	/*rep(i,s){
		mint org=x[i][pcnt[i]];
		per(j,pcnt[i]+1)gnr(k,pcnt[i]-j,min(pcnt[i],n-1-j)+1)
			x[i][j+k]+=x[i][j]*y[i][k];
		x[i][pcnt[i]]-=org;
	}*/
	{//static modint 専用高速化
		#ifdef DYNAMIC_MOD
		static_assert(false);
		#endif
		static_assert(t::mod<(1<<30));
		const int D=16;
		ull buf[nmax]{};
		rep(i,s){
			rng(j,pcnt[i],min(pcnt[i]*2,n-1)+1)buf[j]=0;
			if(pcnt[i]+1<=D){
				rep(j,pcnt[i]+1)rng(k,pcnt[i]-j,min(pcnt[i],n-1-j)+1)
					buf[j+k]+=(ull)x[i][j].v*y[i][k].v;
			}else{
				rep(j,D)rng(k,pcnt[i]-j,min(pcnt[i],n-1-j)+1)
					buf[j+k]+=(ull)x[i][j].v*y[i][k].v;
				rng(j,pcnt[i],min(pcnt[i]*2,n-1)+1)buf[j]%=t::mod;
				rng(j,D,pcnt[i]+1)rng(k,pcnt[i]-j,min(pcnt[i],n-1-j)+1)
					buf[j+k]+=(ull)x[i][j].v*y[i][k].v;
			}
			rng(j,pcnt[i],min(pcnt[i]*2,n-1)+1)x[i][j].v=buf[j]%t::mod;
		}
	}
	rep(k,n)rep(i,s)if((i&1<<k)==0)rng(j,pcnt[i],n)x[i|1<<k][j]-=x[i][j];
	vc<t> res(s);
	rep(i,s-1)res[i]=x[i][pcnt[i]];
	rep(i,s)res[s-1]+=a[i]*b[s-1-i];
	return res;
}

template<class t>
struct online_subset_sum{
	int n,cur;
	vc<t> x;
	online_subset_sum(int nn):n(nn),cur(0),x(1<<n){
		assert(n<=nmax);
	}
	//bit mask が cur の proper subset であるような値の sum を出力
	//get,push,get,push... の順で呼ぶことを想定
	t get(){
		t res{};
		rep(i,n)if(cur&1<<i)res+=x[cur^1<<i];
		return res;
	}
	void push(t v){
		x[cur]=v;
		int k=botbit(++cur);
		rep(lv,k){
			int w=1<<lv;
			rng(i,cur-w*2,cur-w)x[i+w]+=x[i];
		}
	}
	//push(i) した直後に get(i) する，という状況もあるので，それに対応
	t push_then_get(t v){
		t res=get();
		push(v);
		return res+v;
	}
};

template<class t>
struct online_subset_mebius{
	int n,cur;
	vc<t> x;
	online_subset_mebius(int nn):n(nn),cur(0),x(1<<n){
		assert(n<=nmax);
	}
	//\sum_{bit \in cur} x[bit]*(-1)^|cur-bit|
	t get(){
		t res{};
		rep(i,n)if(cur&1<<i)res+=x[cur^1<<i];
		return res*parity(pcnt[cur]);
	}
	void push(t v){
		x[cur]=v*parity(pcnt[cur]);
		int k=botbit(++cur);
		rep(lv,k){
			int w=1<<lv;
			rng(i,cur-w*2,cur-w)x[i+w]+=x[i];
		}
	}
};

//mode=0 普通に array を subet_sum してやる
//mode=1 popcount(bit) 以下しか意味がない
//mode=2 popcount(bit) 以上しか意味がない
template<class t,int mode>
struct online_subset_sum_array{
	using A=array<t,nmax>;
	int n,cur;
	vc<A> x;
	online_subset_sum_array(int nn):n(nn),cur(0),x(1<<n){
		assert(n<=nmax);
		if constexpr(mode==1||mode==2){
			assert(n<=nmax);
		}
	}
	A get(){
		A res{};
		int k=pcnt[cur]-1;
		if constexpr(mode==0){
			rep(i,n)if(cur&1<<i)rep(j,nmax)res[j]+=x[cur^1<<i][j];
		}else if constexpr(mode==1){
			rep(i,n)if(cur&1<<i)rep(j,k+1)res[j]+=x[cur^1<<i][j];
		}else if constexpr(mode==2){
			rep(i,n)if(cur&1<<i)rng(j,k,nmax)res[j]+=x[cur^1<<i][j];
		}else static_assert([]{return false;}());
		return res;
	}
	void push(const A&v){
		x[cur]=v;
		int k=botbit(++cur);
		rep(lv,k){
			int w=1<<lv;
			if constexpr(mode==0){
				rng(i,cur-w*2,cur-w)rep(j,nmax)x[i+w][j]+=x[i][j];
			}else if constexpr(mode==1){
				rng(i,cur-w*2,cur-w)rep(j,pcnt[i]+1)x[i+w][j]+=x[i][j];
			}else if constexpr(mode==2){
				rng(i,cur-w*2,cur-w)rng(j,pcnt[i],nmax)x[i+w][j]+=x[i][j];
			}else static_assert([]{return false;}());
		}
	}
};

//定数倍高速化により，N=20 でも 1sec 程度(手元)で動いている
template<class t>
struct online_subset_convolution{
	using A=array<t,nmax-1+1>;
	int n,cur;
	online_subset_sum_array<t,1> a,b;
	online_subset_sum_array<t,2> c;
	A asum,bsum,csum;
	vc<mint> xs,ys;
	online_subset_convolution(int nn):n(nn),cur(0),a(n),b(n),c(n),xs(1<<n),ys(1<<n){
		assert(n<=nmax);
		assert(n<=nmax);
	}
	//x[cur],y[cur] を使わないものの寄与だけ得る
	t get(){
		assert(cur<(1<<n));
		int k=pcnt[cur];
		asum=a.get();
		bsum=b.get();
		csum=c.get();
		if(k<=n-1){
			t res=csum[k]*parity(k);
			rep(i,k+1)res+=asum[i]*bsum[k-i];
			return res;
		}else{
			t res{};
			rng(p,1,cur)res+=xs[p]*ys[cur-p];
			return res;
		}
	}
	void push(t x,t y){
		xs[cur]=x;
		ys[cur]=y;
		int k=pcnt[cur];
		A tmp{};
		if(k<=n-1){
			asum[k]+=x;
			bsum[k]+=y;
			tmp[k]=x; a.push(tmp);
			tmp[k]=y; b.push(tmp);
			tmp[k]=0;
			//rep(i,k+1)rng(j,k-i,min(n-1-i,k)+1)tmp[i+j]+=asum[i]*bsum[j];
			{//static modint 専用高速化
				#ifdef DYNAMIC_MOD
				static_assert(false);
				#endif
				static_assert(t::mod<(1<<30));
				const int D=16;
				ull buf[nmax]{};
				if(k+1<=D){
					rep(i,k+1)rng(j,k-i,min(n-1-i,k)+1)buf[i+j]+=(ull)asum[i].v*bsum[j].v;
				}else{
					rep(i,D)rng(j,k-i,min(n-1-i,k)+1)buf[i+j]+=(ull)asum[i].v*bsum[j].v;
					rng(ij,k,min(k+k,n-1)+1)buf[ij]%=t::mod;
					rng(i,D,k+1)rng(j,k-i,min(n-1-i,k)+1)buf[i+j]+=(ull)asum[i].v*bsum[j].v;
				}
				rng(ij,k,min(k+k,n-1)+1)tmp[ij].v=buf[ij]%t::mod;
			}
			c.push(tmp*parity(k));
		}else{
			a.push(tmp);
			b.push(tmp);
			c.push(tmp);
		}
		cur++;
	}
};

//定数倍高速化により，N=20 でも 1sec 程度で動いている
template<class t>
struct online_subset_convolution_fixedy{
	using A=array<t,nmax-1+1>;
	int n,cur;
	online_subset_sum_array<t,1> a;
	vc<A> b;
	online_subset_sum_array<t,2> c;
	A asum,bsum,csum;
	vc<mint> xs;
	const vc<mint> ys;
	online_subset_convolution_fixedy(int nn,vc<mint> ysraw):n(nn),cur(0),a(n),b(1<<n),c(n),xs(1<<n),ys(ysraw){
		assert(n<=nmax);
		assert(n<=nmax);
		assert(si(ys)==(1<<n));
		rep(i,1<<n)b[i][pcnt[i]]=ys[i];
		rep(lv,n)rep(i,1<<n)if((i&1<<lv)==0)
			rep(j,pcnt[i]+1)b[i|1<<lv][j]+=b[i][j];
	}
	//x[cur] を使わないものの寄与だけ得る
	t get(){
		assert(cur<(1<<n));
		int k=pcnt[cur];
		asum=a.get();
		bsum=b[cur];
		csum=c.get();
		if(k<=n-1){
			t res=csum[k]*parity(k);
			rep(i,k+1)res+=asum[i]*bsum[k-i];
			return res;
		}else{
			t res{};
			rep(p,cur)res+=xs[p]*ys[cur-p];
			return res;
		}
	}
	void push(t x){
		xs[cur]=x;
		int k=pcnt[cur];
		A tmp{};
		if(k<=n-1){
			asum[k]+=x;
			tmp[k]=x; a.push(tmp);
			tmp[k]=0;
			//rep(i,k+1)rng(j,k-i,min(n-1-i,k)+1)tmp[i+j]+=asum[i]*bsum[j];
			{//static modint 専用高速化
				#ifdef DYNAMIC_MOD
				static_assert(false);
				#endif
				static_assert(t::mod<(1<<30));
				const int D=16;
				ull buf[nmax]{};
				if(k+1<=D){
					rep(i,k+1)rng(j,k-i,min(n-1-i,k)+1)buf[i+j]+=(ull)asum[i].v*bsum[j].v;
				}else{
					rep(i,D)rng(j,k-i,min(n-1-i,k)+1)buf[i+j]+=(ull)asum[i].v*bsum[j].v;
					rng(ij,k,min(k+k,n-1)+1)buf[ij]%=t::mod;
					rng(i,D,k+1)rng(j,k-i,min(n-1-i,k)+1)buf[i+j]+=(ull)asum[i].v*bsum[j].v;
				}
				rng(ij,k,min(k+k,n-1)+1)tmp[ij].v=buf[ij]%t::mod;
			}
			c.push(tmp*parity(k));
		}else{
			a.push(tmp);
			c.push(tmp);
		}
		cur++;
	}
	t push_then_get(t v){
		t res=get()+ys[0]*v;
		push(v);
		return res;
	}
};

}
