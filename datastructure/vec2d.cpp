//point2d_general,wavelet 等で使用
//構築後にメモリサイズを指定する使い方 (reserve) をつけた
//chronological_tree_onlyadd
//確保したメモリを0埋めするようにした
template<class t>
struct vec2d{
	int n,s,used;
	vc<t*> x;
	vi lens;
	unique_ptr<t[]> p;
	vec2d(int nn):n(nn),s(0),used(0),x(n,nullptr),lens(n),p(){}
	vec2d(int nn,int ss):n(nn),s(ss),used(0),x(n,nullptr),lens(n),p(new t[s]){
		memset((void*)p.get(),0,sizeof(t)*s);
	}
	void reserve(int ss){
		assert(!p);
		p.reset(new t[s=ss]);
		memset((void*)p.get(),0,sizeof(t)*s);
	}
	void init(int i,int len){
		assert(lens[i]==0);
		assert(len>=0);
		x[i]=p.get()+used;
		lens[i]=len;
		used+=len;
		assert(used<=s);
	}
	t* operator[](int i){
		assert(inc(0,i,n-1));
		return x[i];
	}
};
