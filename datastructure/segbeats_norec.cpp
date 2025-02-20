//merge で片方が inactive のときはもう片方をそのまま返す，
//といったときに，lazy の情報までコピーして渡さないようにする

//atcoder-library をまあまあコピーして使っている

//VERIFY:
//Japan Mirror Camp 2022 Day2 F
template<class N>
struct segbeats{
	vc<N> x;
	int L,s;
	segbeats(){}
	template<class T>
	segbeats(const vc<T>& a){
		int n=a.size();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	segbeats(int n){
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.assign(s*2,N());
		gnr(i,1,s)upd(i);
	}
	void upd(int i){
		N::merge(x[i*2],x[i*2+1],x[i]);
	}
	void push(int i){
		x[i].push(x[i*2],x[i*2+1]);
	}
	N composite(int l,int r){
		assert(0<=l&&l<r&&r<=s);
		
		l+=s;
		r+=s;
		
		for (int i = L; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		
		N sml[2],smr[2];
		int p=0,q=0;
		bool lnone=true,rnone=true;
		while (l < r) {
			if (l & 1){
				if(lnone)sml[0]=x[l++];
				else{
					N::merge(sml[p],x[l++],sml[p^1]);
					p^=1;
				}
				lnone=false;
			}
			if (r & 1){
				if(rnone)smr[0]=x[--r];
				else {
					N::merge(x[--r],smr[q],smr[q^1]);
					q^=1;
				}
				rnone=false;
			}
			l >>= 1;
			r >>= 1;
		}
		assert(!lnone||!rnone);
		if(lnone)return smr[q];
		else if(rnone)return sml[p];
		else{
			N::merge(sml[p],smr[q],sml[p^1]);
			return sml[p^1];
		}
	}//
	template<class F,class... Args>
	void ch_work(int i,F f,Args&&... args){
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
				push(i);
				i*=2;
			}
		}
	}
	template<class F,class... Args>
	void ch(int l, int r,F f,Args&&... args) {
		assert(0<=l&&l<=r&&r<=s);
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
		//rep(i,cnt[0])(x[buf[0][i]].*f)(forward<Args>(args)...);
		//per(i,cnt[1])(x[buf[1][i]].*f)(forward<Args>(args)...);
		rep(i,cnt[0])ch_work(buf[0][i],f,forward<Args>(args)...);
		per(i,cnt[1])ch_work(buf[1][i],f,forward<Args>(args)...);

		for (int i = 1; i <= L; i++) {
			if (((l >> i) << i) != l) upd(l >> i);
			if (((r >> i) << i) != r) upd((r - 1) >> i);
		}
	}
	N getall(){return x[1];}
	/*template <class F,class... Args> 
	pair<int,N> max_right(int l,F f,Args&&... args){
		assert(0<=l&&l<=s);
		if(l==s)return mp(s,N());
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
		return mp(s,sm);
	}
	//XXI Opencup Krakow M
	template <class F,class... Args> 
	pair<int,N> min_left(int r,F f,Args&&... args){
		assert(0<=r&&r<=s);
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
		assert(0 <= p && p < s);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		(x[p].*f)(forward<Args>(args)...);
		for (int i = 1; i <= L; i++) upd(p >> i);
	}
	N point_get(int p){
		assert(0 <= p && p < s);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		return x[p];
	}
	void point_set(int p,N val){
		assert(0 <= p && p < s);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		x[p]=val;
		for (int i = 1; i <= L; i++) upd(p >> i);
	}*/
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
};

//N::push
//pushしたあとはclearする
//N::merge
