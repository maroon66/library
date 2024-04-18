//CF829 F
template<int B>
struct mybitset{
	static constexpr int L=(B+63)/64;
	ull x[L];
	int c=0;
	mybitset():x{},c(0){};
	void set(){
		one(x);
		int p=B/64,q=B%64;
		if(p<L)x[p]=mask(q);
		c=B;
	}
	//not verified
	void reset(){
		zero(x);
		c=0;
	}
	void set(int i){
		if(!operator[](i))c++;
		x[i/64]|=1ull<<(i%64);
	}
	void reset(int i){
		if(operator[](i))c--;
		x[i/64]&=~(1ull<<(i%64));
	}
	bool operator[](int i)const{return (x[i/64]>>(i%64))&1;}
	bool any()const{return c;}
	//i 以上の最小
	int next(int i){
		int p=i/64,q=i%64;
		if(p<L){
			ull v=x[p]>>q;
			if(v)return botbit(v)+i;
			p++;
		}
		rng(j,p,L)if(x[j]){
			return botbit(x[j])+j*64;
		}
		return B;
	}
	//not verified
	//i 未満の最大
	int prev(int i){
		int p=i/64,q=i%64;
		if(p<L){
			ull v=x[p]&mask(q);
			if(v)return topbit(v)+p*64;
		}
		per(j,p)if(x[j]){
			return topbit(x[j])+j*64;
		}
		return -1;
	}
};

//CF829 F
template<int B>
struct fastset{
	using A=mybitset<B>;
	int n,lg;
	vvc<A> seg;
	fastset(int _n,bool v):n(_n){
		A ini;if(v)ini.set();
		do{
			seg.push_back(vc<A>(_n=(_n+B-1)/B,ini));
		}while(_n>1);
		lg=si(seg);
	}
	//not verified
	/*void fillone(){
		for(auto&a:seg)for(auto&v:a)v.set();
	}*/
	void fillzero(){
		for(auto&a:seg)for(auto&v:a)v.reset();
	}
	bool operator[](int i)const{
		return seg[0][i/B][i%B];
	}
	void set(int i){
		rep(h,lg){
			seg[h][i/B].set(i%B);
			i/=B;
		}
	}
	void reset(int i){
		rep(h,lg){
			seg[h][i/B].reset(i%B);
			if (seg[h][i/B].any())break;
			i/=B;
		}
	}
	//verified only with i=0
	//x以上最小の要素
	int next(int i){
		rep(h,lg){
			if(i/B==si(seg[h]))break;
			int j=seg[h][i/B].next(i%B);
			if(j==B){
				i=i/B+1;
				continue;
			}
			i=i/B*B+j;
			per(g,h){
				i=i*B+seg[g][i].next(0);
			}
			return i;
		}
		return n;
	}
	//not verified
	//x未満最大の要素
	int prev(int i){
		i--;
		rep(h,lg){
			if(i==-1)break;
			int j=seg[h][i/B].prev(i%B+1);
			if(j==-1){
				i=i/B-1;
				continue;
			}
			i=i/B*B+j;
			per(g,h){
				i=i*B+seg[g][i].prev(B);
			}
			return i;
		}
		return -1;
	}
};

//CF829 F
struct mexgetter{
	const int m;
	vi cnt;
	fastset<1024> fs;
	//m 未満の値しか来ない
	mexgetter(int m_):m(m_),cnt(m),fs(m,true){}
	void add(int v){
		assert(inc(0,v,m-1));
		cnt[v]++;
		if(cnt[v]==1)fs.reset(v);
	}
	void del(int v){
		assert(inc(0,v,m-1));
		cnt[v]--;
		if(cnt[v]==0)fs.set(v);
	}
	int mex(){
		return fs.next(0);
	}
};
