//merge で片方が inactive のときはもう片方をそのまま返す，
//といったときに，lazy の情報までコピーして渡さないようにする

//get の最後の引数は単位元と口では言いつつ・・・？
//たとえば min で最後の引数を 0 にしても 1 とかが返ってくることはある（一敗）

//VERIFY: yosupo
//KUPC2017I
//HDU 5306 Gorgeous Sequence
//findmin/max CF458E
template<class N>
struct segbeats{
	vc<N> x;
	int s;
	segbeats(){}
	template<class T>
	segbeats(const vc<T>& a){
		int n=a.size();
		s=1;
		while(s<n)s*=2;
		x.resize(s*2);
		rep(i,n)
			x[s+i]=N(a[i]);
		gnr(i,1,s)
			upd(i);
	}
	void push(int i){
		x[i].push(x[i*2],x[i*2+1]);
	}
	void upd(int i){
		x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	template<class F,class... Args>
	void chr(int l,int r,int i,int b,int e,F f,Args&&... args){
		if(e<=l||r<=b)
			return;
		if(b<=l&&r<=e&&(x[i].*f)(forward<Args>(args)...))
			return;
		push(i);
		int m=(l+r)/2;
		chr(l,m,i*2,b,e,f,forward<Args>(args)...);
		chr(m,r,i*2+1,b,e,f,forward<Args>(args)...);
		upd(i);
	}
	template<class F,class... Args>
	void ch(int b,int e,F f,Args&&... args){
		assert(b<=e);
		chr(0,s,1,b,e,f,forward<Args>(args)...);
	}
	//use decltype((declval<N>().*F())()) for old-fashioned judges
	template<class F,class G,class H>
	auto getr(int l,int r,int i,int b,int e,F f,G g,H h){
		if(e<=l||r<=b)
			return h;
		if(b<=l&&r<=e)
			return (x[i].*f)();
		push(i);
		int m=(l+r)/2;
		return g(getr(l,m,i*2,b,e,f,g,h),getr(m,r,i*2+1,b,e,f,g,h));
	}
	template<class F,class G,class H>
	auto get(int b,int e,F f,G g,H h){
		assert(b<=e);
		return getr(0,s,1,b,e,f,g,h);
	}
	auto compositer(int l,int r,int i,int b,int e){
		if(e<=l||r<=b)assert(0);
		if(b<=l&&r<=e)
			return x[i];
		push(i);
		int m=(l+r)/2;
		if(e<=m)return compositer(l,m,i*2,b,e);
		if(m<=b)return compositer(m,r,i*2+1,b,e);
		return N::merge(compositer(l,m,i*2,b,e),compositer(m,r,i*2+1,b,e));
	}
	//work without identity node
	auto composite(int b,int e){
		assert(b<e);
		return compositer(0,s,1,b,e);
	}
	N getall(){return x[1];}
	//return minimum index
	template<class F,class...Args>
	pair<int,N> findminr(int i,int l,int r,int b,int e,F f,Args&&...args){
		if(e<=l||r<=b)return {e,N()};
		if(b<=l&&r<=e){
			if(!(x[i].*f)(forward<Args>(args)...))return {e,N()};
			if(r-l==1)return {l,x[i]};
		}
		push(i);
		int m=(l+r)/2;
		auto a=findminr(i*2,l,m,b,e,f,forward<Args>(args)...);
		if(a.a<e)return a;
		return findminr(i*2+1,m,r,b,e,f,forward<Args>(args)...);
	}
	template<class F,class...Args>
	pair<int,N> findmin(int b,int e,F f,Args&&...args){
		assert(b<=e);
		return findminr(1,0,s,b,e,f,forward<Args>(args)...);
	}
	//return maximum index
	template<class F,class...Args>
	pair<int,N> findmaxr(int i,int l,int r,int b,int e,F f,Args&&...args){
		if(e<=l||r<=b)return {b-1,N()};
		if(b<=l&&r<=e){
			if(!(x[i].*f)(forward<Args>(args)...))return {b-1,N()};
			if(r-l==1)return {l,x[i]};
		}
		push(i);
		int m=(l+r)/2;
		auto a=findmaxr(i*2+1,m,r,b,e,f,forward<Args>(args)...);
		if(a.a>=b)return a;
		return findmaxr(i*2,l,m,b,e,f,forward<Args>(args)...);
	}
	template<class F,class...Args>
	pair<int,N> findmax(int b,int e,F f,Args&&...args){
		assert(b<=e);
		return findmaxr(1,0,s,b,e,f,forward<Args>(args)...);
	}
	void enumerater(int l,int r,int i,int b,int e,vc<N>&dst){
		if(e<=l||r<=b)
			return;
		if(l+1==r){
			dst.pb(x[i]);
			return;
		}
		push(i);
		int m=(l+r)/2;
		enumerater(l,m,i*2,b,e,dst);
		enumerater(m,r,i*2+1,b,e,dst);
	}
	void enumerate(int b,int e,vc<N>&dst){
		assert(b<=e);
		return enumerater(0,s,1,b,e,dst);
	}
	
	//KUPC 2020 G
	template<class F,class...Args>
	void enumerate_by_findr(int l,int r,int i,int b,int e,vc<pair<int,N>>&dst,F f,Args&&...args){
		if(e<=l||r<=b||!(x[i].*f)(forward<Args>(args)...))
			return;
		if(l+1==r){
			dst.eb(l,x[i]);
			return;
		}
		push(i);
		int m=(l+r)/2;
		enumerate_by_findr(l,m,i*2,b,e,dst,f,forward<Args>(args)...);
		enumerate_by_findr(m,r,i*2+1,b,e,dst,f,forward<Args>(args)...);
	}
	template<class F,class...Args>
	void enumerate_by_find(int b,int e,vc<pair<int,N>>&dst,F f,Args&&...args){
		assert(b<=e);
		enumerate_by_findr(0,s,1,b,e,dst,f,forward<Args>(args)...);
	}
	void prepare(int i){
		if(i/=2){
			prepare(i);
			push(i);
		}
	}
	//point_update と lazy を組み合わせたらどうなるかは，わからない・・・
	void point_set(int i,N w){
		i+=s;
		prepare(i);
		x[i]=w;
		while(i/=2)
			upd(i);
	}
	template<class F,class...Args>
	void point_change(int i,F f,Args&&...args){
		i+=s;
		prepare(i);
		(x[i].*f)(forward<Args>(args)...);
		while(i/=2)
			upd(i);
	}
	//not verified
	N point_get(int i){
		i+=s;
		prepare(i);
		return x[i];
	}
};

//N::push
//pushしたあとはclearする
//N::merge
//変更操作が失敗するときはそのノードに変更を及ぼさない

//usage

//range chmin,sum
//何もかもを ull で計算している
struct N{
	ull mx,cnt,sec,sum,lz;
	N(ull v=0):mx(v),cnt(1),sec(0),sum(v),lz(-1){}
	bool setlim(ull v){
		if(mx<=v){
			chmin(lz,v);
			return true;
		}
		if(sec<v){
			chmin(lz,v);
			sum-=(mx-v)*cnt;
			mx=v;
			return true;
		}
		return false;
	}
	void push(N&x,N&y){
		bool tmp=x.setlim(lz);
		assert(tmp);
		tmp=y.setlim(lz);
		assert(tmp);
		lz=-1;
	}
	static N merge(N x,N y){
		N res;
		res.mx=max(x.mx,y.mx);
		res.sec=max(x.sec,y.sec);
		res.cnt=0;
		if(res.mx==x.mx)res.cnt+=x.cnt;
		else chmax(res.sec,x.mx);
		if(res.mx==y.mx)res.cnt+=y.cnt;
		else chmax(res.sec,y.mx);
		res.sum=x.sum+y.sum;
		return res;
	}
};

//simple range max
struct N{
	int mx;
	N(int v=-1):mx(v){}
	bool set(int v){
		mx=v;
		return true;
	}
	void push(N&,N&){
	}
	static N merge(N x,N y){
		return N(max(x.mx,y.mx));
	}
	bool find(int v){
		return mx>=v;
	}
};

//range add linear function
//don't forget to pass the second argument to addA
struct N{
	int s,len,la,lb;
	N(int v=0,int w=1):s(v),len(w),la(0),lb(0){}
	bool addA(int v,int&d){
		la+=v;
		s+=len*(len-1)/2*v;
		addB(d*v);
		d+=len;
		return true;
	}
	bool addB(int v){
		lb+=v;
		s+=len*v;
		return true;
	}
	void push(N&x,N&y){
		int tmp=0;
		x.addA(la,tmp);
		y.addA(la,tmp);
		x.addB(lb);
		y.addB(lb);
		la=0;
		lb=0;
	}
	static N merge(N x,N y){
		return N(x.s+y.s,x.len+y.len);
	}
	int gets(){return s;}
};

//point set,range query sum lower_bound
struct N{
	int s;
	N(int v=0):s(v){}
	bool set(int v){
		s=v;
		return true;
	}
	void push(N&,N&){
	}
	static N merge(N x,N y){
		return N(x.gets()+y.gets());
	}
	int gets(){return s;}
	bool find(int&v){
		if(s<v){
			v-=s;
			return false;
		}
		return true;
	}
};

//find は多分問題ないと思うけど一応注意して
//usage

//Range add,max
struct N{
	int lz,mx;
	N(int v=-inf):lz(0),mx(v){}
	bool add(int v){
		mx+=v;
		lz+=v;
		return true;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static N merge(N x,N y){
		return N(max(x.getm(),y.getm()));
	}
	int getm(){return mx;}
	bool find(int v){
		return getm()>=v;
	}
};

//Range add,min
struct N{
	int lz,mn;
	N(int v=inf):lz(0),mn(v){}
	bool add(int v){
		mn+=v;
		lz+=v;
		return true;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static N merge(N x,N y){
		return N(min(x.getm(),y.getm()));
	}
	int getm(){return mn;}
};

//区間クエリを投げたらぶっ壊れる！
//全体get以外はダメ！
//Range add,min,count
struct N{
	int lz,mn,cnt;
	N(int v=0):lz(0),mn(v),cnt(1){}
	bool add(int v){
		lz+=v;
		mn+=v;
		return true;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static N merge(N x,N y){
		N res(min(x.mn,y.mn));
		res.cnt=0;
		if(res.mn==x.mn)
			res.cnt+=x.cnt;
		if(res.mn==y.mn)
			res.cnt+=y.cnt;
		return res;
	}
	int getzeros(){
		return mn==0?cnt:0;
	}
};

//affine
struct N{
	mint a,b,c;
	int s;
	N(mint v=0,int len=1):a(1),b(0),c(v),s(len){}
	bool affine(pair<mint,mint> z){
		a*=z.a;
		b=b*z.a+z.b;
		c*=z.a;
		c+=z.b*s;
		return true;
	}
	void push(N&x,N&y){
		x.affine(mp(a,b));
		y.affine(mp(a,b));
		a=1;
		b=0;
	}
	static N merge(N x,N y){
		return N(x.c+y.c,x.s+y.s);
	}
	mint getsum(){
		return c;
	}
};

//ch 内部では false を返す際に内部を変更しない!

//chmin,chmax,add,sum
//VERIFY:yosupo
struct N{
	const static int w[2];
	struct MaxData{
		int a,b,c;
		MaxData(int v):a(v),b(-inf),c(1){}
		int ch(int v){
			assert(v>b);
			int dif=min(v-a,int(0));
			a+=dif;
			return c*dif;
		}
		void updb(int v){
			assert(v<a);
			chmax(b,v);
		}
		void add(int v){
			a+=v;
			b+=v;
		}
		static MaxData merge(const MaxData&x,const MaxData&y){
			MaxData res(max(x.a,y.a));
			res.c=0;
			for(auto z:{x,y}){
				chmax(res.b,z.a<res.a?z.a:z.b);
				if(res.a==z.a)res.c+=z.c;
			}
			return res;
		}
	} d[2];
	int sum,len,lzc[2],lza;
	N(int v=0):d{MaxData(v),MaxData(-v)},sum(v),len(1),lzc{inf,inf},lza(0){}
	bool allsame(){
		return d[0].a==-d[1].a;
	}
	//0->chmin, 1->chmax
	bool change(int k,int v){
		v*=w[k];
		if(allsame()){
			return add(min(v-d[k].a,int(0))*w[k]);
		}
		if(v>d[k].b){
			chmin(lzc[k],v);
			sum+=d[k].ch(v)*w[k];
			d[k^1].updb(-v);
			return true;
		}
		return false;
	}
	bool add(int v){
		lza+=v;
		rep(k,2)lzc[k]+=v*w[k];
		sum+=len*v;
		rep(k,2)d[k].add(v*w[k]);
		return true;
	}
	void push(N&x,N&y){
		auto sub=[&](N&z){
			z.add(lza);
			rep(k,2){
				bool res=z.change(k,w[k]*lzc[k]);
				assert(res);
			}
		};
		sub(x);
		sub(y);
		rep(k,2)lzc[k]=inf;
		lza=0;
	}
	int gets(){
		return sum;
	}
	static N merge(const N&x,const N&y){
		N res;
		rep(k,2)res.d[k]=MaxData::merge(x.d[k],y.d[k]);
		res.sum=x.sum+y.sum;
		res.len=x.len+y.len;
		return res;
	}
};
const int N::w[2]{1,-1};

//01 flip count
struct N{
	int a[2],lz;
	N(int v=0):a{1-v,v},lz(0){}
	bool flip(){
		swap(a[0],a[1]);
		lz^=1;
		return true;
	}
	void push(N&x,N&y){
		if(lz){
			x.flip();
			y.flip();
		}
		lz=0;
	}
	static N merge(N x,N y){
		N res;
		rep(i,2)res.a[i]=x.a[i]+y.a[i];
		return res;
	}
	int get0(){return a[0];}
};


//KUPC 2020 G
//a と c ２つの値を管理
//c の min のなかでの a の最大を持ってくる
//(?) 区間クエリを投げたらぶっ壊れる！
//(?) 全体get以外はダメ！
//c の初期値いじったのでダメかな？
struct N{
	int a,c,la,lc;
	N(int v=-inf):a(v),c(v==-inf?inf:0),la(0),lc(0){}
	bool add_a(int v){
		a+=v;
		la+=v;
		return true;
	}
	bool add_c(int v){
		c+=v;
		lc+=v;
		return true;
	}
	void push_sub(N&x){
		x.add_a(la);
		x.add_c(lc);
	}
	void push(N&x,N&y){
		push_sub(x);
		push_sub(y);
		la=0;
		lc=0;
	}
	void upd_sub(const N&x){
		if(c==x.c)chmax(a,x.a);
	}
	static N merge(const N&x,const N&y){
		N res;
		res.c=min(x.c,y.c);
		res.upd_sub(x);
		res.upd_sub(y);
		return res;
	}
	bool find(int v){
		return c==0&&v==a;
	}
};


//CF875F
//x,y の列があって，区間を選んで y+=x をして y の max　を返してもらう
struct N{
	pi mx;
	int t,lz;
	N(pi v=pi(0,-inf)):mx(v),t(inf),lz(0){}
	bool add(int v){
		if(t<v)return false;
		mx.b+=mx.a*v;
		t-=v;
		lz+=v;
		return true;
	}
	void push(N&x,N&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static N merge(const N&x,const N&y){
		pi a=x.mx,b=y.mx;
		if(a.b<b.b)swap(a,b);
		N res;res.mx=a;
		res.t=min(x.t,y.t);
		if(a.a<b.a)chmin(res.t,(a.b-b.b)/(b.a-a.a));
		return res;
	}
};
