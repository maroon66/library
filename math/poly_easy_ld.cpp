using ld=__float128;

//mint と挙動違うからね，注意

void mod_f(vc<ld>&x,const vc<ld>&f){
	int n=si(x),m=si(f);
	gnr(i,m,n)rep(j,m)
		x[i-m+j]+=x[i]*f[j];
	x.resize(m);
}

vc<ld> mult_mod_f(const vc<ld>&x,const vc<ld>&y,const vc<ld>&f){
	int n=si(x),m=si(y);
	vc<ld> z(n+m-1);
	rep(i,n)rep(j,m)z[i+j]+=x[i]*y[j];
	mod_f(z,f);
	return z;
}

vc<ld> pow_mod_f(vc<ld> x,int n,const vc<ld>&f){
	vc<ld> res(si(f));
	res[0]=1;
	while(n){
		if(n&1)res=mult_mod_f(res,x,f);
		x=mult_mod_f(x,x,f);
		n>>=1;
	}
	return res;
}

//CF658E
ld eval(const vc<ld>&f,ld x){
	ld a=0;
	per(i,si(f)){
		a*=x;
		a+=f[i];
	}
	return a;
}

//CF658E
ld err(ld a,ld b){
	return abs(a-b)/max({ld(1),abs(a),abs(b)});
}

//CF658E
//x の値が妙に近かったりすると，大変なことに・・・
vc<ld> interpolate_f(vc<ld>x,vc<ld>y){
	int n=si(x),raw=n;
	assert(si(y)==n);
	{
		assert(is_sorted(all(x)));
		int m=0;
		rep(i,n){
			if(m&&err(x[m-1],x[i])<=1e-6)continue;
			x[m]=x[i];
			y[m]=y[i];
			m++;
		}
		n=m;
		x.resize(m);
		y.resize(m);
	}
	vc<ld> res(n);
	vc<ld> cur(n+1);
	cur[0]=1;
	rep(i,n){
		per(j,i+1){
			cur[j+1]+=cur[j];
			cur[j]*=-x[i];
		}
	}
	vc<ld> tmp(n);
	rep(i,n){
		ld pre=0;
		per(j,n)pre=tmp[j]=cur[j+1]+pre*x[i];
		ld w=y[i]/eval(tmp,x[i]);
		rep(j,n)res[j]+=tmp[j]*w;
	}
	//dmp2(x,y,res);
	res.resize(raw);
	return res;
}
