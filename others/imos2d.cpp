
struct imos2d{
	int h,w;
	vvc<int> a;
	imos2d(const vvc<int>&rw){
		h=si(rw);
		w=si(rw[0]);
		a.resize(h+1,vi(w+1));
		rng(i,1,h+1)rng(j,1,w+1){
			a[i][j]=a[i-1][j]+a[i][j-1]-a[i-1][j-1]+rw[i-1][j-1];
		}
	}
	int get(int i1,int i2,int j1,int j2){
		assert(0<=i1&&i1<=i2&&i2<=h);
		assert(0<=j1&&j1<=j2&&j2<=w);
		return a[i2][j2]-a[i2][j1]-a[i1][j2]+a[i1][j1];
	}
};
