//VERIFY: yosupo
//AOJGRL5C
template<class t,class u>
struct sparsetable{
	vvc<t> a;
	u f;
	const t id;
	sparsetable(const vc<t>& d,u ff,t w):f(ff),id(w){
		if(d.empty())return;
		int n=d.size(),h=topbit(n);
		a.resize(h+1);
		a[0]=d;
		rng(j,1,h+1){
			a[j].resize(n-(1<<j)+1);
			rep(i,n-(1<<j)+1)
				a[j][i]=f(a[j-1][i],a[j-1][i+(1<<(j-1))]);
		}
	}
	t get(int b,int e){
		assert(b<=e);
		if(b==e)return id;
		int d=topbit(e-b);
		return f(a[d][b],a[d][e-(1<<d)]);
	}
};
const auto imin=[](int a,int b){
	return min(a,b);
};
using minst=sparsetable<int,decltype(imin)>;
const auto imax=[](int a,int b){
	return max(a,b);
};
using maxst=sparsetable<int,decltype(imax)>;

SA sa(t);
sparsetable<int,decltype(imin)> st(sa.lcp,imin,inf);

const int L=21;
const int S=1<<L;
int buf[L][S];
void spinit(const vi&d){
	int n=d.size(),h=topbit(n);
	rep(i,n)buf[0][i]=d[i];
	rng(j,1,h+1){
		rep(i,n-(1<<j)+1)
			buf[j][i]=min(buf[j-1][i],buf[j-1][i+(1<<(j-1))]);
	}
}
int spget(int b,int e){
	assert(b<=e);
	if(b==e)return inf;
	int d=topbit(e-b);
	return min(buf[d][b],buf[d][e-(1<<d)]);
}
