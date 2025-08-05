//UCUP 2023-3 F
template<class F>
void segeasy(int s,int l,int r,F f){
	for(l+=s,r+=s;l<r;l>>=1,r>>=1){
		if(l&1)f(l);
		if(r&1)f(r-1);
		l++; //ceil l
	}
}

//segeasy 的に各ノードに F を適用
//その上のノードに G を適用
//verify: range_detector
template<class F,class G>
void segeasy2(int s,int l,int r,F f,G g){
	assert(0<=l&&l<=r&&r<=s);
	if (l == r) return;
	l+=s;
	r+=s;
	
	static int buf[2][30];
	int cnt[2]{};
	{
		int l2 = l, r2 = r;
		while (l < r) {
			if (l & 1){
				buf[0][cnt[0]++]=l++;
			}
			if (r & 1){
				buf[1][cnt[1]++]=--r;
			}
			l >>= 1;
			r >>= 1;
		}
		l = l2;
		r = r2;
	}
	rep(i,cnt[0])f(buf[0][i]);
	per(i,cnt[1])f(buf[1][i]);

	for (int i = 1; (s>>i); i++) {
		int x=-1;
		if (((l >> i) << i) != l){
			x=l>>i;
			g(x);
		}
		if (((r >> i) << i) != r){
			int y=(r-1)>>i;
			if(x!=y)g(y);
		}
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
