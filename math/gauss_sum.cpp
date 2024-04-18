//fld,cld

//CF530E,ARC055D
//yosupo sum_of_floor_of_linear
//x_i=floor((a*i+b)/c), i=0,1,..n-1
//c>0
//UCUP 2-Prime-43
//n>=0, n,a,c が O(10^9) で b が O(10^18)
int gauss_sum(int n,int a,int b,int c){
	if(n==0)return 0;
	int res=0;
	{
		int p=fld(a,c);
		res+=n*(n-1)/2*p;
		a-=p*c;
	}
	{
		int p=fld(b,c);
		res+=n*p;
		b-=p*c;
	}
	if(a==0)return res;
	int top=(a*(n-1)+b)/c;
	res+=top*n;
	int h=(b+1+c-1)/c;
	if(h<=top)
		res-=gauss_sum(top-h+1,c,c*h-(b+1),a)+top-h+1;
	return res;
}

//Not Verified (9割くらい大丈夫だと思うけど)
//c>=0: Prime New Year Contest 2022 7
//UCUP 2-prime-43
//↑ l,r,a,c が O(10^9) で b が O(10^18)
int gauss_helper(int l,int r,int a,int b,int c){
	int s=1;
	if(r<l){
		s=-s;
		swap(l,r);
	}
	if(c<0){
		a=-a;
		b=-b;
		c=-c;
	}
	b+=l*a;
	return gauss_sum(r-l,a,b,c)*s;
}

template<class t>
t c2(int n){
	int a=n,b=n-1;
	if(a%2)b/=2;
	else a/=2;
	return t(a)*t(b);
}

//AOJ2387 (ull)
//x_i=floor((a*i+b)/c), i=0,1,..n-1
//c>0
//型 t の中で正しく答えを計算する
//modint,ull(mod 2^64) を想定
template<class t>
t gauss_sum(int n,int a,int b,int c){
	if(n==0)return 0;
	t res=0;
	{
		int p=fdiv(a,c);
		res+=c2<t>(n)*t(p);
		a-=p*c;
	}
	{
		int p=fdiv(b,c);
		res+=t(n)*t(p);
		b-=p*c;
	}
	if(a==0)return res;
	int top=(a*(n-1)+b)/c;
	res+=t(top)*t(n);
	int h=(b+1+c-1)/c;
	if(h<=top)
		res-=gauss_sum<t>(top-h+1,c,c*h-(b+1),a)+t(top-h+1);
	return res;
}

//AOJ2387 (ull)
//sum_{0<=i<n} ai+b mod c
template<class t>
t remainder_sum(int n,int a,int b,int c){
	t ans=c2<t>(n)*t(a)+t(n)*t(b);
	ans-=gauss_sum<t>(n,a,b,c)*t(c);
	return ans;
}

//AOJ2387 (ull)
//sum_{l<=i<r} ai+b mod c
template<class t>
t remainder_sum(int l,int r,int a,int b,int c){
	return remainder_sum<t>(r,a,b,c)-remainder_sum<t>(l,a,b,c);
}
