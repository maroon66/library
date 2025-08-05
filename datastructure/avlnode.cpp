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

//サイズ付きの普通の set
//+has のキー
//CF CodeTon3-H
struct N{
	void pushl(N&cnode){push(cnode);}
	void pushr(N&cnode){push(cnode);}
	void updatel(const N&cnode){upd(cnode);}
	void updater(const N&cnode){upd(cnode);}
	void reverse(){}
	
	int len,val;
	bool has,sub;
	N():len(0),val(0),has(false),sub(false){}
	N(int v,bool h):len(1),val(v),has(h),sub(h){}
	void push(N&){}
	void clear(){}
	void single(){
		len=1;
		sub=has;
	}
	void upd(const N&x){
		len+=x.len;
		sub|=x.sub;
	}
	bool ok_len(int x){
		return len<=x;
	}
	bool ok_no_has(){
		return !sub;
	}
	bool operator<(const N&rhs)const{
		return val<rhs.val;
	}
};

//CF Teza Round 1
//sorted 列を持つ
//一次関数の足し込みが可能
//ソート順は降順です！！
struct N{
	void pushl(N&cnode){
		cnode.add(lza,lzb);
	}
	void pushr(N&cnode){
		cnode.add(lza,lza*(ls+1)+lzb);
	}
	void updatel(const N&cnode){
		len+=cnode.len;
		ls=cnode.len;
	}
	void updater(const N&cnode){
		len+=cnode.len;
	}
	void reverse(){assert(false);}
	
	int lza,lzb,len,ls,val;
	N():lza(0),lzb(0),len(0),ls(0),val(0){}
	N(int v):lza(0),lzb(0),len(1),ls(0),val(v){}
	void add(int a,int b){
		lza+=a;
		lzb+=b;
		val+=a*ls+b;
	}
	void clear(){
		lza=0;
		lzb=0;
	}
	void single(){
		assert(lza==0);
		assert(lzb==0);
		len=1;
		ls=0;
	}
	bool ok_len(int x){
		return len<=x;
	}
	bool operator<(const N&rhs)const{
		return val>rhs.val;
	}
};
