//JOISC2024 escape2
template<class N>
struct reverse_vector{
	vc<N> d;
	reverse_vector(){}
	size_t size()const{
		return si(d);
	}
	void push_front(N x){
		d.pb(move(x));
	}
	bool empty()const{
		return d.empty();
	}
	void clear(){
		d.clear();
	}
	N& operator[](int i){
		return d[si(d)-1-i];
	}
	friend ostream&operator<<(ostream&os,const reverse_vector&rv){
		return os<<reout(rv.d);
	}
};

using P=pair<reverse_vector<int>,int>;
void mg(P&a,P&b){
	if(si(a.a)<si(b.a))
		swap(a,b);
	rep(i,si(b.a))chmin(a.a[i],b.a[i]+b.b-a.b);
}
void Ppush(P&a,int v){
	a.a.push_front(v-a.b);
}
void Padd(P&a,int v){
	a.b+=v;
}
