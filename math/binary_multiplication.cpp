//UCUP 3-34-F
vi getbinary(){
	STR(str);
	int n=si(str);
	vi res(n);
	rep(i,n)res[i]=str[n-1-i]-'0';
	return res;
}

void simplify(vi&a){
	int carry=0;
	rep(i,si(a)){
		int v=a[i]+carry;
		a[i]=v%2;
		carry=v/2;
	}
	while(carry){
		a.pb(carry%2);
		carry/=2;
	}
	while(si(a)>1&&a.back()==0)a.pop_back();
}

void add(vi&a,const vi&b){
	a+=b;
	simplify(a);
}

vi simple_convolution(const vi&a,const vi&b){
	vc<mint> x(si(a));
	rep(i,si(a))x[i]=a[i];
	vc<mint> y(si(b));
	rep(i,si(b))y[i]=b[i];
	auto z=multiply(x,y);
	vi c(si(z));
	rep(i,si(z))c[i]=z[i].v;
	simplify(c);
	return c;
}

//i<j で L[i]*R[j] -> res[i+j]
vi ij_conv(const vi&L,const vi&R){
	assert(si(L)==si(R));
	int n=si(L);
	vi res(2*n-1);
	auto dfs=[&](auto self,int l,int r)->void{
		if(r-l==1)return;
		int mid=(l+r)/2;
		{
			vc<mint> x(mid-l),y(r-mid);
			rng(i,l,mid)x[i-l]=L[i];
			rng(i,mid,r)y[i-mid]=R[i];
			auto z=multiply(x,y);
			rep(i,si(z))res[l+mid+i]+=z[i].v;
		}
		self(self,l,mid);
		self(self,mid,r);
	};
	dfs(dfs,0,n);
	simplify(res);
	return res;
}
