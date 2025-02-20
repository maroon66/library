
struct xoruint{
	uint v;
	xoruint(uint vv=0):v(vv){}
	xoruint& operator+=(const xoruint&rhs){v^=rhs.v;return *this;}
	xoruint operator+(const xoruint&rhs)const{return xoruint(*this)+=rhs;}
	xoruint& operator-=(const xoruint&rhs){v^=rhs.v;return *this;}
	xoruint operator-(const xoruint&rhs)const{return xoruint(*this)-=rhs;}
};
istream& operator>>(istream&is,xoruint&x){
	return is>>x.v;
}
ostream& operator<<(ostream&os,const xoruint&x){
	return os<<x.v;
}
