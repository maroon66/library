//f(a,b) が true なら a を削除する
//less をわたすと slide max が取れる
template<class t,class u>
struct slide{
	vc<t> x;
	vi y;
	u f;
	int s,b,e,c,d;
	slide(u ff=u()):f(ff){init();}
	void init(){s=b=e=c=d=0;}
	void push(t a){
		s++;
		while(b<e&&f(x[e-1],a))e--;
		if(e==(int)x.size()){
			x.eb();
			y.eb();
		}
		x[e]=a;
		y[e++]=c++;
	}
	void pop(){s--;if(y[b]==d)b++;d++;}
	t get(){assert(s);return x[b];}
	bool has(){return b<e;}
	int size(){return s;}
};
