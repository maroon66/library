//ソートされてるとは限らない
//UCUP5-3-K
template<int K,pi none=pi(-inf,-inf)>
struct TopK{
	pi vs[K];
	TopK(){
		rep(k,K){
			vs[k]=none;
		}
	};
	void add(int c,int v){
		rep(k,K){
			if(vs[k].a==c){
				chmax(vs[k].b,v);
				return;
			}
		}
		rep(k,K){
			if(vs[k].b<v){
				swap(vs[k].a,c);
				swap(vs[k].b,v);
			}
		}
	}
	auto begin(){
		return vs;
	}
	auto end(){
		int i=0;
		while(i<K&&vs[i]!=none)i++;
		return vs+i;
	}
};