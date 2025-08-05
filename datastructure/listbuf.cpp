
//JOISC 2025 Migration
template<class t>
struct listbuf{
	struct N{
		int head,tail,len;
		N():head(-1),tail(-1),len(0){}
		N(int v):head(v),tail(v),len(1){}
	};
	vi nx;
	vc<t> vs;
	int used;
	listbuf(int s):nx(s,-1),vs(s),used(0){}
	N single(t val){
		vs[used]=val;
		return N(used++);
	}
	//a:=a+b
	//b:=empty
	void concat(N&a,N&b){
		if(a.len==0)return swap(a,b);
		if(b.len==0)return;
		a.len+=b.len;
		nx[a.tail]=b.head;
		a.tail=b.tail;
		b=N();
	}
	template<class F>
	void do_each(const N&a,F f){
		int v=a.head;
		rep(_,a.len){
			f(v,vs[v]);
			v=nx[v];
		}
	}
};
