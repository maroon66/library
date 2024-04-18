//stress-tested
template<class N>
struct slide_monoid{
	vc<N> a,b;
	int head,mid;
	slide_monoid(){clear();}
	void clear(){
		a.clear();
		b.clear();b.eb();
		head=0;
		mid=0;
	}
	void push(const N&x){
		a.pb(x);
		b.pb(N::merge(b.back(),x));
	}
	void pop(){
		if(++head>mid){
			b.back()=N();
			mid=si(b)-1;
			gnr(i,head,mid)
				b[i]=N::merge(a[i],b[i+1]);
		}
		assert(head<=mid);
	}
	N get(){
		return N::merge(b[head],b.back());
	}
};
