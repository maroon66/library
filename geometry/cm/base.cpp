#define GEOF

#ifdef GEOF
using ld=long double;
#else
using ld=ll;
#endif
using cm=complex<ld>;
#define x real()
#define y imag()
const ld eps=1e-9;
#ifdef GEOF
const ld PI=acos(ld(-1));
#endif
int sgn(ld a){return a<-eps?-1:(a>eps?1:0);}
int sgn(ld a,ld b){return sgn(a-b);}
bool incld(ld a,ld b,ld c){return sgn(a,b)<=0&&sgn(b,c)<=0;}
ld rabs(cm a){return max(abs(a.x),abs(a.y));}
ld signed_rabs(cm a){
	if(abs(a.x)>abs(a.y))return a.x;
	else return a.y;
}
ld dot(cm a,cm b){return a.x*b.x+a.y*b.y;}
ld crs(cm a,cm b){return a.x*b.y-a.y*b.x;}
ld crs(cm a,cm b,cm c){return crs(b-a,c-a);}
int ccw(cm a,cm b){return sgn(crs(a,b));}
int ccw(cm a,cm b,cm c){return ccw(b-a,c-a);}
auto cmless=[](cm a,cm b){
	int s=sgn(a.x-b.x);
	if(s)return s<0;
	else return sgn(a.y-b.y)<0;
};
bool cmeq(cm a,cm b){
	return sgn(a.x-b.x)==0&&sgn(a.y-b.y)==0;
};
//(-pi,0](0,pi]
int argtype(cm a){
	if(sgn(a.y)==0)return a.x<0?1:0;
	return a.y<0?0:1;
}
int argcmp(cm a,cm b){
	int at=argtype(a),bt=argtype(b);
	if(at!=bt)return at<bt?-1:1;
	return -ccw(a,b);
};
bool argless(cm a,cm b){return argcmp(a,b)<0;}
//(-2)[a,-1](0)[b,1](2)
int bet(cm a,cm b,cm c){
	cm d=b-a;
	ld e=dot(d,c-a);
	if(sgn(e)<=0)return sgn(e)-1;
	return sgn(e-norm(d))+1;
}
//AOJ0153
//0-no,1-edge,2-in
int cont(cm a,cm b,cm c,cm d){
	if(ccw(a,b,c)==-1)
		swap(b,c);
	return min({ccw(a,b,d),ccw(b,c,d),ccw(c,a,d)})+1;
}
#ifdef GEOF
//AOJ1183
//Xmas2010 E
//-+ small/large の 順で返す
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
//AOJ1183
//arg between ab
//assume given lengths are valid
ld arg(ld a,ld b,ld c){
	return acos(min(max((a*a+b*b-c*c)/(2*a*b),ld(-1)),ld(1)));
}
//AOJ2233
//a->b->c と進むときに曲がる角度
//a-b-cが一直線上にあれば0が帰る
ld turn(cm a,cm b,cm c){
	return arg((c-b)/(b-a));
}
#endif
cm rot90(cm a){
	return cm(-a.y,a.x);
}

#ifndef GEOF
int gcd(int a,int b){return b?gcd(b,a%b):a;}
cm normdir(cm a){
	if(a==cm(0,0))return a;
	int g=gcd(abs(a.x),abs(a.y));
	return cm(a.x/g,a.y/g);
}
#endif

cm&operator*=(cm&a,const cm&b){
	return a=cm(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x);
}

//not verified
//assume there exists a triangle
ld heron(ld a,ld b,ld c){
	ld s=(a+b+c)/2;
	return sqrt(s*max(s-a,ld(0))*max(s-b,ld(0))*max(s-c,ld(0)));
}

cm readcm(){
	ld a,b;cin>>a>>b;
	return cm(a,b);
}

//normalize to [-PI,PI)
ld normarg(ld a){
	ld res=fmod(a+PI,2*PI);
	if(res<0)res+=PI;
	else res-=PI;
	return res;
}
