int mex(vi x){
	mkuni(x);
	rep(i,si(x))if(i!=x[i])return i;
	return si(x);
}

template<class F>
int lwokupng(int lw,int up,F f){
	//assert(F(lw));
	//assert(!F(up));
	while(up-lw>1){
		int mid=(lw+up)/2;
		if(f(mid))lw=mid;
		else up=mid;
	}
	return lw;
};

//CF658E
//(a,b)-(c,d) を結んで，p の評価
ld intp2(ld a,ld b,ld c,ld d,ld p){
	return (b*(c-p)+d*(p-a))/(c-a);
}

using ld=long double;
const ld PI=acosl(-1);

ld rand_ld(){
	static mt19937_64 gen;
	return ld(gen())/ULONG_LONG_MAX;
}

template<class t,class u>
ostream& operator<<(ostream&os,const map<t,u>&a){
	return os<<vc<pair<t,u>>(all(a));
}

using pq=priority_queue<int>;
void pqmg(pq&a,pq&&b){
	while(si(a)<si(b))swap(a,b);
	while(si(b)){
		a.push(b.top());
		b.pop();
	}
}

template<class mint>
ll m2l(mint a){
	return a.v<mint::mod/2?a.v:ll(a.v)-ll(mint::mod);
}

mint c2(int n){
	int a=n,b=n-1;
	if(a%2==0)a/=2;
	else b/=2;
	return mint(a)*b;
}

int bitrev(int bit,int len){
	int res=0;
	rep(i,len)res+=((bit>>i)&1)<<(len-1-i);
	return res;
}
