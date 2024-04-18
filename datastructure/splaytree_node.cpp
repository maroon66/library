//GCJ2022 Round2 D
//range add, find by len, find by val
struct N{
	int lz,len,val;
	N(int v=0):lz(0),len(1),val(v){}
	void add(int v){
		lz+=v;
		val+=v;
	}
	void reverse(){}
	void push(N&x){
		x.add(lz);
	}
	void clear(){
		lz=0;
	}
	void single(){
		assert(lz==0);
		len=1;
	}
	void upd(const N&x){
		assert(lz==0);
		len+=x.len;
	}
	void updatel(const N&x){
		upd(x);
	}
	void updater(const N&x){
		upd(x);
	}
	bool find_by_len(int&x){
		if(x<len)return true;
		x-=len;
		return false;
	}
	static bool cmp(const N&a,const N&b){
		return a.val<b.val;
	}
};

//XX Opencup GP of Wroclaw D
//x 昇順，y 降順の列を管理
struct N{
	int x,y;
	N(int a=0,int b=0):x(a),y(b){}
	void reverse(){}
	void push(N&){
	}
	void clear(){
	}
	void single(){
	}
	void updatel(const N&){
	}
	void updater(const N&){
	}
	static bool cmpX(const N&a,const N&b){
		return a.x<b.x;
	}
	static bool cmpY(const N&a,const N&b){
		return a.y>b.y;
	}
};

//range add,sum
struct N{
	int lz,len,val,sum;
	N(int v=0):lz(0),len(1),val(v),sum(v){}
	void add(int v){
		lz+=v;
		val+=v;
		sum+=len*v;
	}
	void reverse(){}
	void push(N&x){
		x.add(lz);
	}
	void clear(){
		lz=0;
	}
	void single(){
		assert(lz==0);
		len=1;
		sum=val;
	}
	void upd(const N&x){
		assert(lz==0);
		len+=x.len;
		sum+=x.sum;
	}
	void updatel(const N&x){
		upd(x);
	}
	void updater(const N&x){
		upd(x);
	}
	bool find(int&x){
		if(x<len)return true;
		x-=len;
		return false;
	}
};

//Yandex Cup 2020 Qual DE
//multiset<int> で値の sum が取れるだけ
struct N{
	int val,sum,len;
	N(int v=0):val(v),sum(v),len(1){}
	void reverse(){}
	void push(N&){
	}
	void clear(){
	}
	void single(){
		sum=val;
		len=1;
	}
	void updatel(const N&x){
		sum+=x.sum;
		len+=x.len;
	}
	void updater(const N&x){
		sum+=x.sum;
		len+=x.len;
	}
	static bool cmp(const N&x,const N&y){
		return x.val<y.val;
	}
	bool find(int&s)const{
		if(len<=s){
			s-=len;
			return false;
		}else{
			return true;
		}
	}
};

//usage
void slv(){
	int n;cin>>n;
	splaytree<N> t(n);
	using np=splaytree<N>::np;
	np x=0;
	int ans=0;
	vi res;
	rep(i,n){
		int v;cin>>v;
		np a,b;tie(a,b)=t.split_cmp(x,N::cmp,N(v+1));
		if(b)ans+=b->x.sum;
		x=t.insert_cmp(t.merge(a,b),N::cmp,N(v));
		res.pb(ans);
	}
	print(res);
}

//usage
void slv(){
	int n,k,r;cin>>n>>k>>r;
	splaytree<N> t(n);
	using np=splaytree<N>::np;
	np x=t.build(readvi(n));
	rep(_,r){
		int tmp=k;
		np a,b;tie(a,b)=t.split(x,&N::find,tmp);
		int s=a->x.sum;
		np c=a->left();
		a=t.erase(c);
		x=t.insert_cmp(t.merge(a,b),&N::cmp,N(s));
	}
	vc<N> res;
	t.enumerate(x,res);
	vi ans(n);
	rep(i,n)ans[i]=res[i].val;
	print(ans);
}


//2021 USACO USOPEN Platinum A
//sorted by key
//range add,sum
struct N{
	int key,lz,len,val,sum;
	N(pi kv=pi(0,0)):key(kv.a),lz(0),len(1),val(kv.b),sum(kv.b){}
	void add(int v){
		lz+=v;
		val+=v;
		sum+=len*v;
	}
	void reverse(){}
	void push(N&x){
		x.add(lz);
	}
	void clear(){
		lz=0;
	}
	void single(){
		assert(lz==0);
		len=1;
		sum=val;
	}
	void upd(const N&x){
		assert(lz==0);
		len+=x.len;
		sum+=x.sum;
	}
	void updatel(const N&x){
		upd(x);
	}
	void updater(const N&x){
		upd(x);
	}
	static bool cmp(const N&x,const N&y){
		return x.key<y.key;
	}
	bool find(int&x){
		if(x<len)return true;
		x-=len;
		return false;
	}
};

void slv(){
	int n;cin>>n;
	vi b=readvi(n,-1);
	vi last(n,-1),pre(n,-1);
	splaytree<N> t(n);
	using np=splaytree<N>::np;
	np root=nullptr;
	int ans=0;
	rep(i,n){
		int v=b[i];
		pre[i]=last[v];
		last[v]=i;
		int j=pre[i];
		auto [x,y]=t.split_cmp(root,&N::cmp,N(pi(j,0)));
		if(j!=-1){
			assert(y);
			y=t.erase(y->left());
			int k=pre[j];
			auto [l,r]=t.split_cmp(x,&N::cmp,N(pi(k,0)));
			if(r)r->x.add(-1);
			x=t.merge(l,r);
		}
		if(y){
			ans+=y->x.sum;
			y->x.add(1);
		}
		root=t.insert_cmp(t.merge(x,y),&N::cmp,N(pi(i,0)));
	}
	print(ans);
}

//The 2022 ICPC Asia Nanjing Regional Contest H
//一次関数を足す
struct N{
	int lzA,lzB,val,len,Llen;
	N(int v=0):lzA(0),lzB(0),val(v),len(1),Llen(0){}
	void add(int a,int b){
		lzA+=a;
		lzB+=b;
		val+=a*Llen+b;
	}
	void reverse(){}
	void pushl(N&x){
		x.add(lzA,lzB);
	}
	void pushr(N&x){
		x.add(lzA,lzA*(Llen+1)+lzB);
	}
	void clear(){
		lzA=0;
		lzB=0;
	}
	void single(){
		assert(lzA==0);
		assert(lzB==0);
		len=1;
		Llen=0;
	}
	void updatel(const N&x){
		len+=x.len;
		Llen+=x.len;
	}
	void updater(const N&x){
		len+=x.len;
	}
	bool operator<(const N&rhs)const{
		return val>rhs.val;
	}
};
