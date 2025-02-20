
struct xorshift{
public:
	xorshift(){
		Reset(1);
	}
	void Reset(unsigned aa){
		a =   aa;
		b = 1234567890;
		c =  890123456;
		d = 1098765432;
		rep(i,100)
			operator()();
	}
	unsigned operator()(){
		unsigned w = a ^ (a << 11);
		a=b;b=c;c=d;
		d=(d^(d>>19))^(w^(w>>8));
		return d;
	}
private:
	unsigned a,b,c,d;
} xrand;

int rnd(int k){
	return xrand()%k;
}

int rnd(int b,int e){
	return b+rnd(e-b+1);
}

double rnd_d(){
	return double(xrand())/UINT_MAX;
}

struct N{
	chrono::system_clock::time_point startTime;
	int Elapsed(){
		auto cur=chrono::system_clock::now();
		auto msecs=chrono::duration_cast<chrono::milliseconds>(cur-startTime);
		return msecs.count();
	}
	int n,m;
	vvc<int> a,c,w;
	vi u,x;
	int score,globalmin;
	//最大の変化量/最小の変化量がいいらしい
	const int start_temp=ten(13),end_temp=1;
	void init(){
		cin>>n>>m;
		a.resize(n,vi(m));
		c.resize(n,vi(m));
		w.resize(n,vi(n));
		rep(i,n)rep(j,m)cin>>a[i][j]>>c[i][j];
		rep(i,n)rng(j,i+1,n)cin>>w[i][j];
		u.resize(n);
		score=0;
		rep(i,n)score+=c[i][0];
		rep(i,n)rng(j,i+1,n)score+=abs(a[i][u[i]]-a[j][u[j]])*w[i][j];
		globalmin=score;
	}
	int getcost(int i,int j){
		int res=c[i][j];
		rep(k,i)res+=abs(a[k][u[k]]-a[i][j])*w[k][i];
		rng(k,i+1,n)res+=abs(a[k][u[k]]-a[i][j])*w[i][k];
		return res;
	}
	void trymodify(const double temp){
		int i=rnd(n);
		int j=rnd(m);
		int dif=getcost(i,j)-getcost(i,u[i]);
		//minimize
		if(dif<0||rnd_d()<exp(-dif/temp)){
			score+=dif;
			u[i]=j;
		}else{
			//do nothing
		}
	}
	//T in msecs
	void annealing(int T){
		startTime=chrono::system_clock::now();
		T-=50;//reseve 50ms
		while(1){
			int t=Elapsed();
			if(t>T)break;
			double temp=(start_temp*(T-t)+end_temp*t)/T;
			trymodify(temp);
			chmin(globalmin,score);
		}
	}
};


	/*chrono::system_clock::time_point startTime;
	auto Elapsed=[&]()->int{
		auto cur=chrono::system_clock::now();
		auto msecs=chrono::duration_cast<chrono::milliseconds>(cur-startTime);
		return msecs.count();
	};*/
	int globalmin=score;
	auto trymodify=[&](const double temp){
		int i=rnd(n);
		int dif=cost[i];
		//minimize
		if(dif<0||rnd_d()<exp(-dif/temp)){
			flip(i);
		}else{
			//do nothing
		}
	};
	//T in msecs
	/*auto annealing=[&](int T){
		startTime=chrono::system_clock::now();
		T-=50;//reseve 50ms
		while(1){
			int t=Elapsed();
			if(t>T)break;
			double temp=(start_temp*(T-t)+end_temp*t)/T;
			trymodify(temp);
			chmin(globalmin,score);
		}
	};
	annealing(100);*/
	
	double start_temp=3;
	double end_temp=0.1;
	auto annealing=[&](int W){
		rep(t,W){
			double temp=(start_temp*(W-t)+end_temp*t)/W;
			trymodify(temp);
			chmin(globalmin,score);
		}
	};
	annealing(n*100);
