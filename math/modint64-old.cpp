//Codechef 2022 May Lunchtime D
//https://github.com/kth-competitive-programming/kactl/blob/main/content/number-theory/ModMulLL.h
//$a\cdot b\bmod c$ (or $a^b \bmod c$) for $0 \le a, b \le c \le 7.2\cdot 10^{18}$.
/*struct modinfo{
ull mod;
long double d;
void set_mod(ull m){
	mod=m;
	d=(long double)1/m;
}
modinfo(ull m=1){set_mod(m);}
ull product(ull a, ull b)const{
	ll ret = a * b - mod * ull(d * a * b);
	//return ret + mod * (ret < 0) - mod * (ret >= (ll)mod);
	return ret<0?ret+mod:ret>=(ll)mod?ret-mod:ret;
}
};*/
struct modinfo{
	const ull mod;
	modinfo(ull m):mod(m){}
	ull product(ull a,ull b)const{
		return (unsigned __int128)a*b%mod;
	}
};
template<modinfo const&ref>
struct modular{
	static constexpr ull const &mod=ref.mod;
	ull v;
	modular(ll vv=0){s(vv%mod+mod);}
	modular(ull vv){v=vv%mod;}
	modular& s(ull vv){
		v=vv<mod?vv:vv-mod;
		return *this;
	}
	modular operator-()const{return modular()-*this;}
	modular& operator+=(const modular&rhs){return s(v+rhs.v);}
	modular&operator-=(const modular&rhs){return s(v+mod-rhs.v);}
	modular&operator*=(const modular&rhs){
		v=ref.product(v,rhs.v);
		return *this;
	}
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
	friend ostream& operator<<(ostream&os,const modular&m){
		return os<<m.v;
	}
	friend istream& operator>>(istream&is,modular&m){
		ll x;is>>x;
		m=modular(x);
		return is;
	}
	bool operator<(const modular&r)const{return v<r.v;}
	bool operator==(const modular&r)const{return v==r.v;}
	bool operator!=(const modular&r)const{return v!=r.v;}
	explicit operator bool()const{
		return v;
	}
};

template<class mint>
ll m2l(mint a){
	return a.v<mint::mod/2?a.v:ll(a.v)-ll(mint::mod);
}

const modinfo base(9000000000000000041LL);
//modinfo base(1);
using mint=modular<base>;
