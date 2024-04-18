//UCUP 2-27-A

const int S=57;
struct N{
	ull v;
	int L;
	mint m;
	N():v(0),L(0),m(1){}
	void adv(ull b){
		ull nx=(((1ll<<S)+v)*b);
		if(S-L>=0)nx+=1ll<<(S-L);
		int t=topbit(nx);
		//dmp2(nx,t);
		assert(t>=S);
		L+=(t-S);
		v=(nx>>(t-S))&mask(S);
		m=m*b+1;
	}
	void mult(ull x){
		ull nx=(((1ll<<S)+v)*x);
		int t=topbit(nx);
		assert(t>=S);
		L+=(t-S);
		v=(nx>>(t-S))&mask(S);
		m=m*x;
	}
	N operator*(ull x)const{
		N res=*this;
		res.mult(x);
		return res;
	}
	bool operator<(const N&rhs)const{
		if(L!=rhs.L)return L<rhs.L;
		else return v<rhs.v;
	}
};
