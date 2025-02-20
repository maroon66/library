//UCUP 3-27-C
using V=double;
const int S=100;
const int M=ten(18);
struct N{
	V buf[S];
	N(){
		rep(i,S)buf[i]=V(rand_int(0,M));
	}
	static N mg(const N&a,const N&b){
		N res;
		rep(i,S)res.buf[i]=min(a.buf[i],b.buf[i]);
		return res;
	}
	int getsize()const{
		V a=0;
		rep(i,S)a+=buf[i];
		a/=S;
		return round(M/a)-1;
	}
};
