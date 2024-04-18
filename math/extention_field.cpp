//yukicoder No.950
mint phi;

struct F{
	mint a,b;
	F(mint aa=0,mint bb=0):a(aa),b(bb){}
	F operator-()const{
		return F(-a,-b);
	}
	F& operator+=(const F&r){
		a+=r.a;
		b+=r.b;
		return *this;
	}
	F operator+(const F&r)const{
		return F(*this)+=r;
	}
	F& operator-=(const F&r){
		a-=r.a;
		b-=r.b;
		return *this;
	}
	F operator-(const F&r)const{
		return F(*this)-=r;
	}
	F operator*(const F&r)const{
		return F(a*r.a+b*r.b*phi,a*r.b+b*r.a);
	}
	F& operator*=(const F&r){
		return *this=*this*r;
	}
	F& operator/=(const F&r){
		return *this*=r.inv();
	}
	F operator/(const F&r)const{
		return F(*this)/=r;
	}
	F pow(int n)const{
		F res{1,0},x=*this;
		while(n){
			if(n&1)res=res*x;
			x=x*x;
			n>>=1;
		}
		return res;
	}
	F inv()const{
		return pow(ll(mod)*mod-2);
	}
	bool operator<(const F&r)const{
		if(a!=r.a)return a<r.a;
		else return b<r.b;
	}
	bool operator==(const F&r)const{
		return a==r.a&&b==r.b;
	}
	bool operator!=(const F&r)const{
		return a!=r.a||b!=r.b;
	}
	bool is_zero()const{
		return a.v==0&&b.v==0;
	}
	explicit operator bool()const{
		return a||b;
	}
};

ostream&operator<<(ostream&os,const F&f){
	return os<<"{"<<f.a<<","<<f.b<<"}";
}
