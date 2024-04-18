//VERIFY: yosupo judge
//O(2^N N^2)
//N=20で1200msくらい
template<class t,int L=20>
vc<t> subset_convolution(const vc<t>&a,const vc<t>&b){
	int s=si(a);
	assert(si(b)==s);
	int n=__lg(s);
	assert(s==(1<<n));
	assert(n<=L);
	vc<array<t,L>> x(s),y(s);
	rep(i,s-1)x[i][popcount(i)]=a[i];
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]+=x[i][j];
	rep(i,s-1)y[i][popcount(i)]=b[i];
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)y[i|1<<k][j]+=y[i][j];
	rep(i,s)per(j,n){
		rng(k,1,n-j)x[i][j+k]+=x[i][j]*y[i][k];
		x[i][j]*=y[i][0];
	}
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]-=x[i][j];
	vc<t> res(s);
	rep(i,s-1)res[i]=x[i][popcount(i)];
	rep(i,s)res[s-1]+=a[i]*b[s-1-i];
	return res;
}

//ARC105F
template<class t,int L=20>
vc<t> subset_square(const vc<t>&a){
	int s=si(a);
	int n=__lg(s);
	assert(s==(1<<n));
	assert(n<=L);
	vc<array<t,L>> x(s);
	rep(i,s-1)x[i][popcount(i)]=a[i];
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]+=x[i][j];
	rep(i,s){
		auto y=x[i];
		per(j,n){
			rng(k,1,n-j)x[i][j+k]+=x[i][j]*y[k];
			x[i][j]*=y[0];
		}
	}
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]-=x[i][j];
	vc<t> res(s);
	rep(i,s-1)res[i]=x[i][popcount(i)];
	rep(i,s)res[s-1]+=a[i]*a[s-1-i];
	return res;
}

//initfact!
//ARC105F
template<class t,int L=20>
vc<t> subset_log(const vc<t>&a){
	int s=si(a);
	int n=__lg(s);
	assert(s==(1<<n));
	assert(n<=L);
	assert(a[0]==1);
	vc<array<t,L>> x(s);
	rng(i,1,s)x[i][popcount(i)-1]=a[i];
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]+=x[i][j];
	rep(i,s){
		auto y=x[i];
		rep(j,n){
			x[i][j]*=j+1;
			rep(k,j)x[i][j]-=x[i][k]*y[j-k-1];
		}
		rep(j,n)x[i][j]*=invs[j+1];
	}
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]-=x[i][j];
	vc<t> res(s);
	rng(i,1,s)res[i]=x[i][popcount(i)-1];
	return res;
}

//initfact!
//Codechef Decmber Long 2020 MODPARRS
template<class t,int L=20>
vc<t> subset_exp(const vc<t>&a){
	int s=si(a);
	int n=__lg(s);
	assert(s==(1<<n));
	assert(n<=L);
	assert(a[0]==0);
	vc<array<t,L>> x(s);
	rng(i,1,s)x[i][popcount(i)-1]=a[i];
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]+=x[i][j];
	rep(i,s){
		//p: result
		array<t,L> y{};//inv of p without offset
		//p[0]=1
		y[0]=1;
		//p[i+1]=x[i][i]
		//p[1]=x[i][0]
		rng(j,1,n){
			//x[i][j-1]=p[j] is computed
			//compute y[j]
			rep(k,j)y[j]-=y[k]*x[i][j-k-1];
			//compute x[i][j]=p[j+1]
			t val=0;
			rep(k,j)val+=x[i][k]*(k+1)*y[j-k];
			x[i][j]-=val*invs[j+1];
		}
	}
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]-=x[i][j];
	vc<t> res(s);
	res[0]=1;
	rng(i,1,s)res[i]=x[i][popcount(i)-1];
	return res;
}

//initfact!
//N=21 で 1400ms on Atcoder (入力のボトルネック等を除くと本当はもう少し速い)
template<class t,int L=20>
vc<t> subset_sqrt(const vc<t>&a){
	int s=si(a);
	int n=__lg(s);
	assert(s==(1<<n));
	assert(n<=L);
	assert(a[0]==1);
	vc<array<t,L>> x(s);
	rng(i,1,s)x[i][popcount(i)-1]=a[i];
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]+=x[i][j];
	rep(i,s){
		rep(j,n){
			if(j%2)x[i][j]-=sq(x[i][(j-1)/2]);
			x[i][j]*=invs[2];
			rep(k,j/2)x[i][j]-=x[i][k]*x[i][j-k-1];
		}
	}
	rep(k,n)rep(i,s)if(((i>>k)&1)==0)rep(j,n)x[i|1<<k][j]-=x[i][j];
	vc<t> res(s);
	res[0]=1;
	rng(i,1,s)res[i]=x[i][popcount(i)-1];
	return res;
}
