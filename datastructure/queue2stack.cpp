//queue<t> に対する操作を push_front, push_back, undo に分解する　
//UOJ 693
template<class t,class L,class R,class U>
struct queue2stack{
	L left;
	R right;
	U undo;
	vc<t> l,r;
	vi ks;
	queue2stack(L a,R b,U c):left(a),right(b),undo(c){}
	void le(t v){
		ks.pb(0);
		left(v);
	}
	void re(t v){
		ks.pb(1);
		right(v);
	}
	void push(t v){
		r.pb(v);
		re(v);
	}
	void pop(){
		if(l.empty()){
			rep(_,si(r))undo();
			l.swap(r);
			reverse(all(l));
			l.pop_back();
			for(auto v:l)le(v);
		}else{
			int n=si(l),m=si(r),tar=n&(n-1);
			while(tar<n){
				undo();
				int k=ks.back();ks.pop_back();
				if(k==0)n--;
				else m--;
			}
			l.pop_back();
			rng(i,m,si(r))re(r[i]);
			rng(i,n,si(l))le(l[i]);
		}
	}
};

template<class t,class L,class R,class U>
queue2stack<t,L,R,U> q2sgen(L a,R b,U c){
	return queue2stack<t,L,R,U>(a,b,c);
}

//usage
auto solver=q2sgen<int>(::merge,::merge,undo);

//https://codeforces.com/blog/entry/111117
//証明がわからない
//上とは違うが queue ver と同じ要領でできている気がする．
//LLLLRRRR -> RRRLLL みたいな操作をするんだけど，このときに priority 順に並べておけばいいという説がある．
//R の要素については，それより右にあるどの L よりも priority が低いみたいな状況が常に成立している
//計算量解析は queue のときと同じ．
//LR の並べ替えで sort するせいで計算量に余計な log がついている？見なかったことにしてくれマジで
//気合で log 消すこともできるが…
//Level K,K-1,K-2,...,-1 を用意する
//レベル -1 以外の各レベルでは priority が increasing になるようにしておく．
//各レベルごとに全部 L か全部 R みたいになっている
//push は普通に -1 レベルに突っ込むだけ
//pop の際は，まず -1 レベルを sort
//その後小さいレベルの方からマージソートを行う

//P:priority
//V:val
//F:upd
//G:undo
//stress-tested ただし計算量的に本当にあっているかは自信がない
//UOJ R23B (693) も通っているので多分大丈夫
template<class P,class V,class F,class G>
struct pqueue2stack{
	struct T{
		P p;
		V v;
		bool z;
		bool operator<(const T&r)const{
			return p<r.p;
		}
	};
	vc<T> x;
	F upd;
	G undo;
	bool always;
	int n,r,d;
	pqueue2stack(F f,G g,bool a=true):upd(f),undo(g),always(a),n(0),r(0),d(0){}
	pqueue2stack(const pqueue2stack&)=delete;
	pqueue2stack&operator=(const pqueue2stack&)=delete;
	void push(P p,V v){
		x.pb({p,v,false});
		if(always)prep();
	}
	void imerge(int i,int j,int k){
		if(i==j||j==k)return;
		static vc<T> y;
		y.resize(k-i);
		merge(x.bg+i,x.bg+j,x.bg+j,x.bg+k,y.bg);
		rng(a,i,k)x[a]=y[a-i];
	}
	void pop(){
		int tar=r?r&(-r):inf,mn=si(x)-1,head=mn;
		per(i,si(x)){
			if(x[i].z){
				x[i].z=false;
				tar--;
			}
			if(x[i]<x[mn])mn=i;
			else head=i;
			if(tar==0)break;
		}
		sort(x.bg+n,x.ed);
		int cut=si(x);
		gnr(i,head+1,si(x))if(x[i]<x[i-1]){
			imerge(i,cut,si(x));
			cut=i;
		}
		imerge(head,cut,si(x));
		rng(i,head,d)undo();
		chmin(d,head);
		x.pop_back();
		n=si(x);
		if(r==0){
			rep(i,n)x[i].z=true;
			r=n;
		}else{
			rng(i,n-(r&-r)+1,n)x[i].z=true;
			r--;
		}
		if(always)prep();
	}
	~pqueue2stack(){
		rep(i,d)undo();
	}
	void prep(){
		while(d<si(x))upd(x[d++].v);
	}
};
template<class P,class V,class F,class G>
pqueue2stack<P,V,F,G> pq2sgen(F f,G g,bool a=true){
	return pqueue2stack<P,V,F,G>(f,g,a);
}

//できることは時系列 segtree と同じだがメモリが線形
//https://codeforces.com/gym/100551/problem/A (A なし)
//https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum
template<class Q,class A>
struct offline_query{
	vi x,d;
	vc<Q> qs;
	vc<A> as;
	map<Q,int> idx;
	int add(Q q){
		int i=si(qs);
		x.pb(i);
		d.pb(inf);
		qs.pb(q);
		idx[q]=i;//del_by_key 使わないなら消す
		return i;
	}
	//delete the i-th added query
	void del(int i){
		d[i]=si(x);
		x.pb(-1);
	}
	void del_by_key(Q q){
		auto itr=idx.find(q);
		assert(itr!=idx.ed);
		del(itr->b);
		idx.erase(itr);
	}
	void ask(A a){
		int i=si(as);
		x.pb(-2-i);
		as.pb(a);
	}
	template<class F,class G,class H>
	void calc(F upd,G undo,H ask){
		auto solver=pq2sgen<int,Q>(upd,undo,false);
		for(auto i:x){
			if(i>=0)solver.push(-d[i],qs[i]);
			else if(i==-1)solver.pop();
			else{
				solver.prep();
				ask(as[-2-i]);
			}
		}
	}
};
