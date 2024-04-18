//UCUP 2023-3 F
template<class F>
void segeasy(int s,int l,int r,F f){
	for(l+=s,r+=s;l<r;l>>=1,r>>=1){
		if(l&1)f(l);
		if(r&1)f(r-1);
		l++; //ceil l
	}
}

//UOJ Goodbye Renyin C
//range chmax,point get
struct segeasy{
	const int n,s;
	vi a;
	segeasy(int nn):n(nn),s(minp2(n)),a(2*s,-inf){}
	template<class F>
	void fpoint(int i,F f){
		for(i+=s;i;i>>=1)f(i);
	}
	template<class F>
	void frange(int l,int r,F f){
		for(l+=s,r+=s;l<r;l>>=1,r>>=1){
			if(l&1)f(l);
			if(r&1)f(r-1);
			l++; //ceil l
		}
	}
	void upd(int l,int r,int v){
		frange(l,r,[&](int i){
			chmax(a[i],v);
		});
	}
	int get(int p){
		int res=-inf;
		fpoint(p,[&](int i){
			chmax(res,a[i]);
		});
		return res;
	}
};
