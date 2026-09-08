//i->i-1 (重み a[i])
//i->i (重み b[i])
//i->i+1 (重み c[i])
//という遷移を E 回行った結果を返す
//ARC228E
vc<mint> tridiagonal_advance(vc<mint> ini,vc<mint> a,vc<mint> b,vc<mint> c,int E){
	int n=si(ini); assert(n>=1);
	assert(si(a)==n); assert(a[0]==0);
	assert(si(b)==n);
	assert(si(c)==n); assert(c[n-1]==0);
	rep(i,n-1)assert(c[i]!=0);

	vc<mint> alpha(n);
	alpha[0]=1;
	rep(i,n-1)alpha[i+1]=alpha[i]*c[i];

	auto idx=[&](int l,int r){
		return r-l==1?(l+r):(l+r)/2*2;
	};
	using V=array<F,2>;
	using M=array<array<F,2>,2>;
	auto multMM=[&](M&l,M&r,M&to){
		rep(i,2)rep(j,2)rep(k,2)
			to[i][k]+=l[i][j]*r[j][k];
	};
	auto multVM=[&](V&v,M&m){
		V res;
		rep(i,2)rep(j,2)res[j]+=v[i]*m[i][j];
		return res;
	};
	auto middleVM=[&](V&v,M&m){
		V res;
		rep(i,2)rep(j,2)res[j]+=v[i].middle(m[i][j]);
		return res;
	};
	vc<M> buf(2*n);
	auto freebuf=[&](int i){
		rep(j,2)rep(k,2)buf[i][j][k].freememory();
	};
	Poly<mint> h;
	{
		auto dfs=[&](auto self,int l,int r)->V{
			int i=idx(l,r);
			if(r-l==1){
				buf[i]=M{
					F{-b[l],1},F{l==0?0:-a[l]*c[l-1]},
					F{1},F{}
				};
				V res{
					F{ini[l]/alpha[l]},F{}
				};
				return res;
			}else{
				int m=(l+r)/2;
				int j=idx(l,m),k=idx(m,r);

				auto left=self(self,l,m);
				auto right=self(self,m,r);

				multMM(buf[k],buf[j],buf[i]); //M のマージ、行列の書ける順番が左右逆になる！
				V res=multVM(right,buf[j]);
				freebuf(j);
				rep(x,2)res[x]+=left[x];
				return res;
			}
		};
		auto res=dfs(dfs,0,n);
		h=res[0].getrw();
	}

	/*vc<Poly<mint>> P(n+1);
	P[0]=Poly<mint>{1};
	rep(i,n){
		P[i+1]+=P[i]*Poly<mint>{-b[i],1};
		if(i>=1)P[i+1]-=P[i-1]*a[i]*c[i-1];
	}*/
	/*vc<Poly<mint>> S(n+1);
	S[n]=Poly<mint>{1};
	per(i,n){
		S[i]+=S[i+1]*Poly<mint>{-b[i],1};
		if(i+1<n)S[i]-=S[i+2]*c[i]*a[i+1];
	}*/

	/*Poly<mint> h;
	rep(i,n)h+=P[i]*ini[i]/alpha[i];*/
	assert(si(h)==n);

	auto D=reout(buf[idx(0,n)][0][0].getrw());
	freebuf(idx(0,n));

	int L=max<int>(E-(n-1),0),R=E+n;
	Poly<mint> window=get_inv_window(D,L,R);
	window*=reout(h);
	Poly<mint> w(n);
	rep(i,n)w[i]=window[E+i-L];
	rein(w);

	vc<mint> ans(n);
	{
		auto dfs=[&](auto self,int l,int r,V&v)->void{
			rep(x,2)v[x].middle_shrink(r-l);
			if(r-l==1){
				ans[l]=v[0].getrw()[0]*alpha[l];
			}else{
				int m=(l+r)/2;
				int k=idx(m,r);

				V tmp=middleVM(v,buf[k]);
				freebuf(k);

				self(self,l,m,tmp);
				self(self,m,r,v);
			}
		};
		V v;
		v[0]=w;
		dfs(dfs,0,n,v);
	}
	
	/*vc<mint> ans(n);
	rep(i,n){
		auto tmp=w*S[i+1];
		ans[i]=tmp[n-1]*alpha[i];
	}*/

	return ans;
}