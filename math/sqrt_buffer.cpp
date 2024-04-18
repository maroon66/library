
	int n2=sqrtl(n);
	vi vs;
	{
		int v=n;
		while(v){
			vs.pb(v);
			v=n/(n/v+1);
		}
	}
	int s=vs.size();
	auto getidx=[&](int a){
		return a<=n2?s-a:n/a-1;
	};



//CF857F
//切り上げ
struct N{
	vi vs;
	int n,n2,s;
	N(int n_):n(n_),n2(sqrtl(n)){
		{
			int v=n;
			while(1){
				vs.pb(v);
				if(v==1)break;
				int w=(n+v-2)/(v-1);
				v=(n+w-1)/w;
			}
		}
		s=vs.size();
	}
	int getidx(int a){
		return a<=n2?s-a:(n+a-1)/a-1;
	}
};
