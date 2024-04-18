//yukicoder No.950
template<class t>
t det22(const Matrix<t>&a){
	return a[0][0]*a[1][1]-a[0][1]*a[1][0];
}

template<class t>
Matrix<t> inv22(const Matrix<t>&a){
	Matrix<t> res(2);
	res[0][0]=a[1][1];
	res[1][0]=-a[1][0];
	res[0][1]=-a[0][1];
	res[1][1]=a[0][0];
	dmp(res*a);
	t d=det22(a);
	assert(d);
	return res/d;
}
