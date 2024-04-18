//CF707 F
//N.l,N.r が必要
template<class N>
struct reverse_deque{
	int org;
	bool fw;
	deque<N> d;
	reverse_deque():org(0),fw(true){}
	size_t size()const{return si(d);}
	void r2i(N&x)const{
		if(fw){
			x.l+=org;
			x.r+=org;
		}else{
			swap(x.l,x.r);
			x.l=org-x.l;
			x.r=org-x.r;
		}
	}
	void i2r(N&x)const{
		if(fw){
			x.l-=org;
			x.r-=org;
		}else{
			swap(x.l,x.r);
			x.l=org-x.l;
			x.r=org-x.r;
		}
	}
	void push_back(N x){
		r2i(x);
		if(fw)d.push_back(x);
		else d.push_front(x);
	}
	void pop_back(){
		assert(!empty());
		if(fw)d.pop_back();
		else d.pop_front();
	}
	const N back()const{
		N x=fw?d.back():d.front();
		i2r(x);
		return x;
	}
	bool empty()const{return d.empty();}
	//x -> x+v
	void shift(int v){
		if(fw) org-=v;
		else org+=v;
	}
	//x -> v-x
	void rev(int v){
		shift(-v);
		fw^=true;
	}
	void clear(){
		d.clear();
	}
};

struct N{
	int l,r,i;
};

//ARC149D
template<class N>
struct reverse_deque{
	bool fw;
	deque<N> d;
	reverse_deque():fw(true){}
	size_t size()const{return si(d);}
	void push_back(N x){
		if(fw)d.push_back(x);
		else d.push_front(x);
	}
	void pop_back(){
		assert(!empty());
		if(fw)d.pop_back();
		else d.pop_front();
	}
	void push_front(N x){
		if(fw)d.push_front(x);
		else d.push_back(x);
	}
	const N back()const{
		return fw?d.back():d.front();
	}
	bool empty()const{return d.empty();}
	void clear(){
		d.clear();
	}
	N& operator[](int i){
		return fw?d[i]:d[si(d)-1-i];
	}
	void reverse(){
		fw^=true;
	}
};
