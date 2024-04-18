//使ったステップ数の情報を持つ convex hull
//F(a,b)=true なら a を優先
//min-hull なら less をいれればいい
//eval の値が 10^18 オーダーならだいたいうまく行くようになっているはず
//stress-tested (CC 2023-2 Cookoff G)
struct linelr{
	int a,b,c,d;
	int eval(int x){
		return a*x+b;
	}
};
ostream&operator<<(ostream&os,const linelr&ln){
	return os<<"L{"<<ln.a<<","<<ln.b<<","<<ln.c<<","<<ln.d<<"}";
}
template<class F>
struct vlr{
	int v=F()(inf,-inf)?-inf:inf,l=-inf,r=inf;
	static vlr merge(const vlr&a,const vlr&b){
		if(F()(a.v,b.v))return a;
		else if(F()(b.v,a.v))return b;
		else return {a.v,min(a.l,b.l),max(a.r,b.r)};
	}
	bool operator==(const vlr&rhs)const{
		return v==rhs.v&&l==rhs.l&&r==rhs.r;
	}
	bool operator!=(const vlr&rhs)const{
		return v!=rhs.v||l!=rhs.l||r!=rhs.r;
	}
	vlr operator+(int a)const{
		return {v+a,l,r};
	}
	void adv(int a){
		v+=a;
		l++;
		r++;
	}
};
template<class F>
ostream&operator<<(ostream&os,const vlr<F>&z){
	return os<<"vlr{"<<z.v<<",["<<z.l<<","<<z.r<<"]}";
}
template<class F>
struct chtlr{
	static int fdiv(int a,int b){
		return a / b - ((a ^ b) < 0 && a % b);
	}
	static int cdiv(int a,int b){
		return a / b + ((a ^ b) > 0 && a % b);
	}
	//can b be the opt?
	static bool cmpline(const linelr&a,const linelr&b,const linelr&c){
		int ay=a.b-b.b,ax=b.a-a.a;
		int by=b.b-c.b,bx=c.a-b.a;
		return cdiv(ay,ax)<=fdiv(by,bx);
	}
	vector<linelr> ls;
	int head,prex;
	vlr<F> preres;
	chtlr(){clear();}
	void clear(){
		ls.clear();
		head=0;
		prex=-inf;
		preres=vlr<F>();
	}
	//min-hull -> z.a is non-increasing
	void add(linelr z){
		if(si(ls))assert(!F()(ls.back().a,z.a));
		if(prex!=-inf)preres=vlr<F>::merge(preres,{z.eval(prex),z.c,z.d});
		if(si(ls)&&ls.back().a==z.a){
			auto&w=ls.back();
			if(F()(w.b,z.b)){
				return;
			}else if(F()(z.b,w.b)){
				ls.pop_back();
			}else{
				chmin(w.c,z.c);
				chmax(w.d,z.d);
				return;
			}
		}
		while(si(ls)>=2){
			int s=si(ls);
			if(cmpline(ls[s-2],ls[s-1],z))
				break;
			ls.pop_back();
		}
		chmin(head,si(ls));
		ls.push_back(z);
	}
	void add(int a,int b,int c,int d){add(linelr{a,b,c,d});}
	//x is non-decreasing
	vlr<F> query(int x){
		if(ls.empty())return vlr<F>();
		assert(prex<=x);
		if(prex==x)return preres;
		while(head+1<si(ls)){
			if(F()(ls[head+1].eval(x),ls[head].eval(x))) head++;
			else break;
		}
		int res=ls[head].eval(x),c=ls[head].c,d=ls[head].d;
		while(head+1<si(ls)&&!F()(res,ls[head+1].eval(x))){
			head++;
			chmin(c,ls[head].c);
			chmax(d,ls[head].d);
		}
		prex=x;
		return preres={res,c,d};
	}
};

//todo verify
//monge コストでちょうど k step でやれ，みたいな最小化問題 
//ans(step) が単調減少だとする(つまり小刻みであればあるほど嬉しい)
//max(ans(step))-max(ans(step)) の上界を dif で与えている
template<class F>
int kstepmin(int k,int dif,F f){
	assert(dif>=0);
	//int lw=-dif-1;
	int lw=-1; //(
	int up=dif+1;
	while(up-lw>1){
		int mid=(lw+up)/2;
		auto z=f(mid);
		if(z.r<k){
			up=mid;
		}else if(k<z.l){
			lw=mid;
		}else return z.v-mid*k;
	}
	dmp2(lw,up);
	assert(false);
}
