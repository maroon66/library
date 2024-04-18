//change LG
//up to size 1<<LG
static const int LG=14;
using R=double;
using C=complex<R>;

C roots[1<<(LG+1)],iroots[1<<(LG+1)];
struct PrepareRoots{
	PrepareRoots(){
		const R P=2*acos(-1);
		rep(w,LG+1){
			const int s=(1<<w)-1;
			rep(i,1<<w){
				const R a=P/(1<<w)*i;
				roots[s+i]=C(cos(a),sin(a));
				iroots[s+i]=conj(roots[s+i]);
			}
		}
	}
} PrepareRootsDummy;

C& operator*=(C &a,const C&b){
	return a=C(a.real()*b.real()-a.imag()*b.imag(),a.real()*b.imag()+a.imag()*b.real());
}

/*void inplace_fft(vc<C>&f,bool inv){
	static const R P=acos(-1);
	static C r[S],ir[S];
	static bool did=false;
	if(!did){
		rep(i,S){
			R a=2*P/S*i;
			r[i]=C(cos(a),sin(a));
			ir[i]=conj(r[i]);
		}
		did=true;
	}
	const int n=f.size();
	vc<C> g(n);
	for(int b=n/2;b>=1;b/=2){
		int w=S/(n/b),p=0;
		for(int i=0;i<n;i+=b*2){
			rep(j,b){
				f[i+j+b]*=inv?ir[p]:r[p];
				g[i/2+j]=f[i+j]+f[i+b+j];
				g[n/2+i/2+j]=f[i+j]-f[i+b+j];
			}
			p+=w;
		}
		swap(f,g);
	}
	if(inv)rep(i,n)
		f[i]/=n;
}*/

/*
//for doubles
vc<C> convolution(vc<C> x,vc<C> y){
	int n=x.size()+y.size()-1;
	int s=1;
	while(s<n)s*=2;
	x.resize(s);inplace_fft(x,false);
	y.resize(s);inplace_fft(y,false);
	rep(i,s)
		x[i]*=y[i];
	inplace_fft(x,true);x.resize(n);
	return x;
}

template<class D>
vc<D> multiply(const vc<D>&x,const vc<D>&y){
	const auto ch=[&](const vc<D>& v){
		vc<C> r(v.size());
		rep(i,v.size())
			r[i]=v[i];
		return r;
	};
	auto z=convolution(ch(x),ch(y));
	vc<D> r(z.size());
	rep(i,z.size())
		r[i]=z[i].real();
	return r;
}

R invs[S];
struct InitRInvs{
	InitRInvs(){
		rng(i,1,S)
			invs[i]=1/i;
	}
} InitRInvsDummy;*/

#define USE_BROKEN_FFT
void broken_fft(C*f,const int n){
	for(int b=n/2;b>=1;b/=2){
		for(int i=0;i<n;i+=b*2){
			rep(j,b){
				C tmp=f[i+j]-f[i+j+b];
				f[i+j]+=f[i+j+b];
				f[i+j+b]=tmp*roots[b*2-1+j];
			}
		}
	}
}

void broken_ifft(C*f,const int n){
	for(int b=1;b<=n/2;b*=2){
		for(int i=0;i<n;i+=b*2){
			rep(j,b){
				f[i+j+b]*=iroots[b*2-1+j];
				C tmp=f[i+j]-f[i+j+b];
				f[i+j]+=f[i+j+b];
				f[i+j+b]=tmp;
			}
		}
	}
	rep(i,n)
		f[i]/=n;
}

void inplace_fft(C*f,const int n,bool inv){
	if(!inv)broken_fft(f,n);
	else broken_ifft(f,n);
}

vc<mint> multiply(const vc<mint>&x,const vc<mint>&y,bool same=false){
	const int B=15;
	int n=x.size()+y.size()-1;
	vc<mint> r(n);
	if(x.size()<=32||y.size()<=32){
		rep(i,x.size())rep(j,y.size())
			r[i+j]+=x[i]*y[j];
		return r;
	}
	int s=1;
	while(s<n)s*=2;
	const auto ch=[&](const vc<mint>&v,C*w){
		rep(i,v.size())
			w[i]=C(v[i].v&((1<<B)-1),v[i].v>>B);
		rng(i,v.size(),s)
			w[i]=0;
		inplace_fft(w,s,false);
	};
	static C a[1<<LG],b[1<<LG],c[1<<LG],d[1<<LG];
	ch(x,a);
	if(!same)
		ch(y,b);
	else
		copy(a,a+s,b);
	rep(i,s){
		#ifdef USE_BROKEN_FFT
		int j=i==0?0:i^((1<<topbit(i))-1);
		#else
		int j=(s-i)%s;
		#endif
		C xl=(a[i]+conj(a[j]))*C(0.5,0);
		C xh=(a[i]-conj(a[j]))*C(0,-0.5);
		c[i]=xl*b[i];
		d[i]=xh*b[i];
	}
	inplace_fft(c,s,true);
	inplace_fft(d,s,true);
	rep(i,n){
		r[i]=mint(c[i].real()+.5)
			+mint(c[i].imag()+.5)*(1<<B)
			+mint(d[i].real()+.5)*(1<<B)
			+mint(d[i].imag()+.5)*(1<<B*2);
	}
	return r;
}
