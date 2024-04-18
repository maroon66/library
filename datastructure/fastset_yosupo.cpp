//written by yosupo
// bit op
int popcnt(uint x) { return __builtin_popcount(x); }
int popcnt(ull x) { return __builtin_popcountll(x); }
int bsr(uint x) { return 31 - __builtin_clz(x); }
int bsr(ull x) { return 63 - __builtin_clzll(x); }
int bsf(uint x) { return __builtin_ctz(x); }
int bsf(ull x) { return __builtin_ctzll(x); }

struct FastSet {
    static constexpr uint B = 64;
    int n, lg;
    vvc<ull> seg;
    FastSet(int _n,ull val=0) : n(_n) {
        do {
            seg.push_back(vc<ull>((_n + B - 1) / B,val));
            _n = (_n + B - 1) / B;
        } while (_n > 1);
        lg = seg.size();
    }
    void fillone(){
		for(auto&vs:seg)fill(all(vs),ull(-1));
	}
	void fillzero(){
		for(auto&vs:seg)fill(all(vs),ull(0));
	}
    bool operator[](int i) const {
        return (seg[0][i / B] >> (i % B) & 1) != 0;
    }
    void set(int i) {
        for (int h = 0; h < lg; h++) {
            seg[h][i / B] |= 1ULL << (i % B);
            i /= B;
        }
    }
    void reset(int i) {
        for (int h = 0; h < lg; h++) {
            seg[h][i / B] &= ~(1ULL << (i % B));
            if (seg[h][i / B]) break;
            i /= B;
        }
    }
    // x以上最小の要素
    int next(int i) {
        for (int h = 0; h < lg; h++) {
            if (i / B == si(seg[h])) break;
            ull d = seg[h][i / B] >> (i % B);
            if (!d) {
                i = i / B + 1;
                continue;
            }
            // find
            i += bsf(d);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += bsf(seg[g][i / B]);
            }
            return i;
        }
        return n;
    }
    // x未満最大の要素
    int prev(int i) {
        i--;
        for (int h = 0; h < lg; h++) {
            if (i == -1) break;
            ull d = seg[h][i / B] << (63 - i % 64);
            if (!d) {
                i = i / B - 1;
                continue;
            }
            // find
            i += bsr(d) - (B - 1);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += bsr(seg[g][i / B]);
            }
            return i;
        }
        return -1;
    }
};

struct mexgetter{
	const int m;
	vi cnt;
	FastSet fs;
	//m 未満の値しか来ない
	mexgetter(int m_):m(m_),cnt(m),fs(m,-1){}
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
