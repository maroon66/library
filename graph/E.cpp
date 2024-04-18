struct E{
	int to,id;
	operator int()const{return to;}
};
ostream&operator<<(ostream&os,const E&e){
	return os<<"E{"<<e.to<<","<<e.id<<"}";
}

struct E{
	int to,cost;
	operator int()const{return to;}
};

struct Edge{
	int a,b,c;
	Edge(int aa,int bb,int cc):a(aa),b(bb),c(cc){}
	bool operator<(const Edge&e)const{
		return c<e.c;
	}
};
ostream&operator<<(ostream&os,const Edge&e){
	return os<<"Edge("<<e.a<<","<<e.b<<","<<e.c<<")";
}
