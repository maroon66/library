//atcoder-library をまあまあコピーして使っている

//N() が単位元

//merge で片方が inactive のときはもう片方をそのまま返す，
//といったときに，lazy の情報までコピーして渡さないようにする

//クエリできる点が[0,s)だったのを[0,n)に変えた (UCUP1-21D)
//ch,max_rightは動くと思う
//ちゃんと test してないから assert とかが壊れたらごめん

//VERIFY:
//https://atcoder.jp/contests/practice2/tasks/practice2_k
template<class N,bool Beats=false>
struct seglazy{
	vc<N> x;
	int n,L,s;
	seglazy(){}
	template<class T>
	seglazy(const vc<T>& a){
		n=a.size();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	seglazy(int nn){
		n=nn;
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.assign(s*2,N());
		gnr(i,1,s)upd(i);
	}
	template<class t>
	void init(const vc<t>&a){
		n=a.size();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.resize(s*2);
		rep(i,n)
			x[s+i]=N(a[i]);
		rng(i,n,s)
			x[s+i]=N();
		gnr(i,1,s)
			x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	void upd(int i){
		x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	void push(int i){
		x[i].push(x[i*2],x[i*2+1]);
	}
	//空ノードなしで動く ver が segbeats_norec に書いてある
	N composite(int l,int r){
		assert(0<=l&&l<=r&&r<=n);
		if(l==r)return N();
		
		l+=s;
		r+=s;
		
		for (int i = L; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		
		N sml,smr;
		while (l < r) {
			if (l & 1) sml = N::merge(sml, x[l++]);
			if (r & 1) smr = N::merge(x[--r], smr);
			l >>= 1;
			r >>= 1;
		}

		return N::merge(sml, smr);
	}
	//[l,r)内の極大区間について f(i) を呼ぶ
	//UCUP3-26-L
	template<class F>
	void foreach_range(int l,int r,F f){
		assert(0<=l&&l<=r&&r<=n);
		if(l==r)return;
		
		l+=s;
		r+=s;
		
		for (int i = L; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		
		while (l < r) {
			if (l & 1) f(l++);
			if (r & 1) f(--r);
			l >>= 1;
			r >>= 1;
		}
	}
	//UCUP1-21 D
	//JSC2024FinalD
	template<class F,class... Args>
	void ch_beats(int i,F f,Args&&... args){
		int ini=i;
		while(1){
			if((x[i].*f)(forward<Args>(args)...)){
				while(i>ini&&(i&1)){
					i>>=1;
					upd(i);
				}
				if(i==ini)break;
				i++;
			}else{
				assert(i<s);
				push(i);
				i*=2;
			}
		}
	}
	template<class F,class... Args>
	void ch(int l, int r, F f,Args&&... args) {
		assert(0<=l&&l<=r&&r<=n);
		if (l == r) return;

		l+=s;
		r+=s;

		for (int i = L; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		
		static int buf[2][30];
		int cnt[2]{};
		{
			int l2 = l, r2 = r;
			while (l < r) {
				if (l & 1){
					//(x[l++].*f)(forward<Args>(args)...);
					buf[0][cnt[0]++]=l++;
				}
				if (r & 1){
					//(x[--r].*f)(forward<Args>(args)...);
					buf[1][cnt[1]++]=--r;
				}
				l >>= 1;
				r >>= 1;
			}
			l = l2;
			r = r2;
		}
		if constexpr(Beats){
			rep(i,cnt[0])ch_beats(buf[0][i],f,forward<Args>(args)...);
			per(i,cnt[1])ch_beats(buf[1][i],f,forward<Args>(args)...);
		}else{
			rep(i,cnt[0])(x[buf[0][i]].*f)(forward<Args>(args)...);
			per(i,cnt[1])(x[buf[1][i]].*f)(forward<Args>(args)...);
		}

		for (int i = 1; i <= L; i++) {
			if (((l >> i) << i) != l) upd(l >> i);
			if (((r >> i) << i) != r) upd((r - 1) >> i);
		}
	}
	template<class F,class... Args>
	void chall(F f,Args&&... args){
		if constexpr(Beats){
			ch_beats(1,f,forward<Args>(args)...);
		}else{
			(x[1].*f)(forward<Args>(args)...);
		}
	}
	N getall(){return x[1];}
	template <class F,class... Args> 
	pair<int,N> max_right(int l,F f,Args&&... args){
		assert(0<=l&&l<=n);
		if(l==n)return mp(n,N());
		l+=s;
		
		for (int i = L; i >= 1; i--) push(l >> i);
		N sm;
		assert((sm.*f)(forward<Args>(args)...));
		do {
			while (l % 2 == 0) l >>= 1;
			if (!(N::merge(sm,x[l]).*f)(forward<Args>(args)...)){
				while (l < s) {
					push(l);
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
	//XXI Opencup Krakow M
	template <class F,class... Args> 
	pair<int,N> min_left(int r,F f,Args&&... args){
		assert(0<=r&&r<=n);
        if(r==0)return mp(0,N());
        r+=s;
        for (int i = L; i >= 1; i--) push((r - 1) >> i);
        N sm;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!(N::merge(x[r],sm).*f)(forward<Args>(args)...)) {
                while (r < s) {
                    push(r);
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
	template<class F,class...Args>
	void point_change(int p,F f,Args&&...args){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		(x[p].*f)(forward<Args>(args)...);
		for (int i = 1; i <= L; i++) upd(p >> i);
	}
	void point_merge(int p,const N&t){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		x[p]=N::merge(x[p],t);
		for (int i = 1; i <= L; i++) upd(p >> i);
	}
	N point_get(int p){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		return x[p];
	}
	void point_set(int p,N val){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		x[p]=val;
		for (int i = 1; i <= L; i++) upd(p >> i);
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
		assert(0<=b&&b<=e&&e<=n);
		return enumerater(0,s,1,b,e,dst);
	}
};
