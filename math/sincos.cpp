//CF819H
//sin,cos の多項式を exp(iwx) の形に分解して頑張る
struct SC{
	map<int,ld> a[2];
	SC operator-()const{
		SC res;
		rep(k,2)for(auto [w,v]:a[k])res.a[k][w]-=v;
		return res;
	}
	SC&operator*=(ld r){
		rep(k,2)for(auto&[w,v]:a[k])v*=r;
		return *this;
	}
	SC operator*(ld r)const{return SC(*this)*=r;}
	SC&operator/=(ld r){
		rep(k,2)for(auto&[w,v]:a[k])v/=r;
		return *this;
	}
	SC operator/(ld r)const{return SC(*this)/=r;}
	SC&operator+=(const SC&r){
		rep(k,2)for(auto [w,v]:r.a[k])a[k][w]+=v;
		return *this;
	}
	SC&operator-=(const SC&r){
		rep(k,2)for(auto [w,v]:r.a[k])a[k][w]-=v;
		return *this;
	}
	SC&operator*=(const SC&r){
		map<int,ld> res[2];
		rep(i,2)for(auto [w,v]:a[i])
		rep(j,2)for(auto [z,u]:r.a[j])
			res[i^j][w+z]+=v*u*((i&j)?-1:1);
		swap(res,a);
		return *this;
	}
	SC operator+(const SC&r)const{return SC(*this)+=r;}
	SC operator-(const SC&r)const{return SC(*this)-=r;}
	SC operator*(const SC&r)const{return SC(*this)*=r;}
};
SC dif(const SC&x){
	SC y;
	for(auto [w,v]:x.a[0])y.a[1][w]+=v*w;
	for(auto [w,v]:x.a[1])y.a[0][w]-=v*w;
	return y;
}
ld eval(const SC&x,ld p){
	ld res=0;
	for(auto [w,v]:x.a[0])res+=cos(w*p)*v;
	for(auto [w,v]:x.a[1])res-=sin(w*p)*v;
	return res;
}
ld eval_integral(const SC&x,ld p){
	ld res=0;
	for(auto [w,v]:x.a[0])
		if(w==0)res+=p*v;
		else res+=sin(w*p)/w*v;
	for(auto [w,v]:x.a[1])
		if(w!=0)res+=cos(w*p)/w*v;
	return res;
}
ld eval_integral(const SC&x,ld l,ld r){
	return eval_integral(x,r)-eval_integral(x,l);
}
//cos(wx)
SC cf(int w){
	SC res;
	res.a[0][w]+=0.5;
	res.a[0][-w]+=0.5;
	return res;
}
//sin(wx)
SC sf(int w){
	SC res;
	res.a[1][w]-=0.5;
	res.a[1][-w]+=0.5;
	return res;
}
//cos(wx+a)
SC cf(int w,ld a){return cf(w)*cos(a)-sf(w)*sin(a);}
//sin(wx+a)
SC sf(int w,ld a){return cf(w)*sin(a)+sf(w)*cos(a);}
