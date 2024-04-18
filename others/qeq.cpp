using ld=long double;

const ld eps=1e-9;
int sgn(ld a){return a<-eps?-1:(a>eps?1:0);}

//AOJ1183
//Xmas2010 E
//-+ small/large の 順で返す
int qeq(ld a,ld b,ld c,ld&d,ld&e){
	if(a<0){
		a=-a;
		b=-b;
		c=-c;
	}
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

//integer solutions
//XXII Opencup BP of Belarus
int qeq_int(int a,int b,int c,int&d,int&e){
	ld x[2];
	int s=qeq(a,b,c,x[0],x[1]);
	int res=0;
	rep(i,s){
		rng(v,x[i]-1,x[i]+2){
			if(a*v*v+b*v+c==0){
				if(res==0){
					d=v;
					res++;
				}else if(res<2&&d!=v){
					e=v;
					res++;
				}
			}
		}
	}
	assert(res<=2);
	return res;
}

using ld=long double;
const ld infld=1/.0;

const ld eps=1e-9;
int sgn(ld a){return a<-eps?-1:(a>eps?1:0);}

//UCUP 2-26-A
ld parabora_min(ld a,ld b,ld c,ld l,ld r){
	ld res=infld;
	auto eval=[&](ld t){
		chmin(res,a*t*t+b*t+c);
	};
	eval(l);
	eval(r);
	if(sgn(a)){
		ld t=-b/(2*a);
		if(l<=t&&t<=r)eval(t);
	}
	return res;
}
