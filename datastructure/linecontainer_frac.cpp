//using ld=ll;
//The 1st Universal Cup. Stage 15: Hangzhou J
struct F{
	ld a,b; //O(10^?),O(10^?)
	F(ld aa=0,ld bb=1):a(aa),b(bb){}
	F(pair<ld,ld> ab):F(ab.a,ab.b){}
	F operator-()const{return F{-a,b};}
	static int cmp(const F&a,const F&b){
		assert(a.b>=0);
		assert(b.b>=0);
		if(a.b==0&&b.b==0){
			assert(a.a!=0);
			assert(b.a!=0);
			return int(a.a>0)-int(b.a>0);
		}
		//using T=__int128;
		using T=ll;
		T c=(T)a.a*b.b;
		T d=(T)b.a*a.b;
		return c<d?-1:c==d?0:1;
	}
	//Multiuni2023-10-I (ld==ll)
	static int cmp(const F&a,ld b){
		assert(a.b>=0);
		//using T=__int128;
		using T=ll;
		T c=(T)a.a;
		T d=(T)b*a.b;
		return c<d?-1:c==d?0:1;
	}
	template<class RHS>
	bool operator<(const RHS&r)const{
		return cmp(*this,r)<0;
	}
	bool operator<=(const F&r)const{
		return cmp(*this,r)<=0;
	}
	bool operator>(const F&r)const{
		return cmp(*this,r)>0;
	}
	bool operator>=(const F&r)const{
		return cmp(*this,r)>=0;
	}
	bool operator==(const F&r)const{
		return cmp(*this,r)==0;
	}
	bool operator!=(const F&r)const{
		return cmp(*this,r)!=0;
	}
};
const F Finf{1,0};

//Multiuni2023-10-I (ld==ll)
//ライブラリが馬鹿なのでpt<frac>がない
//のでこちらでちまちま頑張るなどの作業を…
struct R{
	F x,y;
	void init(){
		int a,b,c,d;cin>>a>>b>>c>>d;
		x=F(a,b);
		y=F(c,d);
	}
	bool operator==(const R&r)const{
		return x==r.x&&y==r.y;
	}
	bool operator<(const R&r)const{
		int z=F::cmp(x,r.x);
		if(z)return z<0;
		else return y<r.y;
	}
};

//max hull
//有理数で全てを計算する
//The 1st Universal Cup. Stage 15: Hangzhou J

//Author: Simon Lindholm
//https://github.com/kth-competitive-programming/kactl/blob/master/content/data-structures/LineContainer.h
struct Line {
	mutable ld a,b;
	mutable F p;
	bool operator<(const Line& o) const { return a < o.a; }
	bool operator<(F x) const { return p<x;}
};
struct LineContainer:multiset<Line,less<>>{
	F div(ld a,ld b){return b<0?F{-a,-b}:F{a,b};}
	bool isect(iterator x, iterator y) {
		if (y == end()) { x->p = Finf; return false; }
		if (x->a == y->a) x->p = x->b > y->b ? Finf : -Finf;
		else x->p = div(y->b - x->b, x->a - y->a);
		return x->p>=y->p;
	}
	void add(ld a, ld b) {
		auto z = insert({a, b, F()}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	//x の分母が非負でないと壊れるだろうね
	//例えば　(a,b) を add して，(pa+qb) の max を取りたい，みたいなクエリを考える
	//q>=0 ならば p/q をqueryすることで解決
	//幾何的にいうと，(a,b) という点を add
	//ベクトル (p,q) (q>=0) を与えて内積 max を query
	ld query(F x){
		assert(x.b>=0);
		//assert(!empty());
		if(empty())return -inf;
		auto l = *lower_bound(x);
		return l.a * x.a + l.b * x.b;
	}
};
