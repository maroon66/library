//https://codeforces.com/blog/entry/124815
//https://judge.yosupo.jp/submission/270132

//XXII Opencup GP of Siberia 6
//左c列をsweepしする
//aの先頭rank行に要素が入っている感じ
//掃き出しに使った列の番号が返る
vi sweep(vvc<mint>&a,int c=-1){
	if(a.empty())return {};
	if(c==-1)c=a[0].size();
	int h=a.size(),w=a[0].size(),r=0;
	vi res;
	rep(i,c){
		if(r==h)break;
		rng(j,r,h)if(a[j][i].v){
			swap(a[r],a[j]);
			break;
		}
		if(a[r][i].v==0)continue;
		rep(j,h)if(j!=r){
		//rng(j,r+1,h){ //for speed up (sweep に使った列に複数の non-zero があっても良い場合)
			mint z=-a[j][i]/a[r][i];
			rng(k,i,w)
				a[j][k]+=a[r][k]*z;
		}
		res.pb(i);
		r++;
	}
	return res;
}

vvc<mint> inverse(vvc<mint> a){
	int n=si(a);
	rep(i,n){
		assert(si(a[i])==n);
		a[i].resize(2*n);
		a[i][n+i]=1;
	}
	auto tmp=sweep(a,n);
	assert(si(tmp)==n);
	vvc<mint> b(n,vc<mint>(n));
	rep(i,n){
		mint w=a[i][i].inv();
		rep(j,n)b[i][j]=a[i][n+j]*w;
	}
	return b;
}

vc<mint> multvm(const vc<mint>&x,const vvc<mint>&a){
	int n=si(a),m=si(a[0]);
	vc<mint> y(m);
	rep(i,n)rep(j,m)y[j]+=x[i]*a[i][j];
	return y;
}

//empty matrix -> RE
vvc<mint> multmm(const vvc<mint>&a,const vvc<mint>&b){
	int n=si(a),m=si(a[0]),l=si(b[0]);
	assert(si(b)==m);
	vvc<mint> c(n,vc<mint>(l));
	rep(i,n)rep(j,m)rep(k,l)
		c[i][k]+=a[i][j]*b[j][k];
	return c;
}

struct Basis{
	const int n;
	int s; //current number of basis
	vvc<mint> a,b;
	vi c;
	//a reduced form
	//b raw basis
	//c pivot columns
	//row s よりあとはゴミが入っているかもしれないです
	Basis(int nn):n(nn),s(0),a(n+1),b(n),c(n){
		rep(i,n+1)a[i].resize(n+i+1);
		rep(i,n)b[i].resize(n);
	}
	void pivot(int x,int y){
		assert(x<y);
		mint w=a[y][c[x]]/a[x][c[x]];
		rep(i,n+x+1)a[y][i]-=a[x][i]*w;
	}
	//true: added new basis
	bool add(const vc<mint>&raw){
		rep(i,n)a[s][i]=raw[i];
		rep(j,s)a[s][n+j]=0;
		a[s][n+s]=1;
		rep(i,s)pivot(i,s);
		if(s==n)return false;
		c[s]=-1;
		rep(i,n)if(a[s][i])c[s]=i;
		if(c[s]==-1)return false;
		rep(i,n)b[s][i]=raw[i];
		s++;
		return true;
	}
	//add の直後に呼ぶ
	//add(v) に失敗していた場合，今までの基底の線型結合で v が表せるはずなので，それを返してくれる
	vc<mint> annihilator(){
		vc<mint> res(s);
		rep(i,s)res[i]=-a[s][n+i];
		return res;
	}
	void resize(int k){
		assert(inc(0,k,s));
		s=k;
	}
};

void mod_monof(vc<mint>&x,const vc<mint>&f){
	assert(si(f)>=1);
	assert(f.back()==1);
	int k=si(f);
	gnr(i,k-1,si(x)){
		mint w=x[i];
		rep(p,k-1)x[i-(k-1)+p]-=w*f[p];
	}
	x.resize(k-1);
}

vc<mint> mult_mod_monof(const vc<mint>&x,const vc<mint>&y,const vc<mint>&f){
	assert(si(f)>=1);
	assert(f.back()==1);
	int n=si(x),m=si(y);
	vc<mint> res(n+m-1);
	rep(i,n)rep(j,m)res[i+j]+=x[i]*y[j];
	mod_monof(res,f);
	return res;
}

//assume si(f)>=2 (degree(f)>=1)
//f[deg(f)]=1
vc<mint> pow_mod_monof(vc<mint> x,int n,const vc<mint>&f){
	assert(si(f)>=2);
	assert(f.back()==1);
	vc<mint> res{1};
	while(n){
		if(n&1)res=mult_mod_monof(res,x,f);
		x=mult_mod_monof(x,x,f);
		n>>=1;
	}
	return res;
}

struct Frobenius{
	const int n;
	vvc<mint> L,R;
	vvc<mint> fs;
	Frobenius(const vvc<mint>&a):n(si(a)){
		retry:;
		Basis basis(n);
		vvc<mint> ls;
		while(basis.s<n){
			vc<mint> v(n);
			rep(i,n)v[i]=rand_int(mint::mod);
			int s=basis.s,k=0;
			{
				auto curv=v;
				while(1){
					if(!basis.add(curv))break;
					curv=multvm(curv,a);
					k++;
				}
				if(k==0)continue;
			}
			vc<mint> z=basis.annihilator()*(-1);
			vc<mint> p(k+1);
			rep(i,k)p[i]=z[s+i];
			p[k]=1;
			int head=0;
			for(const auto&pre:ls){
				if(si(pre)-1<k)goto retry;
				int tail=head+si(pre)-1;
				gnr(i,head+k,tail){
					mint w=z[i];
					rep(x,n)v[x]+=basis.b[i-k][x]*w;
					rep(j,k)z[i-k+j]-=w*p[j];
					z[i]=0;
				}
				rng(i,head,head+k)if(z[i])goto retry;
				head=tail;
			}
			ls.pb(p);
			basis.resize(s);
			rep(i,k){
				bool tmp=basis.add(v);
				assert(tmp);
				v=multvm(v,a);
			}
		}
		L=inverse(basis.b);
		R=basis.b;
		fs=ls;
	}
	vvc<mint> get_pow(int k){
		VVC(mint,M,n,n);
		int head=0;
		for(const auto&f:fs){
			int tail=head+si(f)-1;
			
			auto g=pow_mod_monof({0,1},k,f);
			rng(i,head,tail){
				rng(j,head,tail)M[i][j]=g[j-head];
				g<<=1;
				mod_monof(g,f);
			}
			
			head=tail;
		}
		return multmm(multmm(L,M),R);
	}
};
