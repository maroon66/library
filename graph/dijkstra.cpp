//dist の初期値が long long になってる
template<class E,class D=ll>
pair<vc<D>,vi> dijkstra(const vvc<E>& g,int s){
	const int n=g.size();
	using P=pair<D,int>;
	priority_queue<P,vc<P>,greater<P>> pq;
	vc<D> dist(n,infLL);
	vi from(n,-1);
	const auto ar=[&](int v,D d,int f){
		if(chmin(dist[v],d)){
			pq.emplace(d,v);
			from[v]=f;
		}
	};
	ar(s,0,-1);
	while(pq.size()){
		auto [d,v]=pq.top();pq.pop();
		if(dist[v]!=d)continue;
		for(auto e:g[v])
			ar(e.to,d+e.cost,v);
	}
	return mp(dist,from);
}

struct E{
	int to,cost;
	operator int()const{return to;}
};


//ini length N, initial length
//second: prev
/*
template<class E,class D=ll>
pair<vc<D>,vi> dijkstra(const vvc<E>& g,vi ini){
	const int n=g.size();
	using P=pair<D,int>;
	priority_queue<P,vc<P>,greater<P>> pq;
	vc<D> dist(n,infLL);
	vi from(n,-1);
	const auto ar=[&](int v,D d,int f){
		if(dist[v]>d){
			dist[v]=d;
			pq.emplace(d,v);
			from[v]=f;
		}
	};
	rep(i,n)ar(i,ini[i],-1);
	while(pq.size()){
		D d;
		int v;
		tie(d,v)=pq.top();pq.pop();
		if(dist[v]!=d)continue;
		for(auto e:g[v])
			ar(e.to,d+e.cost,v);
	}
	return mp(dist,from);
}
*/

//ini length N, initial length
//second: src
template<class E,class D=ll>
pair<vc<D>,vi> dijkstra(const vvc<E>& g,vi ini){
	const int n=g.size();
	using P=pair<D,int>;
	priority_queue<P,vc<P>,greater<P>> pq;
	vc<D> dist(n,infLL);
	vi from(n,-1);
	const auto ar=[&](int v,D d,int f){
		if(dist[v]>d){
			dist[v]=d;
			pq.emplace(d,v);
			from[v]=f;
		}
	};
	rep(i,n)ar(i,ini[i],i);
	while(pq.size()){
		D d;
		int v;
		tie(d,v)=pq.top();pq.pop();
		if(dist[v]!=d)continue;
		for(auto e:g[v])
			ar(e.to,d+e.cost,from[v]);
	}
	return mp(dist,from);
}

//距離だけ
//dist の初期値が numeric_limits<D>::max() になっている
//inf ではない
template<class E,class D=ll>
vc<D> dijkstra(const vvc<E>& g,int s){
	const int n=g.size();
	using P=pair<D,int>;
	priority_queue<P,vc<P>,greater<P>> pq;
	vc<D> dist(n,numeric_limits<D>::max());
	const auto ar=[&](int v,D d){
		if(dist[v]>d){
			dist[v]=d;
			pq.emplace(d,v);
		}
	};
	ar(s,0);
	while(pq.size()){
		D d;
		int v;
		tie(d,v)=pq.top();pq.pop();
		if(dist[v]!=d)continue;
		for(auto e:g[v])
			ar(e.to,d+e.cost);
	}
	return dist;
}

//dist の初期値が long long になってる
//使った辺ももらえる
template<class E,class D=ll>
tuple<vc<D>,vi,vc<E>> dijkstra(const vvc<E>& g,int s){
	const int n=g.size();
	using P=pair<D,int>;
	priority_queue<P,vc<P>,greater<P>> pq;
	vc<D> dist(n,infLL);
	vi from(n,-1);
	vc<E> use(n);
	const auto ar=[&](int v,D d,int f,E e){
		if(chmin(dist[v],d)){
			pq.emplace(d,v);
			from[v]=f;
			use[v]=e;
		}
	};
	ar(s,0,-1,E{});
	while(pq.size()){
		auto [d,v]=pq.top();pq.pop();
		if(dist[v]!=d)continue;
		for(auto e:g[v])
			ar(e.to,d+e.cost,v,e);
	}
	return mt(dist,from,use);
}

struct E{
	int to,cost,idx;
	operator int()const{return to;}
};
