//idx を x を使ってバケットソート
//安定
void bucket(const vi&x,vi&idx){
	static vi pos,out;
	int m=MAX(x)+1;
	pos.assign(m+1,0);
	for(auto i:idx)pos[x[i]+1]++;
	rep(i,m)pos[i+1]+=pos[i];
	out.resize(si(idx));
	for(auto i:idx)out[pos[x[i]]++]=i;
	idx.swap(out);
}

//等差数列
//ax+b (0<=x<c)
struct ap{
	int a,b,c;
	ap():a(1),b(0),c(0){}
	explicit ap(int x):a(1),b(x),c(1){}
	explicit ap(int x,int y):a(y-x),b(x),c(2){}
	explicit ap(int aa,int bb,int cc):a(aa),b(bb),c(cc){}
	void load(vi::iterator l,vi::iterator r){
		int len=r-l;
		assert(len>=0);
		if(len==0){
			a=b=c=0;
		}else if(len==1){
			a=0;
			b=*l;
			c=1;
		}else if(len>=2){
			int x=*l,y=*(l+1);
			a=y-x;
			b=x;
			c=len;
		}
	}
	//y -> y+v
	void add(int v){
		b+=v;
	}
	void rev(){
		if(c){
			b=back();
			a*=-1;
		}
	}
	void neg(){
		a*=-1;
		b*=-1;
	}
	bool contains(int v){
		if(c==0)return false;
		if(a==0)return b==v;
		v-=b;
		if(v%a)return false;
		return inc(0,v/a,c-1);
	}
	int front(){
		assert(c);
		return b;
	}
	int back(){
		assert(c);
		return a*(c-1)+b;
	}
	void enumerate(vi&x){
		rep(i,c)x.pb(a*i+b);
	}
	int eval(int i){
		return a*i+b;
	}
	//先頭に k 個付け足す
	void push_front(int k){
		assert(k>=0);
		b-=a*k;
		c+=k;
	}
	//k 個先頭から消す
	void pop_front(int k){
		assert(inc(0,k,c));
		b+=a*k;
		c-=k;
	}
	//k 個後ろから消す　
	void pop_back(int k){
		assert(inc(0,k,c));
		c-=k;
	}
	void cap(int v){
		assert(a>0);
		if(b>v)c=0;
		else chmin(c,(v-b)/a+1);
	}
};
ostream&operator<<(ostream&os,const ap&z){
	return os<<"AP{"<<z.a<<","<<z.b<<","<<z.c<<"}";
}
void compressaps(vc<ap>&ls){
	for(auto z:ls)assert(z.c>0);
	int tail=si(ls)-1;
	per(i,si(ls)-1){
		if(ls[tail].c==1)ls[tail].a=ls[tail].front()-ls[i].back();
		if(ls[i].back()==ls[tail].eval(-1)){
			if(ls[i].a==ls[tail].a){
				ls[tail].push_front(ls[i].c);
			}else{
				ls[tail].push_front(1);
				ls[i].pop_back(1);
				if(ls[i].c>0)ls[--tail]=ls[i];
			}
		}else{
			ls[--tail]=ls[i];
		}
	}
	ls.erase(ls.bg,ls.bg+tail);
}
void capborders(vc<ap>&ls,int wmax){
	assert(0<=wmax);
	int s=0;
	for(auto z:ls){
		z.cap(wmax);
		if(z.c)ls[s++]=z;
	}
	ls.resize(s);
	compressaps(ls);
}

ap mg(ap x,ap y){
	if(x.c==0||y.c==0)return ap();
	//for borders or something O(1)
	//一般のケースでは CRT とかだろうね
	rep(_,2){
		if(x.c==1){
			if(y.contains(x.front()))return ap(x.front());
			else return ap();
		}else if(x.c==2){
			if(y.contains(x.front())){
				if(y.contains(x.back()))
					return ap(x.front(),x.back());
				else
					return ap(x.front());
			}else{
				if(y.contains(x.back()))
					return ap(x.back());
				else
					return ap();
			}
		}
		swap(x,y);
	}
	assert(x.a==y.a);
	assert(0<x.a);
	assert(0<=x.b);
	assert(0<=y.b);
	if(x.b%x.a!=y.b%y.a)return ap();
	int l=max(x.front(),y.front()),r=min(x.back(),y.back());
	if(r<l)return ap();
	else if(l==r)return ap(l);
	else return ap(x.a,l,(r-l)/x.a+1);
}

//https://www.luogu.com.cn/blog/ix-35/zi-fu-chuan-ji-shu-xun-li
//https://hydro.ac/d/luogu/p/P8006
struct basic_substring_dictionary{
	int n,L;
	vvc<int> kind;
	vvc<vi> pos;
	template<class S>
	basic_substring_dictionary(const S&s){
		n=si(s);
		L=topbit(n)+1;
		kind.resize(L);
		auto vs=s;mkuni(vs);
		vi ord=vid(n);
		kind[0].resize(n);
		rep(i,n)kind[0][i]=lwb(vs,s[i]);
		bucket(kind[0],ord);
		rng(lv,1,L){
			kind[lv].resize(n-(1<<lv)+1);
			int w=1<<(lv-1),len=0;
			for(auto&i:ord)if(i>=w)ord[len++]=i-w;
			ord.resize(len);
			bucket(kind[lv-1],ord);
			kind[lv].resize(len);
			int u=0;
			pi pre(-1,-1);
			for(auto i:ord){
				pi cur(kind[lv-1][i],kind[lv-1][i+w]);
				assert(pre<=cur);
				if(pre!=cur)u++;
				kind[lv][i]=u-1;
				pre=cur;
			}
		}
		pos.resize(L);
		rep(lv,L){
			int m=MAX(kind[lv])+1;
			pos[lv].resize(m);
			rep(i,si(kind[lv]))pos[lv][kind[lv][i]].pb(i);
		}
	}
	//[x,x+w)=[y-w,w) なる w を列挙する
	//log N 個の等差数列で表せる
	//w を昇順に並べて返す
	void presuf(int x,int y,vc<ap>&ls){
		ls.clear();
		ls.pb(ap(0));//0 含む
		rep(lv,L){
			int w=1<<lv;
			if(x+w<=n&&0<=y-w){
				int u=kind[lv][x],v=kind[lv][y-w];
				ap S0,S1;
				S0.load(
					lower_bound(all(pos[lv][u]),y-2*w+1),
					lower_bound(all(pos[lv][u]),y-w+1)
					);
				S0.rev();
				S0.neg();
				S0.add(y);
				S1.load(
					lower_bound(all(pos[lv][v]),x),
					lower_bound(all(pos[lv][v]),x+w)
					);
				S1.add(-x+w);
				auto z=mg(S0,S1);
				if(z.c)ls.pb(z);
			}
		}
	}
};
