
//not verified
//コンパイル通しただけ
namespace std{
__float128 abs(__float128 x){
	return x<0?-x:x;
}
__float128 sin(__float128 x){
	return sinl(x);
}
__float128 cos(__float128 x){
	return cosl(x);
}
__float128 acos(__float128 x){
	return acosl(x);
}
__float128 atan2(__float128 y,__float128 x){
	return atan2l(y,x);
}
__float128 sqrt(__float128 x){
	return sqrtl(x);
}
__float128 fmod(__float128 x,__float128 y){
	return fmodl(x,y);
}
ostream&operator<<(ostream&os,__float128 v){
	return os<<(long double)v;
}
istream&operator>>(istream&is,__float128&v){
	long double z;
	is>>z;
	v=z;
	return is;
}
}
