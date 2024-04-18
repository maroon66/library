//Bytecamp 2022 Day2 L
template<class num,int S>
struct Vector:array<num,S>{
	Vector operator-()const{
		Vector v;
		rep(i,S)v[i]=-(*this)[i];
		return v;
	}
	Vector& operator+=(const Vector&rhs){
		rep(i,S)(*this)[i]+=rhs[i];
		return *this;
	}
	Vector& operator-=(const Vector&rhs){
		rep(i,S)(*this)[i]-=rhs[i];
		return *this;
	}
	Vector operator+(const Vector&rhs)const{
		return Vector(*this)+=rhs;
	}
	Vector operator-(const Vector&rhs)const{
		return Vector(*this)-=rhs;
	}
	Vector& operator*=(const num&x){
		rep(i,S)(*this)[i]*=x;
		return *this;
	}
	Vector& operator/=(const num&x){
		num y=num(1)/x;
		rep(i,S)(*this)[i]*=y;
		return *this;
	}
	/*Vector& operator*=(const Vector&rhs){
		rep(i,S)(*this)[i]*=rhs[i];
		return *this;
	}
	Vector& operator/=(const Vector&rhs){
		rep(i,S)(*this)[i]/=rhs[i];
		return *this;
	}
	Vector operator*(const Vector&rhs)const{
		return Vector(*this)*=rhs;
	}
	Vector operator/(const Vector&rhs)const{
		return Vector(*this)/=rhs;
	}*/
	Vector operator*(const num&x)const{
		return Vector(*this)*=x;
	}
	Vector operator/(const num&x)const{
		return Vector(*this)/=x;
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

//CF CodeTon4 H
template<class num,int S>
struct Matrix:Vector<Vector<num,S>,S>{
	using V=Vector<num,S>;
	Matrix operator*(const Matrix&rhs)const{
		Matrix res{};
		rep(i,S)rep(j,S)rep(k,S)
			res[i][j]+=(*this)[i][k]*rhs[k][j];
		return res;
	}
	Matrix& operator*=(const Matrix&rhs){
		return *this=(*this)*rhs;
	}
	V operator*(const V&rhs)const{
		V res{};
		rep(i,S)
			res[i]=dot((*this)[i],rhs);
		return res;
	}
	static Matrix getid(){
		Matrix res{};
		rep(i,S)res[i][i]=1;
		return res;
	}
};

template<int S>
Matrix<mint,S> matpow(Matrix<mint,S> a,int n){
	Matrix<mint,S> res=Matrix<mint,S>::getid();
	while(n){
		if(n&1)res*=a;
		a*=a;
		n>>=1;
	}
	return res;
}

template<int S>
Vector<mint,S> matpow_V(Matrix<mint,S> a,int n,Vector<mint,S> res){
	while(n){
		if(n&1)res=a*res;
		a*=a;
		n>>=1;
	}
	return res;
}

const int S=2;
using V=Vector<mint,S>;
using M=Matrix<mint,S>;

M matpow(M a,int n){
	M res=M::getid();
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
