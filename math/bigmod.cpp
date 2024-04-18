//普通に四則演算しない！！

const ull mod=9000000000000000041LL;

ull add(ull a,ull b){
	a+=b;
	return a<mod?a:a-mod;
}

ull mult(ull a,ull b){
	return (unsigned __int128)a*b%mod;
}

void chmult(ull&a,ull b){
	a=mult(a,b);
}

ull mpow(ull a,ull n){
	ull res=1;
	while(n){
		if(n&1)chmult(res,a);
		chmult(a,a);
		n>>=1;
	}
	return res;
}

ull minv(ull a){
	return mpow(a,mod-2);
}
