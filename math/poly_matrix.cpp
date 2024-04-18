JSC2023FinalF

const int S=2;
using V=array<Poly<mint>,S>;
using M=array<V,S>;

int vpdeg(const V&a){
	int res=0;
	rep(i,S)chmax(res,si(a[i])-1);
	return res;
}
int mpdeg(const M&a){
	int res=0;
	rep(i,S)rep(j,S)chmax(res,si(a[i][j])-1);
	return res;
}

V mvpmul(M a,V b){
	int n=mpdeg(a)+vpdeg(b)+1;
	int s=minp2(n);
	rep(i,S)rep(j,S){
		a[i][j].resize(s);
		inplace_fmt(a[i][j],false);
	}
	rep(i,S){
		b[i].resize(s);
		inplace_fmt(b[i],false);
	}
	V res;
	rep(i,S)res[i].resize(s);
	rep(i,S)rep(j,S)rep(x,s)res[i][x]+=a[i][j][x]*b[j][x];
	rep(i,S){
		inplace_fmt(res[i],true);
		res[i].resize(n);
	}
	return res;
}

M mmpmul(M a,M b){
	int n=mpdeg(a)+mpdeg(b)+1;
	int s=minp2(n);
	rep(i,S)rep(j,S){
		a[i][j].resize(s);
		inplace_fmt(a[i][j],false);
	}
	rep(i,S)rep(j,S){
		b[i][j].resize(s);
		inplace_fmt(b[i][j],false);
	}
	M res;
	rep(i,S)rep(j,S)res[i][j].resize(s);
	rep(i,S)rep(j,S)rep(k,S)rep(x,s)res[i][k][x]+=a[i][j][x]*b[j][k][x];
	rep(i,S)rep(j,S){
		inplace_fmt(res[i][j],true);
		res[i][j].resize(n);
	}
	return res;
}

M mpinv(const M&a,int s){
	M res;
	{
		vvc<mint> c(S,vc<mint>(S));
		rep(i,S)rep(j,S)c[i][j]=a[i][j].freq(0);
		bool ok=inverse(c);
		assert(ok);
		rep(i,S)rep(j,S)res[i][j].pb(c[i][j]);
	}
	for(int n=1;n<s;n*=2){
		M p,q,r=res;
		rep(i,S)rep(j,S){
			p[i][j]=a[i][j].low(n*2);
			inplace_fmt(p[i][j],false);
		}
		rep(i,S)rep(j,S)q[i][j].resize(n*2);
		rep(i,S)rep(j,S){
			r[i][j].resize(n*2);
			inplace_fmt(r[i][j],false);
		}
		rep(i,S)rep(j,S)rep(k,S)rep(x,n*2)q[i][k][x]+=p[i][j][x]*r[j][k][x];
		rep(i,S)rep(j,S){
			inplace_fmt(q[i][j],true);
			rep(x,n)q[i][j][x]=0;
			inplace_fmt(q[i][j],false);
		}
		rep(i,S)rep(j,S)fill(all(p[i][j]),0);
		rep(i,S)rep(j,S)rep(k,S)rep(x,n*2)p[i][k][x]+=r[i][j][x]*q[j][k][x];
		rep(i,S)rep(j,S){
			inplace_fmt(p[i][j],true);
			res[i][j].resize(n*2);
			rng(x,n,n*2)res[i][j][x]=-p[i][j][x];
		}
	}
	rep(i,S)rep(j,S)res[i][j].resize(s);
	return res;
}
