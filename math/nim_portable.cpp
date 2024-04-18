//10^6 64bit mult takes 168ms
namespace nimber{
	const uint M=(1<<16)-1,S=2*M+7;
	uint ln[M+1],e[S*2+7];
	template<int k>uint slow(uint a,uint b){
		if constexpr(k==0)return a&b;
		else{
			uint s=1u<<(k-1),m=(1u<<s)-1;
			uint x=a>>s,p=a&m,y=b>>s,q=b&m,z=slow<k-1>(p,q);
			return (slow<k-1>(x^p,y^q)^z)<<s^slow<k-1>(slow<k-1>(x,y),1u<<(s-1))^z;
		}
	}
	void init(){
		const uint w=10279;
		uint cur=1;
		rep(i,M){
			e[i]=cur;
			ln[e[i]]=i;
			cur=slow<4>(cur,w);
		}
		rng(i,M,S)e[i]=e[i-M];
		ln[0]=S;
	}
	uint mult(uint a,uint b){
		uint x=a>>16,p=a&M,y=b>>16,q=b&M,z=e[ln[p]+ln[q]];
		return (e[ln[x^p]+ln[y^q]]^z)<<16^e[3+ln[x]+ln[y]]^z;
	}
	uint x31(uint a){
		uint x=a>>16,p=a&M;
		return e[3+ln[x^p]]<<16^e[6+ln[x]];
	}
	ull mult(ull a,ull b){
		uint x=a>>32,p=a,y=b>>32,q=b,z=mult(p,q);
		return ull(mult(x^p,y^q)^z)<<32^x31(mult(x,y))^z;
	}
	struct nim{
		ull v;
		nim(ull vv=0):v(vv){}
		nim& operator+=(const nim&rhs){v^=rhs.v;return *this;}
		nim& operator-=(const nim&rhs){v^=rhs.v;return *this;}
		nim& operator*=(const nim&rhs){v=mult(v,rhs.v);return *this;}
		nim& operator/=(const nim&rhs){return (*this)*=rhs.inv();}
		nim& operator&=(const nim&rhs){v&=rhs.v;return *this;}
		nim& operator^=(const nim&rhs){v^=rhs.v;return *this;}
		nim& operator<<=(const int u){v<<=u;return *this;}
		nim& operator>>=(const int u){v>>=u;return *this;}
		nim operator+(const nim&rhs)const{return nim(*this)+=rhs;}
		nim operator-(const nim&rhs)const{return nim(*this)-=rhs;}
		nim operator*(const nim&rhs)const{return nim(*this)*=rhs;}
		nim operator/(const nim&rhs)const{return nim(*this)/=rhs;}
		nim operator&(const nim&rhs)const{return nim(*this)&=rhs;}
		nim operator^(const nim&rhs)const{return nim(*this)^=rhs;}
		nim operator<<(const int u)const{return nim(*this)<<=u;}
		nim operator>>(const int u)const{return nim(*this)>>=u;}
		bool operator==(const nim&rhs)const{return v==rhs.v;}
		explicit operator bool()const{return v;}
		nim pow(ull n)const{
			nim res=1,x=*this;
			while(n){
				if(n&1)res*=x;
				x*=x;
				n>>=1;
			}
			return res;
		}
		nim inv()const{
			return pow(ull(-2));
		}
	};
	istream& operator>>(istream&is,nim&x){
		return is>>x.v;
	}
	ostream& operator<<(ostream&os,const nim&x){
		return os<<x.v;
	}
	
	nim sqrt(nim v){
		return v.pow(1ull<<63);
	}
	
	//https://natsugiri.hatenablog.com/entry/2020/03/29/073605
	//UCUP2-5
	
	nim quadratic_equation_b1_table[256];
	void initquad(){
		rep(x_,256){
			nim x=x_; 
			quadratic_equation_b1_table[(x*x+x).v]=x;
		}
	}
	
	int middle(nim x) {
		if ((x>>8) == 0) return 4;
		if ((x>>16) == 0) return 8;
		if ((x>>32) == 0) return 16;
		return 32;
	}
	// find x: xx + x = c;
	// answer: x, x+1;
	nim quadratic_equation_b1(nim c) {
		assert((c>>63)==0); // assert c < 2^{63};
		int p = middle(c<<1);
		if (p == 4) return quadratic_equation_b1_table[c.v];
		nim mask = ~0u>>(32-p);
		nim H = 1u<<(p-1);
		nim ch = c >> p;
		nim cl = c & mask;
		nim xh = quadratic_equation_b1(ch);
		nim z = sq(xh)*H;
		if ((z ^ cl) & H) { xh ^= 1; z ^= H.v; }
		nim xl = quadratic_equation_b1(z ^ cl);
		return (xh<<p) ^ xl;
	}

	// find x: xx + bx = c;
	// answer: x, x+b;
	nim quadratic_equation(nim b, nim c) {
		if (b.v == 0) return sqrt(c);
		nim d = (b == 1? c: c*sq(b).inv());
		assert((d>>63)==0); // assert c/(b^2) < 2^{63};
		nim x = quadratic_equation_b1(d);
		return (b == 1? x : b*x);
	}
}
