//adj(A)=det(A)A^{-1}
//i 行目と j 列目を除いたあとの行列式，を -1^(i+j) 倍して更に転置
//-1 倍や転置をやめたい場合は，コメントアウトを外す．
//stress-tested
vvc<mint> adjugate(vvc<mint> a){
	int n=si(a);
	vvc<mint> res(n,vc<mint>(n));
	vvc<mint> l(n,vc<mint>(n));
	rep(i,n)l[i][i]=1;
	vvc<mint> r(n,vc<mint>(n));
	rep(i,n)r[i][i]=1;
	mint coef=1;
	
	vi col;
	rep(i,n){
		int row=si(col);
		rng(j,row+1,n)if(a[j][i].v){
			coef*=-1;
			swap(a[row],a[j]);
			swap(l[row],l[j]);
			break;
		}
		if(a[row][i].v==0)continue;
		coef*=a[row][i];
		rng(j,row+1,n){
			mint z=-a[j][i]/a[row][i];
			rng(k,i,n)a[j][k]+=a[row][k]*z;
			rep(k,n)l[j][k]+=l[row][k]*z;
		}
		{
			rng(j,i+1,n){
				mint z=-a[row][j]/a[row][i];
				a[row][j]=0;
				rep(k,n)r[k][j]+=r[k][i]*z;
			}
		}
		col.pb(i);
	}
	
	if(si(col)<=n-2){
		return res;
	}else if(si(col)==n-1){
		int u=n-1;
		rep(i,n-1)if(col[i]!=i){
			u=i;
			break;
		}
		if((n-1-u)%2)coef*=-1;
		rep(i,n)rep(j,n)res[i][j]=r[i][u]*l[n-1][j];
	}else{
		rep(i,n){
			mint z=a[i][i].inv();
			rep(j,n)l[i][j]*=z;
		}
		res=multmm(r,l);
	}
	rep(i,n)rep(j,n)res[i][j]*=coef;
	//rep(i,n)rep(j,i)swap(res[i][j],res[j][i]);
	//rep(i,n)rep(j,n)if((i+j)%2)res[i][j]*=-1;
	return res;
}
