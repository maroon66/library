//pos と言ったときには元の数列での位置を意味する．
//id と言ったときにはソート済み列での位置を意味する．
//stress-tested lwb_id,sum_by_id,sum_by_val,kth_id_by_sum,add
template<class t,class V>
struct wavelet{
	const vc<t> a;
	const vc<V> wei;
	const int n,L,s;
	vc<t> st;
	vi i2p,p2i;
	vec2d<int> pos;
	vec2d<pi> cut;
	vec2d<V> sum;
	void presum(int len,int*b,V*z){
		rep(i,len)z[i]+=wei[b[i]];
		rep(i,len){
			int j=i+((i+1)&(-i-1));
			if(j<len)z[j]+=z[i];
		}
	}
	void merge_with_cut(int ls,int *l,int rs,int *r,int *b,pi *c){
		c[0]=pi(0,0);
		int x=0,y=0;
		rep(i,ls+rs){
			bool usel=true;
			if(x==ls||(y<rs&&l[x]>r[y]))usel=false;
			if(usel)b[i]=l[x++];
			else b[i]=r[y++];
			c[i+1]=pi(x,y);
		}
	}
	wavelet(const vc<t>&aa,const vc<V>&ww):a(aa),wei(ww),n(si(a)),L(topbit(max<int>(n-1,1))+1),s(1<<L),
		st(n),i2p(n),p2i(n),pos(s*2,(L+1)*n),cut(s*2,L*n+s),sum(s*2,(L+1)*n+s*2)
	{
		{
			iota(all(i2p),0);
			stable_sort(all(i2p),[&](int i,int j){return a[i]<a[j];});
			rep(i,n)st[i]=a[i2p[i]];
			rep(i,n)p2i[i2p[i]]=i;
			rep(i,n){
				pos.init(s+i,1);
				pos[s+i][0]=i2p[i];
			}
		}
		gnr(i,1,s){
			int ls=pos.lens[i*2];
			int rs=pos.lens[i*2+1];
			pos.init(i,ls+rs);
			cut.init(i,ls+rs+1);
			merge_with_cut(ls,pos[i*2],rs,pos[i*2+1],pos[i],cut[i]);
		}
		rng(i,1,s*2){
			sum.init(i,pos.lens[i]);
			presum(pos.lens[i],pos[i],sum[i]);
		}
	}
	pi lrL(int i,int l,int r){
		return pi(cut[i][l].a,cut[i][r].a);
	}
	pi lrR(int i,int l,int r){
		return pi(cut[i][l].b,cut[i][r].b);
	}
	//kth-smallest value in a
	t kth(int k){
		assert(inc(0,k,n-1));
		return st[k];
	}
	//value v 以上になる最初の id を返す
	int lwb_val(t v){
		return lwb(st,v);
	}
	//id of kth-smallest value in [l,r)
	int kth_id(int l,int r,int k){
		assert(0<=l&&l<=r&&r<=n);
		assert(inc(0,k,r-l-1));
		int i=1;
		while(i<s){
			auto [x,y]=lrL(i,l,r);
			auto [z,w]=lrR(i,l,r);
			if(y-x<=k){
				k-=y-x;
				i=i*2+1;
				l=z,r=w;
			}else{
				i=i*2;
				l=x,r=y;
			}
		}
		assert(k==0);
		return i-s;
	}
	//value of kth-smallest value in [l,r)
	t kth_val(int l,int r,int k){
		assert(0<=l&&l<=r&&r<=n);
		return kth(kth_id(l,r,k));
	}
	//positin [l,r) にある値のなかで，tar 以上の値の k 番目の id を返す
	//ないなら n
	int lwb_id(int l,int r,int tar,int k){
		assert(0<=l&&l<=r&&r<=n);
		assert(inc(0,tar,n));
		auto work=[&](int i,int b,int e)->int{
			{
				int v=e-b;
				if(v<=k){
					k-=v;
					return n;
				}
			}
			while(i<s){
				auto [x,y]=lrL(i,b,e);
				auto [z,w]=lrR(i,b,e);
				int v=y-x;
				if(v<=k){
					k-=v;
					i=i*2+1;
					b=z,e=w;
				}else{
					i=i*2;
					b=x,e=y;
				}
			}
			return i-s;
		};
		if(tar==0)return work(1,l,r);
		tar+=s-1;
		static pi buf[30];
		per(h,L){
			int i=tar>>(h+1);
			if((tar>>h)&1){
				tie(l,r)=lrR(i,l,r);
			}else{
				buf[h]=lrR(i,l,r);
				tie(l,r)=lrL(i,l,r);
			}
		}
		rep(h,L)if(((tar>>h)&1)==0){
			int i=(tar>>h)+1;
			tie(l,r)=buf[h];
			int res=work(i,l,r);
			if(res<n)return res;
		}
		return n;
	}
	//position [l,r) にある値のなかで，id が tar 未満の個数を返す
	int count_by_id(int l,int r,int tar){
		assert(0<=l&&l<=r&&r<=n);
		assert(inc(0,tar,n));
		if(tar==n)return r-l;
		
		tar+=s;
		int res=0;
		per(h,L){
			int i=tar>>(h+1);
			if((tar>>h)&1){
				auto [x,y]=lrL(i,l,r);
				res+=y-x;
				tie(l,r)=lrR(i,l,r);
			}else{
				tie(l,r)=lrL(i,l,r);
			}
		}
		
		return res;
	}
	//position [l,r) にある値のなかで，value が val 未満の個数を返す
	int count_by_val(int l,int r,t val){
		return count_by_id(l,r,lwb_val(val));
	}
	//position [l,r) にある値のなかで，value が [d,u) の個数を返す
	int count_val(int l,int r,t d,t u){
		return count_val(l,r,u)-count_val(l,r,d);
	}
	V sum_single(int i,int l,int r){
		assert(0<=l&&l<=r&&r<=sum.lens[i]);
		V res=0;
		for(int j=l-1;j>=0;j-=(j+1)&(-j-1))res-=sum[i][j];
		for(int j=r-1;j>=0;j-=(j+1)&(-j-1))res+=sum[i][j];
		return res;
	}
	//position [l,r) にある値のなかで，id が tar 未満の要素に紐付いた sum を返す
	V sum_by_id(int l,int r,int tar){
		assert(0<=l&&l<=r&&r<=n);
		assert(inc(0,tar,n));
		if(tar==n)return sum_single(1,l,r);
		
		tar+=s;
		V res=0;
		per(h,L){
			int i=tar>>(h+1);
			if((tar>>h)&1){
				auto [x,y]=lrL(i,l,r);
				res+=sum_single(i*2,x,y);
				tie(l,r)=lrR(i,l,r);
			}else{
				tie(l,r)=lrL(i,l,r);
			}
		}
		
		return res;
	}
	//position [l,r) にある値のなかで，value が val 未満の要素に紐付いた sum を返す
	V sum_by_val(int l,int r,t val){
		return sum_by_id(l,r,lwb_val(val));
	}
	//position [l,r) にある id が tar 以上の値の sum の k 番目に対応する id を出力 (ないなら n)
	//sum は全部非負を仮定
	int kth_id_by_sum(int l,int r,int tar,V k){
		assert(0<=l&&l<=r&&r<=n);
		assert(inc(0,tar,n));
		auto work=[&](int i,int b,int e)->int{
			{
				V v=sum_single(i,b,e);
				if(v<=k){
					k-=v;
					return n;
				}
			}
			while(i<s){
				auto [x,y]=lrL(i,b,e);
				auto [z,w]=lrR(i,b,e);
				V v=sum_single(i*2,x,y);
				if(v<=k){
					k-=v;
					i=i*2+1;
					b=z,e=w;
				}else{
					i=i*2;
					b=x,e=y;
				}
			}
			return i-s;
		};
		if(tar==0)return work(1,l,r);
		tar+=s-1;
		static pi buf[30];
		per(h,L){
			int i=tar>>(h+1);
			if((tar>>h)&1){
				tie(l,r)=lrR(i,l,r);
			}else{
				buf[h]=lrR(i,l,r);
				tie(l,r)=lrL(i,l,r);
			}
		}
		rep(h,L)if(((tar>>h)&1)==0){
			int i=(tar>>h)+1;
			tie(l,r)=buf[h];
			int res=work(i,l,r);
			if(res<n)return res;
		}
		return n;
	}
	void add_single(int i,int j,V v){
		for(;j<sum.lens[i];j+=(j+1)&(-j-1))sum[i][j]+=v;
	}
	//position p に v 足す
	void add(int p,V v){
		assert(inc(0,p,n-1));
		int i=1;
		add_single(i,p,v);
		while(i<s){
			auto [x,y]=cut[i][p];
			auto [z,w]=cut[i][p+1];
			if(x<z){
				i=i*2;
				p=x;
			}else{
				i=i*2+1;
				p=y;
			}
			add_single(i,p,v);
		}
	}
};
