//.v とかつかってるやつは  montgomery ですぐ壊れる，助けてくれ

//VERIFY: yosupo
mint det(vvc<mint> a){
	const int n=a.size();
	mint ans(1);
	rep(i,n){
		rng(j,i+1,n)if(a[j][i].v){
			ans=-ans;
			swap(a[j],a[i]);
			break;
		}
		if(!a[i][i].v)return 0;
		ans*=a[i][i];
		mint z=a[i][i].inv();
		rng(j,i+1,n){
			mint w=-a[j][i]*z;
			rng(k,i,n)
				a[j][k]+=a[i][k]*w;
		}
	}
	return ans;
}

template<class t=ll>
t extgcd(t a,t b,t&x,t&y){
	if(b==0){
		x=1;
		y=0;
		return a;
	}else{
		t g=extgcd(b,a%b,y,x);
		y-=a/b*x;
		return g;
	}
}
//https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod
//N=500 で 1400ms 台　遅すぎ
mint det_arbitrary_mod(vvc<mint> a){
	const int n=si(a);
	mint ans=1;
	rep(i,n){
		rng(j,i+1,n)if(a[j][i]){
			ll s,t;
			ll g=extgcd<ll>(a[i][i].v,a[j][i].v,s,t);
			ll p=-(a[j][i].v/g),q=a[i][i].v/g;
			rng(k,i,n)tie(a[i][k],a[j][k])=mp(a[i][k]*s+a[j][k]*t,a[i][k]*p+a[j][k]*q);
			assert(a[j][i]==0);
		}
		ans*=a[i][i];
	}
	return ans;
}
//https://judge.yosupo.jp/submission/80304
//N=500 で 240ms 程度
//理論上計算量はこっちのほうが悪いが速い
mint det_arbitrary_mod(vvc<mint> a){
	const int n=si(a);
	mint ans=1;
	rep(i,n)rng(j,i+1,n)while(a[j][i]){
		ans=-ans;
		swap(a[i],a[j]);
		mint u=a[j][i].v/a[i][i].v;
		rng(k,i,n)a[j][k]-=a[i][k]*u;
	}
	rep(i,n)ans*=a[i][i];
	return ans;
}

//VERIFY: Yukicoder No.1773
int matrank(vvc<mint>&a){
	if(a.empty())return 0;
	int h=a.size(),w=a[0].size(),r=0;
	rep(i,w){
		if(r==h)break;
		rng(j,r,h)if(a[j][i].v){
			swap(a[r],a[j]);
			break;
		}
		if(a[r][i].v==0)continue;
		rng(j,r+1,h){
			mint z=-a[j][i]/a[r][i];
			rng(k,i,w)
				a[j][k]+=a[r][k]*z;
		}
		r++;
	}
	return r;
}

void transpose(vvc<mint>&a){
	int n=si(a),m=si(a[0]);
	vvc<mint> b(m,vc<mint>(n));
	rep(i,n)rep(j,m)b[j][i]=a[i][j];
	a.swap(b);
}

//N*MでランクRの行列 A を A=BC と分解する
//B:N*R, C: R*M
//O(NMR)
//(JPN Mirror) PKU Contest 2, PTZ Summer 2022 Day 5 D
tuple<int,vvc<mint>,vvc<mint>> decomp(vvc<mint> a){
	int h=si(a),w=si(a[0]);
	bool trans=false;
	if(h>w){
		trans=true;
		swap(h,w);
		transpose(a);
	}
	vvc<mint> b(h,vc<mint>(h));
	rep(i,h)b[i][i]=1;
	int r=0;
	rep(i,w){
		if(r==h)break;
		rng(j,r,h)if(a[j][i].v){
			rep(p,h)swap(b[p][r],b[p][j]);
			swap(a[r],a[j]);
			break;
		}
		if(a[r][i].v==0)continue;
		rng(j,r+1,h){
			mint z=-a[j][i]/a[r][i];
			rep(p,h)b[p][r]-=b[p][j]*z;
			rng(k,i,w)a[j][k]+=a[r][k]*z;
		}
		r++;
	}
	a.resize(r);
	rep(i,h)b[i].resize(r);
	if(trans){
		transpose(a);
		transpose(b);
		swap(a,b);
	}
	return mt(r,b,a);
}

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

//解を一つ見つける
//解空間？kernelでも使っとけ
//返り値の最初は解が存在するかどうか（w=0 のときに empty 判定が壊れるため）
//aが空のときに備えてaの列の個数cを与える
//解が見つからないときは {} を返す
//少し高速化 (CF Goodbye 2021)
pair<bool,vc<mint>> lineareq(vvc<mint> a,int c,vc<mint> b){
	assert(si(a)==si(b));
	int h=a.size(),w=c+1,r=0;
	rep(i,h)a[i].pb(b[i]);
	
	vi idx;
	rep(i,c){
		if(r==h)break;
		rng(j,r,h)if(a[j][i].v){
			swap(a[r],a[j]);
			break;
		}
		if(a[r][i].v==0)continue;
		rng(j,r+1,h){ //for speed up (sweep に使った列に複数の non-zero があっても良い場合)
			mint z=-a[j][i]/a[r][i];
			rng(k,i,w)
				a[j][k]+=a[r][k]*z;
		}
		idx.pb(i);
		r++;
	}
	
	rng(i,r,h)if(a[i][c].v)return mp(false,vc<mint>{});
	vc<mint> res(c);
	per(i,si(idx)){
		res[idx[i]]=a[i][c]/a[i][idx[i]];
		per(j,i)a[j][c]-=a[j][idx[i]]*res[idx[i]];
	}
	return mp(true,res);
}

