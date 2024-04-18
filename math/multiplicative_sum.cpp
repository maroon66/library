//SRM552H
//f が 0,1 のパターンしか verify できてねえ，大丈夫か？
//initprime してから使う！
//sqrt(n) までのテーブルがあればいい
//black algorithm http://baihacker.github.io/main/2020/The_prefix-sum_of_multiplicative_function_the_black_algorithm.html
//f(n) は乗法的関数
//sum 1<=i<=n f(i) を求める
//h(n) = sum p<=n, p is prime f(p)
struct multiplicative_sum{
	//f と h いじれば一応もう使える
	
	//f(v)=f(p^c)
	//p is prime
	int f(int v,int p,int c){
		if(p>mx)return 0;
		return c%2;
	}
	int h(int a){
		chmin(a,mx);
		return lwb(ps,a+1);
	}
	
	vi vs,buf;
	int n,n2,s,ans,nxp,off;
	multiplicative_sum(int nn):n(nn){
		n2=floor(sqrtl(n));
		while(sq(n2+1)<=n)n2++;
		
		{
			int v=n;
			while(v){
				vs.pb(v);
				v=n/(n/v+1);
			}
		}
		s=vs.size();
		
		//ここ高速化の余地あり
		buf.resize(s);
		rep(i,s)buf[i]=h(vs[i]);
		
		ans=1+buf[0];
		
		rep(i,si(ps))if(sq(ps[i])<=n)dfs(i,1,ps[i],n/ps[i],1);else break;
	}
	int getidx(int a){
		return a<=n2?s-a:n/a-1;
	}
	void dfs(int i,int c,int v,int lim,int cur){
		ans+=cur*f(v*ps[i],ps[i],c+1);
		if(lim>=sq(ps[i]))dfs(i,c+1,v*ps[i],lim/ps[i],cur);
		cur*=f(v,ps[i],c);
		ans+=cur*(buf[getidx(lim)]-buf[getidx(ps[i])]);
		for(int j=i+1;sq(ps[j])<=lim;j++)
			dfs(j,1,ps[j],lim/ps[j],cur);
	}
};
