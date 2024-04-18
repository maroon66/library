//VERIFY:yosupo

// for all valid i, sum_j a_{i+j}*res_j = 0
// res[last]=1
// x^n % res で n 項目が求まる
// 多項式の向きに注意
vc<mint> berlekamp_massey(const vc<mint>& a){
	vc<mint> b{1},c{0};
	mint d=1;
	rep(i,a.size()){
		int n=b.size();
		mint w=0;
		rep(j,n)
			w+=a[i-(n-1)+j]*b[j];
		c.pb(0);
		if(!w.v)continue;
		int m=c.size();
		mint e=-w/d;
		if(m>n){
			auto tmp=b;
			b.insert(b.bg,m-n,0);
			rep(j,m)
				b[j]+=e*c[j];
			c=tmp;
			d=w;
		}else{
			rep(j,m)
				b[n-m+j]+=e*c[j];
		}
	}
	return b;
}

//Codechef 2022 April Lunchtime ODDSPLIT
void extend_sequence(vc<mint>&a,int k){
	auto p=berlekamp_massey(a);
	int s=si(p);
	assert((s-1)*2<=si(a));
	int cur=si(a);
	a.resize(k);
	rng(n,cur,k)rep(i,s-1)a[n]-=a[n-(s-1)+i]*p[i];
}

mint get_kth(const vc<mint>&a,int k){
	auto p=berlekamp_massey(a);
	assert((si(p)-1)*2<=si(a));
	if(si(p)==1)return 0;
	auto q=pow_mod_f({0,1},k,p);
	mint res=0;
	rep(i,si(q))res+=a[i]*q[i];
	return res;
}

//VERIFY:
//PE***
template<class F>
vc<mint> get_kth_vector(int k,vc<mint> a,F f){
	int n=si(a),s=2*n+1;
	vvc<mint> buf(s);
	buf[0]=a;
	rep(i,s-1)buf[i+1]=f(buf[i]);
	if(k<s)return buf[k];
	vc<mint> wei(n);
	rep(i,n)wei[i]=rand_int(1,mint::mod-1);
	vc<mint> tmp(s);
	rep(i,s)rep(j,n)tmp[i]+=buf[i][j]*wei[j];
	auto p=berlekamp_massey(tmp);
	vc<mint> res(n);
	if(si(p)==1)return res;
	auto q=pow_mod_f({0,1},k,p);
	rep(i,si(q))rep(j,n)res[j]+=buf[i][j]*q[i];
	return res;
}

//VERIFY: yosupo
//Petrozavodsk2019W Day1 E
//Xmas2013 D
//g は列->行のイメージだが，実際はどちらでも良い（det(A)=det(A^T)）
mint sparse_det(const vvc<pair<int,mint>>&g){
	int n=g.size();
	if(n==0)return 1;
	mt19937 rnd;
	while(1){
		vc<mint> d(n),l(n),r(n);
		rep(i,n)d[i]=rnd()%(mint::mod-1)+1;
		rep(i,n)l[i]=rnd()%(mint::mod-1)+1;
		rep(i,n)r[i]=rnd()%(mint::mod-1)+1;
		vc<mint> w(2*n);
		rep(i,2*n){
			rep(j,n)w[i]+=l[j]*r[j];
			rep(j,n)r[j]*=d[j];
			vc<mint> s(n);
			rep(j,n)for(auto e:g[j])
				s[e.a]+=r[j]*e.b;
			r=s;
		}
		vc<mint> p=berlekamp_massey(w);
		if(!p[0].v)return 0;
		if(int(p.size())<n+1)
			continue;
		mint res=p[0];
		if(n%2)res=-res;
		rep(i,n)res/=d[i];
		return res;
	}
}