//yosupo system of linear equations
//ker(a)を返す
//aが空のときに備えてaの列の個数wを与える
vvc<mint> kernel(const vvc<mint>&a,int w){
	int h=a.size();
	vvc<mint> b(w,vc<mint>(h+w));
	rep(i,h)rep(j,w)b[j][i]=a[i][j];
	rep(i,w)b[i][h+i]=1;
	int r=sweep(b,h).size();
	vvc<mint> res;
	rng(i,r,w)res.eb(b[i].bg+h,b[i].ed);
	return res;
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

vc<mint> multmv(const vvc<mint>&a,const vc<mint>&x){
	int n=si(a),m=si(x);
	vc<mint> y(n);
	rep(i,n)rep(j,m)y[i]+=a[i][j]*x[j];
	return y;
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

//Petrozavodsk 2020w Day9 K
void hessenberg_form(vvc<mint>&a){
	int n=si(a);
	rng(i,1,n){
		rng(j,i+1,n)if(a[j][i-1]){
			rep(k,n)swap(a[k][i],a[k][j]);
			rep(k,n)swap(a[i][k],a[j][k]);
			break;
		}
		if(a[i][i-1]==0)continue;
		rng(j,i+1,n){
			mint w=a[j][i-1]/a[i][i-1];
			rep(k,n)a[k][i]+=a[k][j]*w;
			rep(k,n)a[j][k]-=a[i][k]*w;
		}
	}
}

//多分 det(a-xI) を求めている
//Petrozavodsk 2020w Day9 K
vc<mint> characteristic_polynomial(vvc<mint> a){
	int n=si(a);
	hessenberg_form(a);
	vvc<mint> f(n+1);
	f[0]={1};
	rep(i,n){
		f[i+1].resize(i+2);
		rep(j,i+1)f[i+1][j+1]=-f[i][j];
		mint w=1;
		per(j,i+1){
			mint z=w*a[j][i];
			rep(k,j+1)f[i+1][k]+=f[j][k]*z;
			if(j)w*=-a[j][j-1];
		}
	}
	return f[n];
}

//Prime New Year Contest 2022 11 (modulo 2)
//stress test (998244353)
//det(ax+b)
vc<mint> det_ax_b(vvc<mint> a,vvc<mint> b){
	int n=si(a);assert(si(b)==n);
	mint z=1;
	int off=0;
	rep(i,n){
		while(1){
			rng(j,i+1,n)if(a[i][j]){
				z=-z;
				rep(k,n){
					swap(a[k][i],a[k][j]);
					swap(b[k][i],b[k][j]);
				}
				break;
			}
			if(a[i][i]==0){
				off++;
				if(off>n)return vc<mint>(n+1);
				rep(j,n){
					assert(a[i][j]==0);
					swap(a[i][j],b[i][j]);
				}
				rep(j,i){
					mint w=-a[i][j];
					rep(k,n)b[i][k]+=b[j][k]*w;
					a[i][j]=0;
				}
			}else{
				{
					z*=a[i][i];
					mint w=a[i][i].inv();
					rep(k,n){
						a[i][k]*=w;
						b[i][k]*=w;
					}
				}
				rng(j,i+1,n){
					mint w=-a[j][i];
					rep(k,n){
						a[j][k]+=a[i][k]*w;
						b[j][k]+=b[i][k]*w;
					}
				}
				rng(j,i+1,n){
					mint w=-a[i][j];
					rep(k,n){
						a[k][j]+=a[k][i]*w;
						b[k][j]+=b[k][i]*w;
					}
				}
				break;
			}
		}
	}
	rep(i,n)rep(j,n)assert(a[i][j]==(i==j?1:0));
	auto res=characteristic_polynomial(b);
	for(int i=1;i<=n;i+=2)res[i]*=-1;
	rotate(res.bg,res.bg+off,res.ed);
	for(auto&v:res)v*=z;
	return res;
}

//正定値行列をいい感じに吐き出して固有値を得る
//PAP^T = D 
//Multi-Uni Contest 2020 Day1 D
//ここではベクトル B に対し BP^T も求めている
	rep(step,n){
		bool fd=false;
		rng(i,step,n){
			if(a[i][i]){
				if(step<i){
					rng(j,step,n){
						swap(a[step][j],a[i][j]);
					}
					rng(j,step,n){
						swap(a[j][step],a[j][i]);
					}
					swap(b[i],b[step]);
				}
				fd=true;
				break;
			}
		}
		assert(fd);
		mint w=a[step][step].inv();
		rng(j,step+1,n){
			mint z=-w*a[j][step];
			rng(k,step,n){
				a[j][k]+=a[step][k]*z;
			}
			rng(k,step,n){
				a[k][j]+=a[k][step]*z;
			}
			b[j]+=b[step]*z;
		}
	}
