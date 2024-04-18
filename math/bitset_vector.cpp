//Multiuni2023-8 F
int S,L;
void setSL(int m){
	S=m;
	L=(S+63)/64;
}
struct V{
	vc<ull> x;
	V():x(L){}
	bool get(int i)const{
		return (x[i/64]>>(i%64))&1;
	}
	void reset(int i){
		x[i/64]&=~(1ll<<(i%64));
	}
	void set(int i){
		x[i/64]|=1ll<<(i%64);
	}
	void xorsuf(const V&a,int i){
		i/=64;
		while(i<L){
			x[i]^=a.x[i];
			i++;
		}
	}
	int find_first()const{
		rep(i,L)if(x[i])return i*64+botbit(x[i]);
		return S;
	}
	bool operator<(const V&r)const{
		rep(i,S)if(get(i)!=r.get(i))return get(i)<r.get(i);
		return false;
	}
};
ostream& operator<<(ostream&os,const V&v){
	rep(i,S)os<<v.get(i);
	return os;
}

V readV(){
	V res;
	if(dbg){
		rep(i,S)if(rand_int(2))res.set(i);
	}else{
		string s;sc.read(s);
		rep(i,S)if(s[i]=='1')res.set(i);
	}
	return res;
}
