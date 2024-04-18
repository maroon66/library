//source: Tellegen’s Principle into Practice

//Yukicoder No.1145
//top には，(x-a[i]) の積が入っている(a がサイズ s に拡張されていることに注意)
//なので例えば (1-a[i]x) の積が欲しい場合は，
//	Poly<mint> f=s.top;
//	Poly<mint> g(n+1);
//	rep(i,n+1)g[i]=f[si(f)-1-i];
template<class mint>
struct subproduct_tree{
	using poly=Poly<mint>;
	int raws,s,h;
	mint*buf;
	vc<mint*>f;
	vi len;
	poly top;
	void inner_product(const int n,const mint*a,const mint*b,mint*c){
		rep(i,n)c[i]=a[i]*b[i];
	}
	//first n elements are fft-ed
	//last n elements are raw data mod x^n-1
	//the coefficient of x^n is v
	//convert the whole array into size-2n fft-ed array
	void doubling_fmt(const int n,mint*a,const mint v){
		a[n]-=v*2;
		half_fmt(n,a+n);
	}
	subproduct_tree(const vc<mint>&a){
		raws=si(a);
		s=1;while(s<si(a))s*=2;
		h=__lg(s)+1;
		buf=new mint[s*h*2];
		f.resize(s*2);
		len.resize(s*2);
		{
			mint*head=buf;
			rng(i,1,s*2){
				len[i]=s>>__lg(i);
				f[i]=head;
				head+=len[i]*2;
			}
		}
		rep(i,s){
			mint w=i<si(a)?a[i]:0;
			f[s+i][0]=-w+1;
			f[s+i][1]=-w-1;
		}
		if(s==1)f[1][1]=f[1][0];
		gnr(i,1,s){
			inner_product(len[i],f[i*2],f[i*2+1],f[i]);
			copy(f[i],f[i]+len[i],f[i]+len[i]);
			inplace_fmt(len[i],f[i]+len[i],true);
			if(i>1)doubling_fmt(len[i],f[i],1);
		}
		top.resize(s+1);
		rep(i,s)top[i]=f[1][s+i];
		top[0]-=1;
		top[s]=1;
	}
	~subproduct_tree(){
		delete[] buf;
	}
	//VERIFY: yosupo
	vc<mint> multieval(const poly&b){
		mint*buf2=new mint[s*2];
		vc<mint*> c(s*2);
		{
			mint*head=buf2;
			rng(i,1,s*2){
				if((i&(i-1))==0&&__lg(i)%2==0)head=buf2;
				c[i]=head;
				head+=len[i];
			}
		}
		{
			poly tmp=top.rev().inv(si(b)).rev()*b;
			rep(i,s)c[1][i]=i<si(b)?tmp[si(b)-1+i]:0;
		}
		vc<mint> tmp(s);
		rng(i,1,s){
			inplace_fmt(len[i],c[i],false);
			rep(k,2){
				tmp.resize(len[i]);
				rep(j,len[i])tmp[j]=f[i*2+(k^1)][j]*c[i][j];
				inplace_fmt(tmp,true);
				rep(j,len[i]/2)c[i*2+k][j]=tmp[len[i]/2+j];
			}
		}
		vc<mint> ans(raws);
		rep(i,raws)ans[i]=c[s+i][0];
		delete[] buf2;
		return ans;
	}
	poly interpolate(const vc<mint>&val){
		mint*buf2=new mint[s*2*2];
		vc<mint*> c(s*2);
		{
			mint*head=buf2;
			rng(i,1,s*2){
				if((i&(i-1))==0&&__lg(i)%2==0)head=buf2;
				c[i]=head;
				head+=len[i]*2;
			}
		}
		{
			vc<mint> z=multieval(poly(top.bg+(s-si(val)),top.ed).dif());
			rep(i,s){
				mint w=i<si(val)?val[i]/z[i]:0;
				c[s+i][0]=c[s+i][1]=w;
			}
		}
		gnr(i,1,s){
			rep(j,len[i])
				c[i][j]=c[i*2][j]*f[i*2+1][j]+c[i*2+1][j]*f[i*2][j];
			copy(c[i],c[i]+len[i],c[i]+len[i]);
			inplace_fmt(len[i],c[i]+len[i],true);
			if(i>1)doubling_fmt(len[i],c[i],0);
		}
		poly res(c[1]+s+(s-si(val)),c[1]+s*2);
		delete[] buf2;
		return res;
	}
	//res[i]=prod_{j!=i} (a[i]-a[j])
	//Yukicoder 2556
	vc<mint> product_of_difference(){
		return multieval(poly(top.bg+(s-raws),top.ed).dif());
	}
};

template<class mint>
vc<mint> multieval(const Poly<mint>&f,const vc<mint>&x){
	return subproduct_tree<mint>(x).multieval(f);
}

template<class mint>
Poly<mint> interpolate(const vc<mint>&x,const vc<mint>&y){
	assert(si(x)==si(y));
	if(si(x)==1)return {y[0]};
	subproduct_tree<mint> tree(x);
	return tree.interpolate(y);
}

//a==-1 だけ verify されてる
//LOJ 6703
//product of ax+b
//ARC155F
template<class mint>
Poly<mint> product_linear(const vc<pair<mint,mint>>&rw){
	mint w=1;
	vc<mint> buf;
	for(auto ab:rw){
		if(ab.a==0){
			w*=ab.b;
		}else{
			w*=ab.a;
			buf.pb(-ab.b/ab.a);
		}
	}
	subproduct_tree<mint> tree(buf);
	const auto&f=tree.top;
	int n=si(buf)+1;
	vc<mint> res(n);
	rep(i,n)res[i]=f[si(f)-n+i]*w;
	res.resize(si(rw)+1);
	return res;
}
