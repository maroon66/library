struct mybitset{
	vc<ull> x;
	mybitset(int B):x((B+63)/64){};
	void set(int i){
		x[i/64]|=1ull<<(i&63);
	}
	void reset(int i){
		x[i/64]&=~(1ull<<(i&63));
	}
	void set(int i,bool v){
		if(v)set(i);
		else reset(i);
	}
	int _Find_first(){
		rep(i,si(x))if(si(x)){
			return i*64+botbit(x[i]);
		}
		return si(x)*64;
	}
	mybitset& operator^=(const mybitset&rhs){
		rep(i,si(x))x[i]^=rhs.x[i];
		return *this;
	}
	mybitset operator^(const mybitset&rhs)const{
		return mybitset(*this)^=rhs;
	}
	bool operator==(const mybitset&rhs)const{
		return x==rhs.x;
	}
	bool operator<(const mybitset&rhs)const{
		return x<rhs.x;
	}
	bool any()const{
		rep(i,si(x))if(x[i])return true;
		return false;
	}
};
