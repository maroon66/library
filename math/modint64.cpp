//Montgomery
//Ucup1-10 G (モンゴメリじゃないとTLE)
struct modinfo{
	using u128=__uint128_t;
	ull n,n2,r,t,e;
	modinfo(ull nn){
		n=nn;
        assert(n<(1ull<<62));
        assert(n%2==1);
        n2=n*2;
        r=n&3;
        rep(_,5)r*=2-n*r;
        r=-r;
        assert(r*n==-1ull);
        t=-ull(n)%n;
        e=-u128(n)%n;
	}
	ull add(ull x,ull y)const{x+=y;return x<n2?x:x-n2;}
	ull re(u128 x)const{return (x+u128(ull(x)*r)*n)>>64;}
	ull mult(ull x,ull y)const{return re(u128(x)*y);}
	ull en(ull x)const{return mult(x,e);}
	ull de(ull x)const{x=re(x);return x<n?x:x-n;}
};
template<modinfo const&ref>
struct modular{
	static constexpr ull const &mod=ref.n;
	static constexpr ull const &mod2=ref.n2;
	ull v;
	modular(ll vv=0){v=ref.en(vv%(ll)mod+mod);}
	modular operator-()const{return modular()-*this;}
	modular&operator+=(const modular&rhs){v=ref.add(v,rhs.v);return *this;}
	modular&operator-=(const modular&rhs){v=ref.add(v,mod2-rhs.v);return *this;}
	modular&operator*=(const modular&rhs){v=ref.mult(v,rhs.v);return *this;}
	modular&operator/=(const modular&rhs){return *this*=rhs.inv();}
	modular operator+(const modular&rhs)const{return modular(*this)+=rhs;}
	modular operator-(const modular&rhs)const{return modular(*this)-=rhs;}
	modular operator*(const modular&rhs)const{return modular(*this)*=rhs;}
	modular operator/(const modular&rhs)const{return modular(*this)/=rhs;}
	modular pow(ll n)const{
		modular res(1),x(*this);
		while(n){
			if(n&1)res*=x;
			x*=x;
			n>>=1;
		}
		return res;
	}
	modular inv()const{return pow(mod-2);}
	friend modular operator+(ll x,const modular&y){
		return modular(x)+y;
	}
	friend modular operator-(ll x,const modular&y){
		return modular(x)-y;
	}
	friend modular operator*(ll x,const modular&y){
		return modular(x)*y;
	}
	friend modular operator/(ll x,const modular&y){
		return modular(x)/y;
	}
	ull val()const{return ref.de(v);}
	friend ostream& operator<<(ostream&os,const modular&m){
		return os<<m.val();
	}
	friend istream& operator>>(istream&is,modular&m){
		ll x;is>>x;
		m=modular(x);
		return is;
	}
	bool operator<(const modular&r)const{return val()<r.val();}
	bool operator==(const modular&r)const{return val()==r.val();}
	bool operator!=(const modular&r)const{return val()!=r.val();}
	explicit operator bool()const{
		return val();
	}
};

template<class mint>
ll m2l(mint a){
	ull v=a.val();
	return v<mint::mod/2?v:ll(v)-ll(mint::mod);
}

//2^62 未満での最大の素数
const modinfo base(4611686018427387847ll);
//modinfo base(1);
using mint=modular<base>;

//random primitive root
//3277392570379474389

mint parity(int i){
	return i%2==0?1:-1;
}

vc<mint> powbuf(mint v,int n){
	vc<mint> res(n+1);
	res[0]=1;
	rep(i,n)res[i+1]=res[i]*v;
	return res;
}
