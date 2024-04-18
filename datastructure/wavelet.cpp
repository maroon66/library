//#define WVSUM
//https://pjudge.ac/contest/895/problem/21619
//Bytecamp 2022 Day5 D
//ABC280H (t=pi, タイブレーク, stress-tested)
//pos と言ったときには元の数列での位置を意味する．
//id と言ったときにはソート済み列での位置を意味する．
template<class t>
struct Wavelet{
	const vc<t> a;
	const int n,L,s;
	vc<t> st;
	vi i2p;
	vec2d<int> pos;
	vec2d<pi> cut;
	#ifdef WVSUM
	vec2d<ll> sum;
	void presum(int len,int* b,ll* z){
		rep(i,len)z[i+1]=z[i]+a[b[i]];
	}
	#endif
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
	Wavelet(const vc<t>&aa):a(aa),n(si(a)),L(topbit(max<int>(n-1,1))+1),s(1<<L),
		st(n),i2p(n),pos(s*2,(L+1)*n),cut(s*2,L*n+s)
		#ifdef WVSUM
		,sum(s*2,(L+1)*n+s*2)
		#endif
	{
		{
			iota(all(i2p),0);
			stable_sort(all(i2p),[&](int i,int j){return a[i]<a[j];});
			rep(i,n)st[i]=a[i2p[i]];
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
		#ifdef WVSUM
		rng(i,1,s*2){
			sum.init(i,pos.lens[i]+1);
			presum(pos.lens[i],pos[i],sum[i]);
		}
		#endif
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
	int lwbval(t v){
		return lwb(st,v);
	}
	//id of kth-smallest value in [l,r)
	int kthid(int l,int r,int k){
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
	t kthval(int l,int r,int k){
		assert(0<=l&&l<=r&&r<=n);
		return kth(kthid(l,r,k));
	}
	#ifdef WVSUM
	//position [l,r) に対応する値を昇順に並べたとする
	//先頭 len 個の sum <= tar を満たす len のうちで最大
	//返り値
	//len
	//その len 個の sum
	//len+1 番目の値の id (ないなら S)
	tuple<int,ll,int> mxlen(int l,int r,ll tar){
		assert(0<=l&&l<=r&&r<=n);
		assert(tar>=0);
		if(sum[1][r]-sum[1][l]<=tar)return mt(r-l,sum[1][r]-sum[1][l],s);
		int len=0;
		ll cur=0;
		int i=1;
		while(i<s){
			auto [x,y]=lrL(i,l,r);
			auto [z,w]=lrR(i,l,r);
			if(cur+sum[i*2][y]-sum[i*2][x]<=tar){
				len+=y-x;
				cur+=sum[i*2][y]-sum[i*2][x];
				i=i*2+1;
				l=z,r=w;
			}else{
				i=i*2;
				l=x,r=y;
			}
		}
		return mt(len,cur,i-s);
	}
	#endif
	//positin [l,r) にある値のなかで，tar 以上の最小の id 返す
	//ないなら S
	int lwbid(int l,int r,int tar){
		assert(0<=l&&l<=r&&r<=n);
		assert(inc(0,tar,n));
		if(tar==n)return s;
		if(inc(l,i2p[tar],r-1))return tar;
		tar+=s;
		pi buf[30];
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
			if(l==r)continue;
			while(i<s){
				auto [x,y]=lrL(i,l,r);
				auto [z,w]=lrR(i,l,r);
				if(x==y){
					i=i*2+1;
					l=z,r=w;
				}else{
					i=i*2;
					l=x,r=y;
				}
			}
			return i-s;
		}
		return s;
	}
	//position [l,r) にある値のなかで，id が tar 未満の個数を返す
	int countid(int l,int r,int tar){
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
	int countval(int l,int r,t val){
		return countid(l,r,lwbval(val));
	}
	//position [l,r) にある値のなかで，value が [d,u) の個数を返す
	int countval(int l,int r,t d,t u){
		return countval(l,r,u)-countval(l,r,d);
	}
};
