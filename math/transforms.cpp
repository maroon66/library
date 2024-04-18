//the size of f must be a power of 2
template<class T>
void inplace_xor_transform(vector<T>&f){
	int n=f.size();
	for(int b=1;b<=n/2;b*=2){
		for(int i=0;i<n;i+=b*2){
			for(int j=i;j<i+b;j++){
				tie(f[j],f[j+b])=make_tuple(f[j]+f[j+b],f[j]-f[j+b]);
			}
		}
	}
}

//JSC2023 D
template<class t>
vc<t> xor_convolution(vc<t> a,vc<t> b){
	assert(si(a)==si(b));
	inplace_xor_transform(a);
	inplace_xor_transform(b);
	rep(i,si(a))a[i]*=b[i];
	inplace_xor_transform(a);
	//division
	//rep(i,si(a))a[i]/=si(a);
	return a;
}

void inplace_and_transform(vector<mint>&f){
	int n=f.size();
	for(int b=1;b<=n/2;b*=2){
		for(int i=0;i<n;i+=b*2){
			for(int j=i;j<i+b;j++){
				tie(f[j],f[j+b])=make_tuple(f[j+b],f[j]+f[j+b]);
			}
		}
	}
}

void inplace_and_transform_inverse(vector<mint>&f){
	int n=f.size();
	for(int b=1;b<=n/2;b*=2){
		for(int i=0;i<n;i+=b*2){
			for(int j=i;j<i+b;j++){
				tie(f[j],f[j+b])=make_tuple(-f[j]+f[j+b],f[j]);
			}
		}
	}
}


//or?
//|1 0| |1  0|
//|1 1| |-1 1|


//CodeChef 2022 February Lunchtime D
//FHC2023-2-D

template<class t>
void divisor_transform(vc<t>&a){
	int n=si(a)-1;
	for(auto p:ps){
		if(p>n)break;
		int mx=n/p;
		gnr(i,1,mx+1)a[i]+=a[i*p];
	}
}

template<class t>
void multiple_transform(vc<t>&a){
	int n=si(a)-1;
	for(auto p:ps){
		if(p>n)break;
		int mx=n/p;
		rng(i,1,mx+1)a[i]-=a[i*p];
	}
}

vi gcd_convolution(int n,vi a,vi b){
	divisor_transform(n,a);
	divisor_transform(n,b);
	rng(i,1,n+1)a[i]*=b[i];
	multiple_transform(n,a);
	return a;
}
