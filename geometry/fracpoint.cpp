template<class D>
struct FracPoint{
	D x,y,den;
	FracPoint(D xx=0,D yy=0,D dd=1):x(xx),y(yy),den(dd){}
	FracPoint operator-(const FracPoint&rhs)const{
		return FracPoint(x*rhs.den-rhs.x*den,y*rhs.den-rhs.y*den,den*rhs.den);
	}
	FracPoint operator-()const{
		return FracPoint(-x,-y,den);
	}
};

template<class D>
bool isless00(FracPoint<D> a){
	if(a.x)return a.x<0;
	return a.y<0;
}

template<class D>
int ccw(FracPoint<D> a,FracPoint<D> b){
	D s=a.x*b.y-a.y*b.x;
	if(s<0)return -1;
	else if(s>0)return 1;
	else return 0;
}

template<class D>
bool argless(FracPoint<D> a,FracPoint<D> b){
	return ccw(a,b)>0;
}
