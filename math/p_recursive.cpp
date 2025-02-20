//Xmas2023 H
//YukiCoder No93

//res: k by d table
//for all n sum_{0<=i<k} f_i(n)*a_{n+i} = 0
//f_i(n)=sum_{0<=j<d} res[i][j]*(n+i)^j
vvc<mint> p_recursive(const vc<mint>&a,int d){
	int n=a.size();
	int k=(n+2)/(d+1);
	int m=k*d;
	vvc<mint> x(m,vc<mint>(m+m-1));
	rep(p,m-1)rep(i,k){
		mint w=a[p+i];
		rep(j,d){
			x[i*d+j][p]=w;
			w*=(p+i);
		}
	}
	rep(i,m)
		x[i][m-1+i]=1;
	vc<mint> v;
	rep(i,m){
		int j=0;
		while(j<m-1&&x[i][j].v==0)
			j++;
		if(j==m-1){
			rep(h,i+1)
				v.pb(x[i][m-1+h]);
			break;
		}else{
			mint z=x[i][j].inv();
			rng(h,i+1,m){
				mint w=x[h][j]*z;
				rep(l,m+m-1)
					x[h][l]-=x[i][l]*w;
			}
		}
	}
	assert(v.size());
	vvc<mint> res=vvc<mint>(int(v.size()+d-1)/d,vc<mint>(d));
	rep(i,v.size())
		res[i/d][i%d]=v[i];
	return res;
}

vc<mint> extend_sequence(const vc<mint>&a,const vvc<mint>& f,int n){
	int k=f.size(),d=f[0].size();
	assert(int(a.size())>=k-1);
	vc<mint> res(n),w(k);
	rep(i,n){
		if(i<si(a))
			res[i]=a[i];
		else{
			rep(j,k){
				w[j]=0;
				mint z=1;
				rep(h,d){
					w[j]+=f[j][h]*z;
					z*=i-k+1+j;
				}
			}
			assert(w[k-1].v);
			mint s=0;
			rep(j,k-1)
				s+=w[j]*res[i-k+1+j];
			res[i]=-s/w[k-1];
		}
	}
	return res;
}

vc<mint> check_and_extend_sequence(vc<mint> a,int d,int n){
	int s=si(a);
	mint v=a.back();
	a.pop_back();
	auto f=p_recursive(a,d);
	vc<mint> res=extend_sequence(a,f,max(s,n));
	if(v!=res[s-1]){
		cerr<<"Fail to extend!"<<endl;
		return {};
	}
	cerr<<"Length: "<<si(f)<<endl;
	cerr<<f<<endl;
	res.resize(n);
	return res;
}
