//point2d_general,wavelet 等で使用
template<class t>
struct vec2d{
	int n,s,used;
	vc<t*> x;
	vi lens;
	unique_ptr<t[]> p;
	vec2d(int nn,int ss):n(nn),s(ss),used(0),x(n,nullptr),lens(n),p(new t[s]){}
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
