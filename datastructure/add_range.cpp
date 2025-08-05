//JOISC2025 Day3 Brave
struct add_range{
	int n;
	vi dif;
	add_range(int nn):n(nn),dif(n+1){}
	void add(int l,int r,int v){
		assert(0<=l&&l<=r&&r<=n);
		dif[l]+=v;
		dif[r]-=v;
	}
	vi calc(){
		vi res(n);
		int sum=0;
		rep(i,n){
			sum+=dif[i];
			res[i]=sum;
		}
		return res;
	}
};
