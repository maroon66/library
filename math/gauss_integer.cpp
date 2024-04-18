//complex に突っ込んでいるため定数倍が遅いがち

modinfo base2{1,0};
using gint=modular<base2>;
using G=complex<gint>;
G gpow(G a,int n){
	G res(1);
	while(n){
		if(n&1)res*=a;
		a*=a;
		n>>=1;
	}
	return res;
}

G gauss_factor(int p){
	assert(p%4==1);
	//assert(isprime(p));
	for(int i=1;i*i<=p;i++){
		int j=sqrtl(p-i*i);
		if(i*i+j*j==p){
			return G(i,j);
		}
	}
	assert(false);
}
