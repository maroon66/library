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

//a: n by k, val in [0,s)
//s: pow of 2
//res: s by 2^k
//各 0<=v<s に対し
//parity(a[i][j]&v) for 0<=j<k を考えると長さ k の 01 列になる
//これの頻度分布を求める
//CF Global2 H
vvc<int> xor_transform_list(const vvc<int>&a,int s){
	int n=si(a),k=si(a[0]);
	VVC(int,res,s,1<<k);
	rep(z,1<<k){
		vi buf(s);
		rep(i,n){
			int val=0;
			rep(j,k)if(z&1<<j)
				val^=a[i][j];
			buf[val]++;
		}
		inplace_xor_transform(buf);
		rep(bit,s)res[bit][z]=buf[bit];
	}
	rep(bit,s){
		inplace_xor_transform(res[bit]);
		rep(z,1<<k)res[bit][z]/=(1<<k);
	}
	return res;
}

//\prod (x*a+y*b)
//x,y: [0,n) の値
//a,b: mint の係数
//x,y を選ぶ方法が a,b 通りある
//これを FWHT したものすべての積を求める
//これをさらに xor_transform して n で割れば，全 xor の頻度分布が出てくると思う
//https://yukicoder.me/problems/no/2990
vc<mint> prod_fwht_axby(int n,const vc<tuple<int,mint,int,mint>>&buf){
	assert(ispow2(n));
	using A=array<mint,2>;
	vc<A> f(n,A{1,1});
	for(auto [x,a,y,b]:buf){
		f[x][0]*=1;
		f[x][1]*=-1;
		f[x^y][0]*=a+b;
		f[x^y][1]*=a-b;
	}
	for(int b=1;b<=n/2;b*=2){
		for(int i=0;i<n;i+=b*2){
			for(int j=i;j<i+b;j++){
				auto u=f[j],v=f[j+b];
				rep(k,2)f[j][k]=u[k]*v[k];
				rep(k,2)f[j+b][k]=u[k]*v[1-k];
			}
		}
	}
	vc<mint> res(n);
	rep(i,n)res[i]=f[i][0];
	return res;
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
