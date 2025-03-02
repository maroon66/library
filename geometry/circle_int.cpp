//UCUP3-31-J
struct C{
	int x,y,r,f;
	void init(){
		input(x,y,r,f);
	}
	bool operator<(const C&rhs)const{
		return r>rhs.r;
	}
};

//円周が共通部分を持つかどうか
bool has_common(const C&a,const C&b){
	int d2=sq(a.x-b.x)+sq(a.y-b.y);
	return sq(a.r-b.r)<=d2&&d2<=sq(a.r+b.r);
}
//aがbをcontainするかどうか
bool contain(const C&a,const C&b){
	assert(a.r>=b.r);
	int d2=sq(a.x-b.x)+sq(a.y-b.y);
	return d2<=sq(a.r-b.r);
}
