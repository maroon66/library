//デバッグ実行でオーバーフローするとコアダンプしがち

using int128=__int128_t;
using uint128=unsigned __int128_t;

namespace std{
istream& operator>>(istream&is,int128&res){
	res=0;
	string s;is>>s;
	int head=0;
	int128 w=1;
	if(s[0]=='-'){
		w=-1;
		head++;
	}
	while(head<int(s.size())){
		res=res*10+s[head++]-'0';
	}
	res*=w;
	return is;
}
ostream& operator<<(ostream&os,int128 i){
	if(i==0)
		return os<<0;
	static char buf[100];
	if(i<0){
		os<<"-";
		i=-i;
	}
	int p=0;
	while(i){
		buf[p++]=char('0'+i%10);
		i/=10;
	}
	reverse(buf,buf+p);
	buf[p]=0;
	return os<<buf;
}
ostream& operator<<(ostream&os,uint128 i){
	if(i==0)
		return os<<0;
	static char buf[100];
	int p=0;
	while(i){
		buf[p++]=char('0'+i%10);
		i/=10;
	}
	reverse(buf,buf+p);
	buf[p]=0;
	return os<<buf;
}
}
int128 abs128(int128 a){
	return a<0?-a:a;
}
int botbit(int128 a){
	const int128 m=(int128(1)<<64)-1;
	if(a&m)return __builtin_ctzll(ll(a));
	else return __builtin_ctzll(ll(a>>64))+64;
}
int128 gcd(int128 a,int128 b){
	if(a<0)a=-a;
	if(b<0)b=-b;
	if(a==0)return b;
	if(b==0)return a;
	int128 s=botbit(a|b);
	a>>=botbit(a);
	do{
		b>>=botbit(b);
		if(a>b)swap(a,b);
		b-=a;
	}while(b);
	return a<<s;
}
const int128 inf128=int128(1)<<122;
//UCUP 2-Prime-97
int128 fld(int128 a,int128 b) { // floored division
	return a / b - ((a ^ b) < 0 && a % b); }
//UCUP 2-Prime-43
int128 cld(int128 a, int128 b) { // ceiled division
	return a / b + ((a ^ b) > 0 && a % b); }
//a,c O(10^36)
//b,d O(10^18)
//a/b, c/d の大小比較
int cmpfrac(int128 a,int128 b,int128 c,int128 d){
	assert(b);
	assert(d);
	if(b<0){
		a=-a;
		b=-b;
	}
	if(d<0){
		c=-c;
		d=-d;
	}
	int128 x=fld(a,b),y=fld(c,d);
	if(x<y)return -1;
	if(x>y)return 1;
	int128 u=(a-x*b)*d,v=(c-y*d)*b;
	if(u<v)return -1;
	if(u>v)return 1;
	return 0;
}
bool inc(int128 a,int128 b,int128 c){
	return a<=b&&b<=c;
}
