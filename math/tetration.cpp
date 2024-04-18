int totient(int m){
	int res=m;
	for(int i=2;i*i<=m;i++){
		if(m%i==0){
			res=res/i*(i-1);
			while(m%i==0)m/=i;
		}
	}
	if(m>1)
		res=res/m*(m-1);
	return res;
}

int tetration_sub(int a,int b,int m){
	if(b==0)return 1;
	if(a<=1||m==1)return a;
	
	auto mult=[&](int x,int y){
		int z=x*y;
		return z-max(z-m,int(0))/m*m;
	};
	
	int w=tetration_sub(a,b-1,totient(m));
	int res=1;
	
	while(w){
		if(w&1)res=mult(res,a);
		a=mult(a,a);
		w>>=1;
	}
	
	return res;
}

//yosupo tetration mod
//0<=a
//0<=b
//1<=m
//全部 10^9 オーダー
int tetration(int a,int b,int m){
	if(a==0)return (b+1)%2%m;
	return tetration_sub(a,b,m)%m;
}
