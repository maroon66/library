// https://qoj.ac/contest/763/problem/4920
////////////////////////////////////////////////////////////////////////////////
#ifndef __x86_64__
#error Only x86-64 targets are supported
#endif
#include<cstdint>
#include<vector>
#include<string>
#include<iosfwd>
#define __builtin_ia32_adc(x,y,flag) __asm__("addb   %3, %0\n\t" "adcq   %2, %1\n\t" "setc   %0":"+r"(flag),"+r"(x):"r"(y),"i"(-1):"cc")

struct bigint{// made by dengyaotriangle!
    typedef unsigned long long u64;
    typedef unsigned __int128 u128;
    typedef std::size_t st;
    std::vector<u64> data;
    bigint(){}
    bigint(u64 x):data(x?std::vector<u64>{x}:std::vector<u64>{}){}
    bigint(const std::string &s){
        st pos=s.length();
        int cnt=0;
        u64 val=0;
        while(pos){
            pos--;
            if(cnt==64){
                data.push_back(val);
                val=0;cnt=0;
            }
            val|=(u64)(s[pos]=='1')<<cnt;
            ++cnt;
        }
        if(cnt&&val)data.push_back(val);
    }
    explicit operator std::string()const{
        if(data.empty())return "0";
        bool t=0;
        std::string ret;
        for(int i=63;i>=0;i--){
            t|=(data.back()>>i)&1;
            if(t)ret+='0'|((data.back()>>i)&1);
        }
        st i=data.size()-1;
        while(i){
            i--;
            for(int j=63;j>=0;j--)ret+='0'|((data[i]>>j)&1);
        }
        return ret;
    }
    explicit operator bool()const{return !data.empty();}
    explicit operator u64()const{return data.empty()?0:data[0];}
    st digit()const{
        if(data.empty())return 0;
        return (data.size()<<6)-__builtin_clzll(data.back());
    }
    bool operator==(const bigint &a)const{return a.data==data;}
    bool operator!=(const bigint &a)const{return a.data!=data;}
    bool operator<(const bigint &a)const{
        if(data.size()!=a.data.size())return data.size()<a.data.size();
        for(st i=data.size();i;){
            i--;
            if(data[i]!=a.data[i])return data[i]<a.data[i];
        }
        return 0;
    }
    bool operator>(const bigint &a)const{return a<(*this);}
    bool operator<=(const bigint &a)const{return !(*this>a);}
    bool operator>=(const bigint &a)const{return !(*this<a);}
    bigint &operator<<=(st n){
        if(data.empty())return *this;
        int w=n&63;st z=n>>6;
        st i=data.size();
        bool flg=0;
        if(w&&(data.back()>>(64-w)))data.push_back(0),flg=1;
        data.resize(data.size()+z);
        while(i){
            i--;
            if(flg)data[i+z+1]|=data[i]>>(64-w);
            data[i+z]=data[i]<<w;
            flg|=bool(w);
        }
        for(st i=0;i<z;i++)data[i]=0;
        return *this;
    }
    bigint &operator>>=(st n){
        int w=n&63;st z=n>>6,i=0;
        for(;i+z<data.size();i++){
            if(w&&i)data[i-1]|=data[i+z]<<(64-w);
            data[i]=data[i+z]>>w;
        }
        while(data.size()>i)data.pop_back();
        while(!data.empty()&&data.back()==0)data.pop_back();
        return *this;
    }
    bigint operator<<(st n)const{return bigint(*this)<<=n;}
    bigint operator>>(st n)const{return bigint(*this)>>=n;}
    bigint &operator+=(const bigint &a){
        data.resize(std::max(data.size(),a.data.size()));
        bool carry=0;
        for(st i=0;i<data.size();i++){
            u64 rg=0;
            if(i<a.data.size())rg=a.data[i];
            __builtin_ia32_adc(data[i],rg,carry);
        }
        if(carry)data.push_back(1);
        return *this;
    }
    bigint &operator-=(const bigint &a){
        bool carry=1;
        for(st i=0;i<data.size();i++){
            u64 rg=-1;
            if(i<a.data.size())rg=~a.data[i];
            __builtin_ia32_adc(data[i],rg,carry);
        }
        while(!data.empty()&&data.back()==0)data.pop_back();
        return *this;
    }
    bigint &operator++(){return *this+=bigint(1);}
    bigint &operator--(){return *this-=bigint(1);}
    bigint operator++(int){bigint tmp=*this;++*this;return tmp;}
    bigint operator--(int){bigint tmp=*this;--*this;return tmp;}
    bigint &operator*=(const bigint &a){
        std::vector<u64> ret(data.size()+a.data.size());
        for(st i=0;i<data.size();i++){
            u64 carry=0;bool wcarry=0;
            st k=i;
            for(st j=0;j<a.data.size();j++,k++){
                u128 r=data[i]*(u128)a.data[j]+carry;
                u64 cur=r;
                carry=r>>64;
                __builtin_ia32_adc(ret[k],cur,wcarry);
            }
            while(carry||wcarry){
                __builtin_ia32_adc(ret[k],carry,wcarry);
                carry=0;k++;
            }
        }
        while(!ret.empty()&&ret.back()==0)ret.pop_back();
        data=ret;
        return *this;
    }
    bigint &operator/=(const bigint &a){
        if(a.digit()>digit()){
            data.clear();
            return *this;
        }
        st z=digit()-a.digit();
        std::vector<u64> ret;
        while(1){
            bigint tmp=a<<z;
            if(tmp<=*this){
                *this-=tmp;
                st v1=z>>6;
                if(ret.size()<=v1)ret.resize(v1+1);
                ret[v1]|=(u64)(1)<<(z&63);
            }
            if(!z)break;
            z--;
        }
        data=ret;
        return *this;
    }
    bigint &operator%=(const bigint &a){
        if(a.digit()>digit())return *this;
        st z=digit()-a.digit();
        while(1){
            bigint tmp=a<<z;
            if(tmp<=*this)*this-=tmp;
            if(!z)break;
            z--;
        }
        return *this;
    }
    bigint operator+(const bigint &a)const{return bigint(*this)+=a;}
    bigint operator-(const bigint &a)const{return bigint(*this)-=a;}
    bigint operator*(const bigint &a)const{return bigint(*this)*=a;}
    bigint operator/(const bigint &a)const{return bigint(*this)/=a;}
    bigint operator%(const bigint &a)const{return bigint(*this)%=a;}
};
std::istream &operator>>(std::istream &st,bigint &a){
    std::string s;st>>s;a=bigint(s);return st;
}
std::ostream &operator<<(std::ostream &st,const bigint &a){
    return st<<(std::string)(a);
}
////////////////////////////////////////////////////////////////////////////////

