//Poly というのは常にサイズ 1 以上であることにしよう
//low のあたりをかならずサイズ s のものを返すようにいじった
//その影響で何かが起きているかも知れないし，起きていないかも知れない
template<class mint>
struct Poly:public vc<mint>{
	/*Poly(){}
	//Poly(const vc<mint>&val):vc<mint>(val){}
	//Poly(vc<mint>&&val):vc<mint>(val){}
	explicit Poly(int n):vc<mint>(n){}
	explicit Poly(int n,mint v):vc<mint>(n,v){}
	//Poly(initializer_list<mint>init):vc<mint>(all(init)){}
	template<class...Args>
	Poly(Args&&...args):vc<mint>(forward<Args>(args)...){}*/
	Poly(const vc<mint>&val):vc<mint>(val){}
	Poly(vc<mint>&&val):vc<mint>(val){}
	using vc<mint>::vector;
	using vc<mint>::operator=;
	//Poly(const Poly<mint>&rhs):vc<mint>((vc<mint>)rhs){}
	//Poly(Poly<mint>&&rhs):vc<mint>(forward<vc<mint>>(rhs)){}
	int size()const{
		return vc<mint>::size();
	}
	void ups(int s){
		if(size()<s)this->resize(s,0);
	}
	Poly low(int s)const{
		assert(s);
		Poly res(s);
		rep(i,min(s,size()))res[i]=(*this)[i];
		return res;
	}
	Poly rev()const{
		auto r=*this;
		reverse(all(r));
		return r;
	}
	Poly operator>>(int x)const{
		assert(x<size());
		Poly res(size()-x);
		rep(i,size()-x)res[i]=(*this)[i+x];
		return res;
	}
	Poly operator<<(int x)const{
		Poly res(size()+x);
		rep(i,size())res[i+x]=(*this)[i];
		return res;
	}
	mint freq(int i)const{
		return i<size()?(*this)[i]:0;
	}
	Poly operator-()const{
		Poly res=*this;
		for(auto&v:res)v=-v;
		return res;
	}
	Poly& operator+=(const Poly&r){
		ups(r.size());
		rep(i,r.size())
			(*this)[i]+=r[i];
		return *this;
	}
	Poly& operator+=(const vc<mint>&r){
		return operator+=((const Poly&)r);
	}
	template<class T>
	Poly& operator+=(T t){
		(*this)[0]+=t;
		return *this;
	}
	Poly& operator-=(const Poly&r){
		ups(r.size());
		rep(i,r.size())
			(*this)[i]-=r[i];
		return *this;
	}
	Poly& operator-=(const vc<mint>&r){
		return operator-=((const Poly&)r);
	}
	template<class T>
	Poly& operator-=(T t){
		(*this)[0]-=t;
		return *this;
	}
	template<class T>
	Poly& operator*=(T t){
		for(auto&v:*this)
			v*=t;
		return *this;
	}
	Poly& operator*=(const Poly&r){
		*this=multiply(*this,r);
		return *this;
	}
	Poly& operator*=(const vc<mint>&r){
		return operator*=((const Poly&)r);
	}
	Poly square()const{
		return multiply(*this,*this,true);
	}
	#ifndef USE_GOOD_MOD
	Poly inv(int s)const{
		Poly r{mint(1)/(*this)[0]};
		for(int n=1;n<s;n*=2)
			r=r*2-(r.square()*low(2*n)).low(2*n);
		r.resize(s);
		return r;
	}
	#else
	//source: Section 4 of "Removing redundancy from high-precision Newton iteration"
	// 5/3
	Poly inv(int s)const{
		Poly r(s);
		r[0]=mint(1)/(*this)[0];
		for(int n=1;n<s;n*=2){
			vc<mint> f=low(2*n);
			f.resize(2*n);
			inplace_fmt(f,false);
			vc<mint> g=r.low(2*n);
			g.resize(2*n);
			inplace_fmt(g,false);
			rep(i,2*n)f[i]*=g[i];
			inplace_fmt(f,true);
			rep(i,n)f[i]=0;
			inplace_fmt(f,false);
			rep(i,2*n)f[i]*=g[i];
			inplace_fmt(f,true);
			rng(i,n,min(2*n,s))r[i]=-f[i];
		}
		return r;
	}
	#endif
	template<class T>
	Poly& operator/=(T t){
		return *this*=mint(1)/mint(t);
	}
	Poly quotient(const Poly&r,const Poly&rri)const{
		int m=r.size();
		assert(r[m-1].v);
		int n=size();
		int s=n-m+1;
		if(s<=0) return {0};
		return (rev().low(s)*rri.low(s)).low(s).rev();
	}
	Poly& operator/=(const Poly&r){
		return *this=quotient(r,r.rev().inv(max(size()-r.size(),int(0))+1));
	}
	Poly& operator%=(const Poly&r){
		*this-=*this/r*r;
		return *this=low(r.size()-1);
	}
	Poly operator+(const Poly&r)const{return Poly(*this)+=r;}
	template<class T>
	Poly operator+(T t)const{return Poly(*this)+=t;}
	template<class T>
	friend Poly operator+(T t,Poly r){
		r[0]+=t;
		return r;
	}
	Poly operator-(const Poly&r)const{return Poly(*this)-=r;}
	template<class T>
	Poly operator-(T t)const{return Poly(*this)-=t;}
	template<class T>
	friend Poly operator-(T t,Poly r){
		for(auto&v:r)v=-v;
		r[0]+=t;
		return r;
	}
	template<class T>
	Poly operator*(T t)const{return Poly(*this)*=t;}
	Poly operator*(const Poly&r)const{return Poly(*this)*=r;}
	template<class T>
	Poly operator/(T t)const{return Poly(*this)/=t;}
	Poly operator/(const Poly&r)const{return Poly(*this)/=r;}
	Poly operator%(const Poly&r)const{return Poly(*this)%=r;}
	Poly dif()const{
		assert(size());
		if(size()==1){
			return {0};
		}else{
			Poly r(size()-1);
			rep(i,r.size())
				r[i]=(*this)[i+1]*(i+1);
			return r;
		}
	}
	Poly inte(const mint invs[])const{
		Poly r(size()+1,0);
		rep(i,size())
			r[i+1]=(*this)[i]*invs[i+1];
		return r;
	}
	//VERIFY: yosupo
	//opencupXIII GP of Peterhof H
	Poly log(int s,const mint invs[])const{
		assert((*this)[0]==1);
		if(s==1)return {0};
		return (low(s).dif()*inv(s-1)).low(s-1).inte(invs);
	}
	//Petrozavodsk 2019w mintay1 G
	//yosupo judge
	//UOJ Round23 C
	Poly exp(int s,const mint invs[])const{
		assert((*this)[0]==mint(0));
		Poly f{1},g{1};
		for(int n=1;;n*=2){
			if(n>=s)break;
			g=g*2-(g.square()*f).low(n);
			//if(n>=s)break;
			Poly q=low(n).dif();
			q=q+g*(f.dif()-f*q).low(2*n-1);
			f=f+(f*(low(2*n)-q.inte(invs))).low(2*n);
		}
		return f.low(s);
	}
	//exp(x),exp(-x) のペアを返す
	//UOJ Round23 C
	pair<Poly,Poly> exp2(int s,const mint invs[])const{
		assert((*this)[0]==mint(0));
		Poly f{1},g{1};
		for(int n=1;;n*=2){
			//if(n>=s)break;
			g=g*2-(g.square()*f).low(n);
			if(n>=s)break;
			Poly q=low(n).dif();
			q=q+g*(f.dif()-f*q).low(2*n-1);
			f=f+(f*(low(2*n)-q.inte(invs))).low(2*n);
		}
		return make_pair(f.low(s),g.low(s));
	}
	#ifndef USE_GOOD_MOD
	//CF250 E
	Poly sqrt(int s)const{
		assert((*this)[0]==1);
		static const mint half=mint(1)/mint(2);
		Poly r{1};
		for(int n=1;n<s;n*=2)
			r=(r+(r.inv(n*2)*low(n*2)).low(n*2))*half;
		return r.low(s);
	}
	#else
	//11/6
	//VERIFY: yosupo
	Poly sqrt(int s)const{
		assert((*this)[0]==1);
		static const mint half=mint(1)/mint(2);
		vc<mint> f{1},g{1},z{1};
		for(int n=1;n<s;n*=2){
			rep(i,n)z[i]*=z[i];
			inplace_fmt(z,true);
			
			vc<mint> delta(2*n);
			rep(i,n)delta[n+i]=z[i]-freq(i)-freq(n+i);
			inplace_fmt(delta,false);
			
			vc<mint> gbuf(2*n);
			rep(i,n)gbuf[i]=g[i];
			inplace_fmt(gbuf,false);
			
			rep(i,2*n)delta[i]*=gbuf[i];
			inplace_fmt(delta,true);
			f.resize(2*n);
			rng(i,n,2*n)f[i]=-half*delta[i];
			
			if(2*n>=s)break;
			
			z=f;
			inplace_fmt(z,false);
			
			vc<mint> eps=gbuf;
			rep(i,2*n)eps[i]*=z[i];
			inplace_fmt(eps,true);
			
			rep(i,n)eps[i]=0;
			inplace_fmt(eps,false);
			
			rep(i,2*n)eps[i]*=gbuf[i];
			inplace_fmt(eps,true);
			g.resize(2*n);
			rng(i,n,2*n)g[i]=-eps[i];
		}
		f.resize(s);
		return f;
	}
	#endif
	pair<Poly,Poly> divide(const Poly&r,const Poly&rri)const{
		Poly a=quotient(r,rri);
		Poly b=*this-a*r;
		return make_pair(a,b.low(r.size()-1));
	}
	//Yukicoder No.215
	Poly pow_mod(int n,const Poly&r)const{
		Poly rri=r.rev().inv(r.size());
		Poly cur{1},x=*this%r;
		while(n){
			if(n%2)
				cur=(cur*x).divide(r,rri).b;
			x=(x*x).divide(r,rri).b;
			n/=2;
		}
		return cur;
	}
	int lowzero()const{
		rep(i,size())if((*this)[i]!=0)return i;
		return size();
	}
	//VERIFY: yosupo
	//UOJ Round23 C (z=0,p<0)
	//Multiuni2023-4-B
	Poly pow(int s,int p,const mint invs[])const{
		assert(s>0);
		if(p==0){
			Poly res(s,0);
			res[0]=1;
			return res;
		}
		int n=size(),z=0;
		for(;z<n&&(*this)[z]==0;z++);
		assert(z==0||p>0);
		if(z*p>=s)return Poly(s,0);
		mint c=(*this)[z],cinv=c.inv();
		mint d=c.pow(p);
		int t=s-z*p;
		Poly x(t);
		rng(i,z,min(z+t,n))x[i-z]=(*this)[i]*cinv;
		x=x.log(t,invs);
		rep(i,t)x[i]*=p;
		x=x.exp(t,invs);
		rep(i,t)x[i]*=d;
		Poly y(s);
		rep(i,t)y[z*p+i]=x[i];
		return y;
	}
	//verified in compositional inverse
	Poly pow_const1(mint p,const mint invs[])const{
		assert((*this)[0]==1);
		Poly x=log(size(),invs);
		rep(i,size())x[i]*=p;
		return x.exp(size(),invs);
	}
	mint eval(mint x)const{
		mint r=0,w=1;
		for(auto v:*this){
			r+=w*v;
			w*=x;
		}
		return r;
	}
};
template<class T>
void print_single(const Poly<T>&v,int suc=1){
	rep(i,v.size())
		print_single(v[i],i==int(v.size())-1?3:2);
	if(suc==1){
		if(dbg)cout<<endl;
		else cout<<"\n";
	}
	if(suc==2)
		cout<<" ";
}

//CF641 F2
//f*x^(-a)
template<class mint>
struct Laurent{
	Poly<mint> f;
	int a;
	Laurent(const Poly<mint>&num,const Poly<mint>&den,int s){
		a=den.lowzero();
		assert(a<si(den));
		f=(num*(den>>a).inv(s)).low(s);
	}
	Laurent(const Poly<mint>&ff,int aa):f(ff),a(aa){}
	Laurent dif()const{
		return Laurent(f*(-a)+(f.dif()<<1),a+1);
	}
	mint&operator[](int i){
		assert(inc(0,i+a,si(f)-1));
		return f[i+a];
	}
};
