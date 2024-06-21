//ARC055D
//比較関数のあたりはverifyされてない?多分大丈夫だと思うけど
//多分verifyした
//ABC301G (約分なし)
template<class t>
struct frac{
	t a,b;
	frac():a(0),b(1){}
	template<class u>
	frac(u aa):a(aa),b(1){}
	template<class u,class v>
	frac(u aa,v bb):a(aa),b(bb){g();}
	frac&g(){
		if(a==0){
			b=1;
			return *this;
		}
		t x=gcd(a,b);
		a/=x;b/=x;
		if(b<0){a=-a;b=-b;}
		return *this;
	}
	t quo()const{return a/b-(((a<0&&b>0)||(a>0&&b<0))&&(a%b!=0));}
	t rem()const{return a-quo()*b;}
	frac inv()const{return frac(b,a);}
	frac operator-()const{return frac(-a,b);}
	frac&operator+=(frac r){a=a*r.b+r.a*b;b*=r.b;return g();}
	frac&operator-=(frac r){a=a*r.b-r.a*b;b*=r.b;return g();}
	frac&operator*=(frac r){a*=r.a;b*=r.b;return g();}
	frac&operator/=(frac r){a*=r.b;b*=r.a;return g();}
	frac operator+(frac r)const{return frac(*this)+=r;}
	frac operator-(frac r)const{return frac(*this)-=r;}
	frac operator*(frac r)const{return frac(*this)*=r;}
	frac operator/(frac r)const{return frac(*this)/=r;}
	bool operator<(frac r)const{
		t c=quo(),d=r.quo();
		if(c!=d)return c<d;
		else return rem()*r.b<r.rem()*b;
	}
	bool operator<=(frac r)const{
		t c=quo(),d=r.quo();
		if(c!=d)return c<=d;
		else return rem()*r.b<=r.rem()*b;
	}
	bool operator>(frac r)const{
		t c=quo(),d=r.quo();
		if(c!=d)return c>d;
		else return rem()*r.b>r.rem()*b;
	}
	bool operator>=(frac r)const{
		t c=quo(),d=r.quo();
		if(c!=d)return c>=d;
		else return rem()*r.b>=r.rem()*b;
	}
	bool operator==(frac r)const{
		return quo()==r.quo()&&rem()*r.b==r.rem()*b;
	}
	bool operator!=(frac r)const{
		return !operator==(r);
	}
	frac pow(int n)const{
		frac res(1);
		rep(_,n)res*=*this;
		return res;
	}
};
template<class t>
ostream&operator<<(ostream&os,frac<t> a){
	return os<<a.a<<"/"<<a.b;
}
template<class t>
istream&operator>>(istream&is,frac<t>&a){
	a.b=1;
	return is>>a.a;
}
template<class t>
int sgn(frac<t> a){return a.a<0?-1:(a.a>0?1:0);}
template<class t>
int sgn(frac<t> a,frac<t> b){
	t p=a.a*b.b-b.a*a.b;
	return p<0?-1:(p==0?0:1);
}
template<class t>
frac<t> abs(frac<t> a){return a.a<0?-a:a;}

using mint=frac<int128>;
const int vmax=20;
mint fact[vmax],finv[vmax],invs[vmax];
void initfact(){
	fact[0]=1;
	finv[0]=1;
	rng(i,1,vmax){
		fact[i]=fact[i-1]*i;
		finv[i]=finv[i-1]/i;
	}
	rng(i,1,vmax)invs[i]=mint(1,i);
}

//GCJ2019R2C
//assume a<b
//find a < p/q < b with minimum p,q
template<class t>
frac<t> between(frac<t> a,frac<t> b){
	int c=a.quo();
	if(b>c+1)return c+1;
	a-=c;
	b-=c;
	if(a.a==0)return frac<t>(1,b.b/b.a+1)+c;
	auto d=between(b.inv(),a.inv());
	return d.inv()+c;
}

//HDU 6624
//assume a<b
//find a < p < b with minimum p,q
pi between(pi a,pi b){
	int c=a.a/a.b;
	if(b.a>b.b*(c+1))return pi(c+1,1);
	a.a-=a.b*c;
	b.a-=b.b*c;
	int p,q;
	if(a.a==0){
		p=1;
		q=b.b/b.a+1;
	}else{
		tie(q,p)=between(pi(b.b,b.a),pi(a.b,a.a));
	}
	return pi(p+q*c,q);
}
