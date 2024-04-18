//Xmas2011 C
//XX Opencup GP of Zhejiang J
//include poly_easy
//assume f is monic
struct gf{
	static int k;
	static vc<mint> f;
	vc<mint> x;
	gf():x(k){}
	gf(const vc<mint>&xx):x(xx){}
	mint&operator[](int i){
		return x[i];
	}
	const mint operator[](int i)const{
		return x[i];
	}
	gf operator+(const gf&rhs)const{
		gf res;
		rep(i,k)res[i]+=x[i]+rhs[i];
		return res;
	}
	gf operator*(const gf&rhs)const{
		vc<mint> res(2*k-1);
		rep(i,k)rep(j,k)res[i+j]+=x[i]*rhs[j];
		gnr(i,k,2*k-1)rep(j,k)res[i-k+j]-=res[i]*f[j];
		res.resize(k);
		return gf(res);
	}
	static void set_degree(int kk){
		k=kk;
		f=get_irreducible_poly(k);
	}
	int to_int()const{
		int res=0;
		per(i,k)res=res*mint::mod+x[i].v;
		return res;
	}
	static gf from_int(int v){
		gf res;
		rep(i,k){
			res[i]=v;
			v/=mint::mod;
		}
		return res;
	}
};
int gf::k;
vc<mint> gf::f;
