const int s=50;

//CF459C
struct Vec{
	int val[s];
	Vec(){
		rep(i,s)val[i]=inf;
	}
};

//CF459C
struct Matrix{
	int val[s][s];
	Matrix(){
		rep(i,s)rep(j,s)val[i][j]=inf;
	}
	Matrix operator*(const Matrix&r)const{
		Matrix res;
		rep(i,s)rep(j,s)rep(k,s)
			chmin(res.val[i][j],val[i][k]+r.val[k][j]);
		return res;
	}
	Vec operator*(const Vec&r)const{
		Vec res;
		rep(i,s)rep(j,s)chmin(res.val[i],val[i][j]+r.val[j]);
		return res;
	}
};

Matrix matpow(Matrix a,int n){
	Matrix res;
	rep(i,s)res.val[i][i]=0;
	while(n){
		if(n&1)res=res*a;
		a=a*a;
		n>>=1;
	}
	return res;
}
