//initprime (size sqrt(N))
//手元でテストしたら 10^18 まで大丈夫そうだった

ll isqrt(ll n){
	return sqrtl(n);
}
ll icbrt(ll n){
	return cbrtl(n);
}

//VERIFY:yosupo
//pi(n/i) 列挙
//sum[i] = pi(vs[i])
//O(N^(2/3))
//N=10^11, 297ms on yosupo judge 
//D=int Xmas2019 D
template<class D=int>
struct counting_primes{
	const int n,n2,n3,n6;
	int s;
	vi vs,sum,bit;
	int pre;
	void trans(int i,int p){
		int j=getidx(vs[i]/p);
		sum[i]-=sum[j]-pre;
	}
	void trans2(int i,int p){
		int j=getidx(vs[i]/p);
		int z=sum[j];
		if(j>=n3){
			j-=n3;
			for(;j<si(bit);j+=(j+1)&(-j-1))
				z+=bit[j];
		}
		sum[i]-=z-pre;
	}
	void upd(int i,int cur,bool f){
		if(!f){
			for(int x=getidx(cur)-n3;x>=0;x-=(x+1)&(-x-1))
				bit[x]--;
		}
		for(int j=i;cur*ps[j]<=vs[n3];j++)
			upd(j,cur*ps[j],false);
	}
	int getidx(int a){
		return a<=n2?s-a:n/a-1;
	}
	counting_primes(int nn):n(nn),n2(isqrt(n)),n3(icbrt(n)),n6(icbrt(n2)){
		{
			int v=n;
			while(v){
				vs.pb(v);
				v=n/(n/v+1);
			}
		}
		s=vs.size();
		
		sum.resize(s);
		rep(i,s)sum[i]=vs[i]-1;
		
		int idx=0;
		pre=0;
		{
			while(ps[idx]<=n6){
				rep(i,s){
					if(sq(ps[idx])>vs[i])break;
					trans(i,ps[idx]);
				}
				pre++;
				idx++;
			}
		}
		{
			bit.resize(s-n3);
			while(ps[idx]<=n3){
				rep(i,n3){
					if(sq(ps[idx])>vs[i])break;
					trans2(i,ps[idx]);
				}
				assert(ps[idx]<=vs[n3]);
				upd(idx,ps[idx],true);
				pre++;
				idx++;
			}
			per(i,s-n3){
				int j=i+((i+1)&(-i-1));
				if(j<si(bit))
					bit[i]+=bit[j];
			}
			rep(i,s-n3){
				sum[i+n3]+=bit[i];
			}
		}
		{
			while(ps[idx]<=n2){
				rep(i,s){
					if(sq(ps[idx])>vs[i])break;
					trans(i,ps[idx]);
				}
				pre++;
				idx++;
			}
		}
	}
	
	//f(v)=f(p^c)
	//p is prime
	//black algorithm http://baihacker.github.io/main/2020/The_prefix-sum_of_multiplicative_function_the_black_algorithm.html
	//f(n) は乗法的関数
	//sum 1<=i<=n f(i) を求める
	//h(n) = sum p<=n, p is prime f(p)
	//f(p) が p の多項式だとして，sum を用い buf[i]=h(vs[i]) を初期化する
	D f(int v,int p,int c){
		return c&1?-1:1;
	}
	vc<D> buf;
	D ans;
	D multiplicative_sum(){
		buf.resize(s);
		rep(i,s){
			buf[i]=-sum[i];
		}
		
		//f(1),f(p) の分
		ans=1+buf[0];
		
		//f(n) (n は重複込みで 2 個以上の素因数を持つ) の分
		rep(i,si(ps))if(sq(ps[i])<=n)dfs(i,1,ps[i],n/ps[i],1);else break;
		
		return ans;
	}
	//v=p^c まで確定
	//更に p 以上の素因数を 1 つ以上追加する
	//追加分の積は lim 以下
	void dfs(int i,int c,int v,int lim,D cur){
		//p^(c+1) で終了
		ans+=cur*f(v*ps[i],ps[i],c+1);
		//p^(c+1) まで確定させて，次へ行く
		if(lim>=sq(ps[i]))dfs(i,c+1,v*ps[i],lim/ps[i],cur);
		//p^c で確定，次へ行く
		cur*=f(v,ps[i],c);
		//残りの素因数 1 個
		ans+=cur*(buf[getidx(lim)]-buf[getidx(ps[i])]);
		//残りの素因数 2 個以上
		for(int j=i+1;sq(ps[j])<=lim;j++)
			dfs(j,1,ps[j],lim/ps[j],cur);
	}
};
