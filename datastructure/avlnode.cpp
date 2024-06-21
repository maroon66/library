//range add, find by len, find by val
struct N{
	void pushl(N&cnode){push(cnode);}
	void pushr(N&cnode){push(cnode);}
	void updatel(const N&cnode){upd(cnode);}
	void updater(const N&cnode){upd(cnode);}
	void reverse(){}
	
	int lz,len,val;
	N():lz(0),len(0),val(0){}
	N(int v):lz(0),len(1),val(v){}
	void add(int v){
		lz+=v;
		val+=v;
	}
	void push(N&x){
		x.add(lz);
	}
	void clear(){
		lz=0;
	}
	void single(){
		assert(lz==0);
		len=1;
	}
	void upd(const N&x){
		assert(lz==0);
		len+=x.len;
	}
	bool ok_len(int x){
		return len<=x;
	}
	bool operator<(const N&rhs)const{
		return val<rhs.val;
	}
};
