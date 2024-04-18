//copied from yosupo's library
//PARTLY VERIFIED

//USACO 2022 January ptlatinum C

//#define GEOF

#ifdef GEOF
using ld=long double;
//using ld=double;
const ld PI=acos(ld(-1));
#else
using ld=ll;
#endif
const ld eps=1e-9;
int sgn(ld a){return a<-eps?-1:(a>eps?1:0);}
int sgn(ld a,ld b){return sgn(a-b);}
/*
using pt=complex<ld>;
#define x real()
#define y imag()
*/
struct pt {
    ld x,y;
    //pt(ld _x = ld(0), ld _y = ld(0)) : x(_x), y(_y) {}
    pt():x(0),y(0){}
    pt(ld xx,ld yy):x(xx),y(yy){}
    pt operator+(const pt& r) const { return {x + r.x, y + r.y}; }
    pt operator-(const pt& r) const { return {x - r.x, y - r.y}; }
    pt operator*(const pt& r) const {
        return {x * r.x - y * r.y, x * r.y + y * r.x};
    }
    pt inv()const{
		ld d=norm();
		return {x/d,-y/d};
	}
    pt operator/(const pt&r)const{
		return operator*(r.inv());
	}

    pt operator*(const ld& r) const { return {x * r, y * r}; }
    pt operator/(const ld& r) const { return {x / r, y / r}; }

    pt& operator+=(const pt& r) { return *this = *this + r; }
    pt& operator-=(const pt& r) { return *this = *this - r; }
    pt& operator*=(const pt& r) { return *this = *this * r; }
    pt& operator/=(const pt& r) { return *this = *this / r; }
    pt& operator*=(const ld& r) { return *this = *this * r; }
    pt& operator/=(const ld& r) { return *this = *this / r; }

    pt operator-() const { return {-x, -y}; }

	static int cmp(const pt&a,const pt&b){
		int v=sgn(a.x,b.x);
		return v?v:sgn(a.y,b.y);
	}
    bool operator<(const pt& r) const {
        return cmp(*this,r)<0;
    }
    bool operator<=(const pt& r) const {
        return cmp(*this,r)<=0;
    }
    bool operator>(const pt& r) const {
        return cmp(*this,r)>0;
    }
    bool operator==(const pt& r) const { return sgn((*this - r).rabs()) == 0; }
    bool operator!=(const pt& r) const { return !(*this == r); }

    ld norm() const { return x * x + y * y; }
    ld rabs() const { return max(std::abs(x), std::abs(y)); }  // robust abs
    pair<ld, ld> to_pair() const { return {x, y}; }
    #ifdef GEOF
    ld abs() const { return sqrt(norm()); }
    ld arg() const { return atan2(y, x); }
    static pt polar(ld le, ld th) { return {le * cos(th), le * sin(th)}; }
	#endif
};
istream& operator>>(istream& is, pt& p){
	return is>>p.x>>p.y;
}
ostream& operator<<(ostream& os, const pt& p) {
    return os << "pt(" << p.x << ", " << p.y << ")";
}
ld norm(const pt&a){
	return a.norm();
}
ld rabs(const pt&a){
	return a.rabs();
}
#ifdef GEOF
ld abs(const pt&a){
	return sqrt(norm(a));
}
//XXII Opencup Gpt of Ural K
pt normalized(const pt&a){
	return a/abs(a);
}
ld arg(const pt&a){return a.arg();}
//normalize to [-PI,PI)
//Contest 2: ptKU Contest 1, ptTZ Summer 2022 Day 4
ld normarg(ld a){
	ld res=fmod(a+PI,2*PI);
	if(res<0)res+=PI;
	else res-=PI;
	return res;
}
//normalize to [0,2*PI)
//Multiuni2023-10-E
ld normarg_nonnegative(ld a){
	ld res=fmod(a,2*PI);
	if(res<0)res+=2*PI;
	return res;
}
//AOJ1183
//arg between ab
//assume given lengths are valid
ld arg(ld a,ld b,ld c){
	return acos(min(max((a*a+b*b-c*c)/(2*a*b),ld(-1)),ld(1)));
}
//UCUP 2-20-D
ld heron(ld a,ld b,ld c){
	ld s=(a+b+c)/2;
	return sqrt(s*(s-a)*(s-b)*(s-c));
}
#endif
ld norm(const pt&a,const pt&b){
	return (a-b).norm();
}
ld dot(const pt&a,const pt&b){return a.x*b.x+a.y*b.y;}
ld crs(const pt& a,const pt& b){return a.x*b.y-a.y*b.x;}
ld crs(const pt& a,const pt& b,const pt& c){return crs(b-a,c-a);}
int ccw(const pt& a,const pt& b){return sgn(crs(a,b));}
int ccw(const pt& a,const pt& b,const pt& c){return ccw(b-a,c-a);}
//(-pi,0](0,pi]
int argtype(const pt&a){
	if(sgn(a.y)==0)return a.x<0?1:0;
	return a.y<0?0:1;
}
int argcmp(const pt&a,const pt&b){
	int at=argtype(a),bt=argtype(b);
	if(at!=bt)return at<bt?-1:1;
	return -ccw(a,b);
};
bool argless(const pt&a,const pt&b){return argcmp(a,b)<0;}
//c の位置を聞く関数です，b じゃないです
//(-2)[a,-1](0)[b,1](2)
int bet(pt a,pt b,pt c){
	pt d=b-a;
	ld e=dot(d,c-a);
	if(sgn(e)<=0)return sgn(e)-1;
	return sgn(e-norm(d))+1;
}
//AOJ0153
//三角形 abc に d が含まれるか？0-no,1-edge,2-in
int cont(pt a,pt b,pt c,pt d){
	if(ccw(a,b,c)==-1) swap(b,c);
	return min({ccw(a,b,d),ccw(b,c,d),ccw(c,a,d)})+1;
}
//(a,b) を結ぶ直線を考え，x 座標との交点の y 座標を求める
//(分子,分母)を返す
pair<ld,ld> xcut(const pt&a,const pt&b,ld x){
	return mp(a.y*(b.x-x)-b.y*(a.x-x),b.x-a.x);
}
//XXII Opencup Gpt of Ural K
pt rot90(pt a){
	return pt(-a.y,a.x);
}
#ifdef GEOF
ld xcutval(const pt&a,const pt&b,ld x){
	auto [p,q]=xcut(a,b,x);
	return p/q;
}
//AOJ1183
//Xmas2010 E
//-+ の 順で返す
//a の符号により，small/large が決まる
int qeq(ld a,ld b,ld c,ld&d,ld&e){
	if(sgn(a)==0){
		if(sgn(b)==0)return 0;
		d=-c/b;
		return 1;
	}
	ld f=b*b-4*a*c;
	if(sgn(f)<0)return 0;
	ld g=sqrt(max(f,ld(0)));
	d=(-b-g)/(2*a);
	e=(-b+g)/(2*a);
	return sgn(f)+1;
}
#else
pt normdir(pt a){
	if(a==pt(0,0))return a;
	int g=gcd(a.x,a.y);
	return pt(a.x/g,a.y/g);
}
#endif

ld area2(const vc<pt>&ps){
	ld res=0;
	rep(i,si(ps))res+=crs(ps[i],ps[(i+1)%si(ps)]);
	return res;
}
