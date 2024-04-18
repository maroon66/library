template<class num>
struct Vector{
	vector<num> v;
	Vector(int s=0){
		v.resize(s,num(0));
	}
	int size()const{
		return v.size();
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

template<class num>
num dot(const Vector<num>&a,const Vector<num>&b){
	assert(a.size()==b.size());
	const int s=a.size();
	num ans(0);
	rep(i,s)
		ans+=a[i]*b[i];
	return ans;
}

template<class num>
ostream&operator<<(ostream&os,const Vector<num>&v){
	return os<<v.v;
}

template<class num>
struct Matrix{
	using V=Vector<num>;
	vector<V> m;
	Matrix(int s=0,num z=num(0)){
		m.resize(s,V(s));
		rep(i,size())
			m[i][i]=z;
	}
	int size()const{
		return m.size();
	}
	Matrix operator*(const Matrix&rhs)const{
		assert(size()==rhs.size());
		Matrix res(size());
		rep(i,size())rep(j,size())rep(k,size())
			res[i][j]+=m[i][k]*rhs[k][j];
		return res;
	}
	Matrix& operator*=(const Matrix&rhs){
		return *this=(*this)*rhs;
	}
	V operator*(const V&rhs)const{
		assert(size()==rhs.size());
		V res(size());
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

template<class num>
ostream&operator<<(ostream&os,const Matrix<num>&x){
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

template<class num>
num det(Matrix<num> a){
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

using V=Vector<mint>;
using M=Matrix<mint>;

M matpow(M a,int n){
	M res(si(a),1);
	while(n){
		if(n&1)res*=a;
		a*=a;
		n>>=1;
	}
	return res;
}

V matpow_V(M a,int n,V res){
	while(n){
		if(n&1)res=a*res;
		a*=a;
		n>>=1;
	}
	return res;
}

