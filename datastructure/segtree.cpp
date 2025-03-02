//N() が単位元
//VERIFY: yosupo
//CF407E
template<class N>
struct segtree{
	vc<N> x;
	int n,s;
	segtree(){}
	template<class t>
	segtree(const vc<t>&a){
		n=a.size();
		s=1;
		while(s<n){s*=2;}
		x.resize(s*2);
		rep(i,n)
			x[s+i]=N(a[i]);
		gnr(i,1,s)
			x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	//NOT Verified
	segtree(int nn){
		resize(nn);
	}
	void resize(int nn){
		n=nn;
		s=1;
		while(s<n){s*=2;}
		x.assign(s*2,N());
		gnr(i,1,s)
			x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	template<class t>
	void init(const vc<t>&a){
		n=a.size();
		s=1;
		while(s<n){s*=2;}
		x.resize(s*2);
		rep(i,n)
			x[s+i]=N(a[i]);
		rng(i,n,s)
			x[s+i]=N();
		gnr(i,1,s)
			x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	void clear(){
		rep(i,n)
			x[s+i]=N();
		gnr(i,1,s)
			x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	N point_get(int i){
		assert(inc(0,i,n-1));
		return x[i+s];
	}
	void point_set(int i,const N&t){
		assert(inc(0,i,n-1));
		i+=s;
		x[i]=t;
		while(i>>=1)x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	void point_merge(int i,const N&t){
		assert(inc(0,i,n-1));
		i+=s;
		x[i]=N::merge(x[i],t);
		while(i>>=1)x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	template<class F,class...Args>
	void point_change(int i,F f,Args&&...args){
		assert(inc(0,i,n-1));
		i+=s;
		(x[i].*f)(forward<Args>(args)...);
		while(i>>=1)x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	N composite(int b,int e){
		assert(0<=b&&b<=e&&e<=n);
		N lf,rt;
		for(int l=b+s,r=e+s;l<r;l>>=1,r>>=1){
			if (l&1){
				lf=N::merge(lf,x[l]);
				l++;
			}
			if (r&1){
				r--;
				rt=N::merge(x[r],rt);
			}
		}
		return N::merge(lf,rt);
	}
	N getall(){
		return x[1];
	}
	//used in namori-bisect
	template <class F,class... Args> 
	pair<int,N> max_right_withinit(int l,N sm,F f,Args&&... args){
		assert((sm.*f)(forward<Args>(args)...));
		assert(0<=l&&l<=n);
		if(l==n)return mp(n,sm);
		l+=s;
		
		assert((sm.*f)(forward<Args>(args)...));
		do {
			while (l % 2 == 0) l >>= 1;
			if (!(N::merge(sm,x[l]).*f)(forward<Args>(args)...)){
				while (l < s) {
					l = (2 * l);
					N tmp=N::merge(sm,x[l]);
					if ((tmp.*f)(forward<Args>(args)...)) {
						sm = tmp;
						l++;
					}
				}
				return mp(l - s,sm);
			}
			sm = N::merge(sm, x[l]);
			l++;
		} while ((l & -l) != l);
		return mp(n,sm);
	}
	//UCUP-2-22-K
	template <class F,class... Args> 
	pair<int,N> min_left_withinit(int r,N sm,F f,Args&&... args){
		assert((sm.*f)(forward<Args>(args)...));
		assert(0<=r&&r<=n);
        if(r==0)return mp(0,sm);
        r+=s;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!(N::merge(x[r],sm).*f)(forward<Args>(args)...)) {
                while (r < s) {
                    r = (2 * r + 1);
                    N tmp=N::merge(x[r],sm);
                    if ((tmp.*f)(forward<Args>(args)...)) {
                        sm = tmp;
                        r--;
                    }
                }
                return mp(r + 1 - s,sm);
            }
            sm = N::merge(x[r], sm);
        } while ((r & -r) != r);
        return mp(0,sm);
    }
	//UTPC2020E
	//n 超えるかもしれない
	template <class F,class... Args> 
	pair<int,N> max_right(int l,F f,Args&&... args){
		return max_right_withinit(l,N(),f,forward<Args>(args)...);
	}
	//UTPC2020E
	template <class F,class... Args> 
	pair<int,N> min_left(int r,F f,Args&&... args){
		return min_left_withinit(r,N(),f,forward<Args>(args)...);
    }
    //行列とか乗せて必要なのはベクトルとの積，みたいなときに使える？
    //CF Goodbye 2016 E
    //CF 896 F
	template<class F,class T,class... Args>
	T accumulate(int l,int r,F f,T t,Args&&... args) {
		assert(0<=l&&l<=r&&r<=n);
		static int buf[2][30];
		int cnt[2]{};
		for(l+=s,r+=s;l<r;l>>=1,r>>=1){
			if(l&1)buf[0][cnt[0]++]=l;
			if(r&1)buf[1][cnt[1]++]=r-1;
			l++;
		}
		rep(i,cnt[0])t=(x[buf[0][i]].*f)(t,forward<Args>(args)...);
		per(i,cnt[1])t=(x[buf[1][i]].*f)(t,forward<Args>(args)...);
		return t;
	}
};
