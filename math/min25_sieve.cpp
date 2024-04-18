//initprime
//kth_root_integer
//sump

//LOJ6053
//sqrt(n) までの素数のテーブル必要
//各 i について，(n/i) 以下の素数の 0,1,...,K-1 乗和を求める
//O(K N^(2/3))
//https://min-25.hatenablog.com/entry/2018/11/11/172216
template<int K>
struct min25_sieve{
	using A=array<mint,K>;
	void add(A&a,const A&b){rep(i,K)a[i]+=b[i];}
	void sub(A&a,const A&b){rep(i,K)a[i]-=b[i];}
	const int n,n2,n3,n6;
	int s;
	vi vs;
	vc<A> sum,bit;
	A pre;
	A getpows(mint p){
		A res;
		res[0]=1;
		rng(i,1,K)res[i]=res[i-1]*p;
		return res;
	}
	void trans(int i,int p){
		A w=getpows(p);
		int j=getidx(vs[i]/p);
		rep(k,K)sum[i][k]-=(sum[j][k]-pre[k])*w[k];
	}
	void trans2(int i,int p){
		A w=getpows(p);
		int j=getidx(vs[i]/p);
		A z=sum[j];
		if(j>=n3){
			j-=n3;
			for(;j<si(bit);j+=(j+1)&(-j-1))
				add(z,bit[j]);
		}
		rep(k,K)sum[i][k]-=(z[k]-pre[k])*w[k];
	}
	void upd(int i,int cur,bool f){
		if(!f){
			A w=getpows(cur);
			for(int x=getidx(cur)-n3;x>=0;x-=(x+1)&(-x-1))
				sub(bit[x],w);
		}
		for(int j=i;cur*ps[j]<=vs[n3];j++)
			upd(j,cur*ps[j],false);
	}
	min25_sieve(int nn):n(nn),n2(isqrt(n)),n3(icbrt(n)),n6(kth_root_integer(n,6)){
		{
			int v=n;
			while(v){
				vs.pb(v);
				v=n/(n/v+1);
			}
		}
		s=vs.size();
		
		sum.resize(s);
		rep(i,s)rep(j,K)sum[i][j]=sump(j,vs[i])-1;
		
		int idx=0;
		pre={};
		{
			while(ps[idx]<=n6){
				rep(i,s){
					if(sq(ps[idx])>vs[i])break;
					trans(i,ps[idx]);
				}
				add(pre,getpows(ps[idx]));
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
				add(pre,getpows(ps[idx]));
				idx++;
			}
			per(i,s-n3){
				int j=i+((i+1)&(-i-1));
				if(j<si(bit))
					add(bit[i],bit[j]);
			}
			rep(i,s-n3){
				add(sum[i+n3],bit[i]);
			}
		}
		{
			while(ps[idx]<=n2){
				rep(i,s){
					if(sq(ps[idx])>vs[i])break;
					trans(i,ps[idx]);
				}
				add(pre,getpows(ps[idx]));
				idx++;
			}
		}
	}
	int getidx(int a){
		return a<=n2?s-a:n/a-1;
	}
	
	//f(v)=f(p^c)
	//p is prime
	//black algorithm http://baihacker.github.io/main/2020/The_prefix-sum_of_multiplicative_function_the_black_algorithm.html
	//f(n) は乗法的関数
	//sum 1<=i<=n f(i) を求める
	//h(n) = sum p<=n, p is prime f(p)
	//f(p) が p の多項式だとして，sum を用い buf[i]=h(vs[i]) を初期化する
	mint f(int v,int p,int c){
		return p^c;
	}
	vc<mint> buf;
	mint ans;
	mint multiplicative_sum(){
		buf.resize(s);
		rep(i,s){
			buf[i]=sum[i][1]-sum[i][0];
			if(vs[i]>=2)buf[i]+=2;
		}
		
		//f(1),f(p) の分
		ans=1+buf[0];
		
		//f(n) (n は重複込みで 2 個以上の素因数を持つ) の分
		rep(i,si(ps))if(sq(ps[i])<=n)dfs(i,1,ps[i],n/ps[i],1);else break;
		
		return ans;
	}
	void dfs(int i,int c,int v,int lim,mint cur){
		ans+=cur*f(v*ps[i],ps[i],c+1);
		if(lim>=sq(ps[i]))dfs(i,c+1,v*ps[i],lim/ps[i],cur);
		cur*=f(v,ps[i],c);
		ans+=cur*(buf[getidx(lim)]-buf[getidx(ps[i])]);
		for(int j=i+1;sq(ps[j])<=lim;j++)
			dfs(j,1,ps[j],lim/ps[j],cur);
	}
};
