template<class num,int S>
struct Vector{
	array<num,S> v;
	Vector(){
		rep(i,size())v[i]=0;
	}
	int size()const{
		return S;
	}
	num& operator[](int i){
		return v[i];
	}
	num const& operator[](int i)const{
		return v[i];
	}
	Vector& operator+=(const Vector&rhs){
		assert(size()==rhs.size());
		rep(i,size())
			v[i]+=rhs[i];
		return *this;
	}
	Vector& operator-=(const Vector&rhs){
		assert(size()==rhs.size());
		rep(i,size())
			v[i]-=rhs[i];
		return *this;
	}
	Vector& operator*=(const num&x){
		rep(i,size())
			v[i]*=x;
		return *this;
	}
	Vector& operator/=(const num&x){
		num y=num(1)/x;
		rep(i,size())
			v[i]*=y;
		return *this;
	}
	Vector operator+(const Vector&rhs)const{
		return Vector(*this)+=rhs;
	}
	Vector operator-(const Vector&rhs)const{
		return Vector(*this)-=rhs;
	}
	Vector operator*(const num&x)const{
		return Vector(*this)*=x;
	}
	Vector operator/(const num&x)const{
		return Vector(*this)/=x;
	}
	bool operator==(const Vector&rhs)const{
		return v==rhs.v;
	}
};

template<class num,int S>
num dot(const Vector<num,S>&a,const Vector<num,S>&b){
	assert(a.size()==b.size());
	const int s=a.size();
	num ans(0);
	rep(i,s)
		ans+=a[i]*b[i];
	return ans;
}

template<class num,int S>
ostream&operator<<(ostream&os,const Vector<num,S>&v){
	return os<<v.v;
}

template<class num,int S>
struct Matrix{
	using V=Vector<num,S>;
	array<V,S> m;
	Matrix(num z=num(0)){
		rep(i,size())rep(j,size()){
			if(i==j)m[i][j]=z;
			else m[i][j]=0;
		}
	}
	int size()const{
		return S;
	}
	Matrix operator*(const Matrix&rhs)const{
		assert(size()==rhs.size());
		Matrix res;
		rep(i,size())rep(j,size())rep(k,size())
			res[i][j]+=m[i][k]*rhs[k][j];
		return res;
	}
	Matrix& operator*=(const Matrix&rhs){
		return *this=(*this)*rhs;
	}
	V operator*(const V&rhs)const{
		assert(size()==rhs.size());
		V res;
		rep(i,size())
			res[i]=dot(m[i],rhs);
		return res;
	}
	V& operator[](int i){
		return m[i];
	}
	V const& operator[](int i) const{
		return m[i];
	}
	Matrix& operator/=(const num&r){
		rep(i,m.size())m[i]/=r;
		return *this;
	}
	Matrix operator/(const num&r)const{
		return Matrix(*this)/=r;
	}
	bool operator==(const Matrix&rhs)const{
		return m==rhs.m;
	}
};

template<class num,int S>
ostream&operator<<(ostream&os,const Matrix<num,S>&x){
	const int s=x.size();
	os<<"----------"<<endl;
	rep(i,s){
		rep(j,s){
			os<<x[i][j];
			if(j==s-1){
				os<<endl;
			}else{
				os<<" ";
			}
		}
	}
	return os<<"----------";
}

template<class num,int S>
num det(Matrix<num,S> a){
	const int s=a.size();
	num ans(1);
	rep(i,s){
		rng(j,i+1,s)if(a[j][i]){
			ans=-ans;
			swap(a[j],a[i]);
			break;
		}
		if(!a[i][i])return 0;
		ans*=a[i][i];
		rng(j,i+1,s){
			mint w=-a[j][i]/a[i][i];
			rng(k,i,s)
				a[j][k]+=a[i][k]*w;
		}
	}
	return ans;
}

template<class num,int S>
Matrix<num,S> matpow(Matrix<num,S> a,int n){
	Matrix<num,S> res(1);
	while(n){
		if(n&1)res*=a;
		a*=a;
		n>>=1;
	}
	return res;
}

template<class num,int S>
Vector<num,S> matpow_V(Matrix<num,S> a,int n,Vector<num,S> res){
	while(n){
		if(n&1)res=a*res;
		a*=a;
		n>>=1;
	}
	return res;
}

//0,1,2,3,4,5
//1,1,2,3,5,8
pair<mint,mint> getfib(int n){
	using V=Vector<mint,2>;
	using M=Matrix<mint,2>;
	M m;
	m[0][1]=m[1][0]=m[1][1]=1;
	V v;
	v[0]=v[1]=1;
	V res=matpow_V(m,n,v);
	return mp(res[0],res[1]);
}

//using V=Vector<mint,2>;
//using M=Matrix<mint,2>;