// my extension
////////////////////////////////////////////////////////////////////////////////
void swap(bigint &a, bigint &b) {
  a.data.swap(b.data);
}
int bsf(const bigint &a) {
  for (int i = 0; i < (int)a.data.size(); ++i) if (a.data[i]) {
    return i << 6 | __builtin_ctzll(a.data[i]);
  }
  assert(false);
}
bigint gcd(bigint a, bigint b) {
  return b ? gcd(b, a % b) : a;
  // anmari kawaranai?
  /*
  if (!a) return b;
  if (!b) return a;
  const int s = min(bsf(a), bsf(b));
  a >>= bsf(a);
  do {
    b >>= bsf(b);
    if (a > b) swap(a, b);
    b -= a;
  } while (b);
  return a << s;
  //*/
}
bool isOne(const bigint &a) {
  return (a.digit() == 1 && (unsigned long long)a == 1);
}
int mod(const bigint &a, int m) {
  int b = 0;
  for (int i = (int)a.data.size(); --i >= 0; ) {
    b = ((unsigned __int128)b << 64 | a.data[i]) % m;
  }
  return b;
}

namespace montgomery {
int sz;
bigint m, negInvM, two2;
void init(const bigint &m_) {
  assert(m_);
  assert(m_.data[0] & 1);
  m = m_;
  sz = m.data.size();
  negInvM = 1;
  for (int e = 1; e < sz << 6; ) {
    negInvM *= (m * negInvM + 2);
    chmin(e <<= 1, sz << 6);
    if ((int)negInvM.data.size() > (e + 63) >> 6) negInvM.data.resize((e + 63) >> 6);
  }
  assert((m * negInvM + 1) % (bigint(1) << (sz << 6)) == 0);
  two2 = (bigint(1) << (sz << 7)) % m;
}
// 2^(-6sz) a mod m
bigint reduce(const bigint &a) {
  bigint c = a * negInvM;
  if ((int)c.data.size() > sz) c.data.resize(sz);
  c = (a + c * m) >> (sz << 6);
  return (c >= m) ? (c - m) : c;
}
bigint mod(const bigint &a) {
  return reduce(a * two2);
}
bigint mul(const bigint &a, const bigint &b) {
  return reduce(reduce(a * b) * two2);
}
bigint power(const bigint &a, const bigint &e) {
  bigint b = 1;
  for (int i = (int)e.digit(); --i >= 0; ) {
    b = mul(b, b);
    if (e.data[i >> 6] >> (i & 63) & 1) b = mul(b, a);
  }
  return b;
}
}  // montgomery
////////////////////////////////////////////////////////////////////////////////
