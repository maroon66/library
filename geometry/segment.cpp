
//F は linecontainer_frac から引っ張ってこようね
//(x,y)->(x+y*eps,y) と変形してあると思い込む
//2 つの線分が占める x 座標の範囲に正の共通部分があるとする
//The 1st Universal Cup. Stage 15: Hangzhou J
struct S{
	//多分線分同士が端点以外で交わらないというケースだけ動くコード
	//線分に接するみたいな状況が発生すると話が変わってきてしまう
	pt a,b; //O(10^6)
	S(){}
	S(const pt&aa,const pt&bb):a(min(aa,bb)),b(max(aa,bb)){assert(a<b);}
	bool operator<(const S&r)const{
		if(a==r.a)return ccw(a,b,r.b)>0;
		else if(a<r.a)return ccw(a,b,r.a)>0;
		else return ccw(r.a,r.b,a)<0;
	}
	//Multiuni2023-10-I
	int cmp(const R&r)const{
		int ax=a.x*r.x.b-r.x.a; //O(10^12)
		int ay=a.y*r.y.b-r.y.a; //O(10^12)
		
		int bx=b.x*r.x.b-r.x.a; //O(10^12)
		int by=b.y*r.y.b-r.y.a; //O(10^12)
		
		__int128 res=__int128(ax)*by-__int128(bx)*ay>0;
		return res>0?-1:res==0?0:1;
	}
	bool operator<(const R&r)const{
		return cmp(r)<0;
	}
	/*F evaly(pt x)const{
		assert(a<=x&&x<=b);
		if(a.x==b.x)return F{x.y,1};
		else return xcut(a,b,x.x);
	}
	//値が 4 乗程度になりがち
	static int cmp(const S&a,const S&b,pt x){
		return F::cmp(a.evaly(x),b.evaly(x));
	}
	bool operator<(const pair<S,pt>&r)const{
		return cmp(*this,r.a,r.b)<0;
	}*/
};
