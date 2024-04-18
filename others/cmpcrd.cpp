//XXII Opencup GP of Daejeon H
template<class T>
struct cmpcrd{
	using P=pair<T,int>;
	vc<P> buf;
	cmpcrd(const vc<T>&rw){
		buf.resize(si(rw));
		rep(i,si(rw))buf[i]=mp(rw[i],i);
		sort(all(buf));
	}
	int getid(const T&a){
		int i=lwb(buf,P(a,-1));
		assert(i<si(buf));
		assert(get<0>(buf[i])==a);
		return buf[i].b;
	}
};
