//手元でテストしたら 10^18 まで大丈夫そうだった
ll isqrt(ll n){
	return sqrtl(n);
}
ll icbrt(ll n){
	return cbrtl(n);
}

const ull mask64=-1;

pair<ull,bool> pow_cap(ull a,int n){
	if(a==0)return make_pair(0,0);
	bool over=false;
	auto mult=[&](ull x,ull y){
		if(x<=mask64/y)return x*y;
		else{
			over=true;
			return mask64;
		}
	};
	ull res=1;
	while(n){
		if(n&1)res=mult(res,a);
		n>>=1;
		if(n)a=mult(a,a);
	}
	return make_pair(res,over);
}

//yosupo kth root integer
//0<=a<2^64
ull kth_root_integer(ull a,int k){
	if(a==0)return 0;
	ull z=min((ull)powl(a,(long double)1/k),mask64);
	while(1){
		auto w=pow_cap(z,k);
		if(w.a>a||w.b)z--;
		else break;
	}
	while(z<mask64){
		auto w=pow_cap(z+1,k);
		if(w.a<=a&&!w.b)z++;
		else break;
	}
	return z;
}
