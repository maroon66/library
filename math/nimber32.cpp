//Prime New Year Contest 2022 47
//TLE
namespace Nimber32{
	const uint s=-1;
	
	uint multiply_powers_of_2(uint,uint);

	uint multiply(uint a,uint b) {
	   uint ans = 0;
	   rep(p1,32)if((a>>p1)&1)
		   rep(p2,32)if((b>>p2)&1)
			  ans=ans^multiply_powers_of_2(uint(1)<<p1,uint(1)<<p2);
	   return ans;
	}
	uint multiply_powers_of_2(uint a, uint b) {
		if (a == 1 or b == 1) return a * b;
		int n=__lg(__lg(max(a,b)));
		uint power = uint(1)<<(1<<n);
		if (a >= power and b >= power) {
			return multiply(power * 3 / 2, multiply_powers_of_2(a / power, b / power));
		} else if (a >= power) {
			return multiply_powers_of_2(a / power, b) * power;
		} else {
			return multiply_powers_of_2(a, b / power) * power;
		}
	}
	
	struct precalc{
		uint tmp[32][32];
		unsigned char x[256][256];
		uint y[4][4][256];
		template<int len>
		uint multfast(uint a,uint b){
			uint c=0;
			rep(i,len)if((a>>i)&1)rep(j,len)if((b>>j)&1)c^=tmp[i][j];
			return c;
		}
		precalc():tmp(),x(),y(){
			rep(i,32)rep(j,32)tmp[i][j]=multiply_powers_of_2(1U<<i,1U<<j);
			rep(i,256)rep(j,256)x[i][j]=multfast<8>(i,j);
			rep(i,4)rep(j,i+1)rep(k,256)y[i][j][k]=multfast<32>(tmp[i*8][j*8],k);
		}
		uint multfast2(uint a,uint b)const{
			uint res=0;
			rep(i,4){
				rep(j,i){
					res^=y[i][j][
					x[(a>>(i*8))&255][(b>>(j*8))&255]^
					x[(a>>(j*8))&255][(b>>(i*8))&255]];
				}
				res^=y[i][i][x[(a>>(i*8))&255][(b>>(i*8))&255]];
			}
			return res;
		}
	};
	const precalc buf;
	
	struct nim{
		uint v;
		nim(uint vv=0):v(vv){}
		nim& operator+=(const nim&rhs){v^=rhs.v;return *this;}
		nim& operator*=(const nim&rhs){v=buf.multfast2(v,rhs.v);return *this;}
		nim& operator/=(const nim&rhs){return (*this)*=rhs.inv();}
		nim operator+(const nim&rhs)const{return nim(*this)+=rhs;}
		nim operator*(const nim&rhs)const{return nim(*this)*=rhs;}
		nim operator/(const nim&rhs)const{return nim(*this)/=rhs;}
		nim pow(uint n)const{
			nim res=1,x=*this;
			while(n){
				if(n&1)res*=x;
				x*=x;
				n>>=1;
			}
			return res;
		}
		nim inv()const{
			return pow(uint(-2));
		}
	};
	istream& operator>>(istream&is,nim&x){
		return is>>x.v;
	}
	ostream& operator<<(ostream&os,const nim&x){
		return os<<x.v;
	}
}
using Nimber32::nim;

//Copy-pasted from
//https://judge.yosupo.jp/submission/19835

#define lg2 std::__lg
#define ctz __builtin_ctz

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

namespace nimbers {
	constexpr u32 n2f[16] = {0x0001u, 0x071cu, 0x6bd1u, 0x1224u, 0x6ba8u, 0x1333u, 0x1553u, 0x0007u, 0x071eu, 0x0925u, 0xc586u, 0x5dbdu, 0xc463u, 0x5efdu, 0x2aa1u, 0x155au},
				  f2n[16] = {0x0001u, 0x0102u, 0x0183u, 0x8041u, 0x015cu, 0x5f24u, 0xde2cu, 0x957eu, 0x01f4u, 0xf7d8u, 0x76b0u, 0x5d52u, 0xa977u, 0x20d1u, 0xc1a4u, 0x271fu};
	inline u32 nimber2field(u32 x) {u32 y = 0; for (; x; x &= x - 1) y ^= n2f[ctz(x)]; return y;}
	inline u32 field2nimber(u32 x) {u32 y = 0; for (; x; x &= x - 1) y ^= f2n[ctz(x)]; return y;}
	inline u32 __builtin_double(u32 x) {return x << 1 ^ (x < 32768 ? 0 : 0x16babu);}

	u16 ln[65536], exp[196605], *Hexp = exp + 62133, *H2exp = exp + 58731;

	inline void init() {
		int i;
		for (*exp = i = 1; i < 65535; ++i) exp[i] = __builtin_double(exp[i - 1]);
		for (i = 1; i < 65535; ++i) exp[i] = field2nimber(exp[i]), ln[exp[i]] = i;
		memcpy(exp + 65535, exp, 131070);
		memcpy(exp + 131070, exp, 131070);
	}

	inline u16 product(u16 A, u16 B) {return A && B ? exp[ln[A] + ln[B]] : 0;}
	inline u16 H(u16 A) {return A ? Hexp[ln[A]] : 0;}
	inline u16 H2(u16 A) {return A ? H2exp[ln[A]] : 0;}
	inline u16 Hproduct(u16 A, u16 B) {return A && B ? Hexp[ln[A] + ln[B]] : 0;}

	inline u32 product(u32 A, u32 B) {
		u16 a = A & 65535, b = B & 65535, c = A >> 16, d = B >> 16, e = product(a, b);
		return u32(product(u16(a ^ c), u16(b ^ d)) ^ e) << 16 | (Hproduct(c, d) ^ e);
	}

	inline u32 H(u32 A) {
		u16 a = A & 65535, b = A >> 16;
		return H(u16(a ^ b)) << 16 | H2(b);
	}

	inline u64 product(u64 A, u64 B) {
		u32 a = A & UINT_MAX, b = B & UINT_MAX, c = A >> 32, d = B >> 32, e = product(a, b);
		return u64(product(a ^ c, b ^ d) ^ e) << 32 | (H(product(c, d)) ^ e);
	}
}
