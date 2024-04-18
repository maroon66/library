//やらないとは思うが，int=ll 状態で D=signed を入れると壊れるからね，注意してくれ
//AGC031E
//AGC034D
//CF621G
//負辺あったら壊れるからBellman-Fordとかやるといいんじゃない？
template<class D>
struct mincf{
	using P=pair<D,int>;
	struct E{int to,rev,cap;D cost;};
	int n;
	vvc<E> g;
	vc<D> h,d;
	vi pv,pe;
	mincf(int nn):n(nn),g(n),h(n),d(n),pv(n),pe(n){}
	pi ae(int a,int b,int cap,D cost){
		//dmp2(a,b,cap,cost);
		pi res(a,g[a].size());
		g[a].pb({b,(int)g[b].size(),cap,cost});
		g[b].pb({a,(int)g[a].size()-1,0,-cost});
		return res;
	}
	P go(int s,int t,int f){
		static vc<P> q;
		q.clear();
		fill(all(d),inf);
		d[s]=0;
		auto push=[&](D w,int v){
			q.eb(w,v);
			push_heap(all(q),greater<P>());
		};
		auto pop=[&](){
			pop_heap(all(q),greater<P>());
			auto res=q.back();
			q.pop_back();
			return res;
		};
		push(0,s);
		while(q.size()){
			D a;
			int v;
			tie(a,v)=pop();
			if(d[v]<a)continue;
			if(v==t)break;
			rep(i,(int)g[v].size()){
				E e=g[v][i];
				if(e.cap>0){
					D w=d[v]+e.cost+h[v]-h[e.to];
					if(w<d[e.to]){
						d[e.to]=w;
						push(w,e.to);
						pv[e.to]=v;
						pe[e.to]=i;
					}
				}
			}
		}
		if(d[t]==inf)return P(0,0);
		rep(i,n)
			h[i]=min<D>(h[i]+min(d[i],d[t]),inf);
		int a=f;
		for(int v=t;v!=s;v=pv[v])
			chmin(a,g[pv[v]][pe[v]].cap);
		for(int v=t;v!=s;v=pv[v]){
			E& e=g[pv[v]][pe[v]];
			e.cap-=a;
			g[e.to][e.rev].cap+=a;
		}
		return P(h[t],a);
	}
	pair<int,D> calc(int s,int t,int f){
		int a=0;
		D res=0;
		while(f>0){
			P w=go(s,t,f);
			if(w.b==0)break;
			a+=w.b;
			f-=w.b;
			res+=w.a*w.b;
		}
		return mp(a,res);
	}
	tuple<vi,vc<D>,vc<D>> get_slopes(int s,int t,int f){
		vi fs{0};
		vc<D> vs{0},sls;
		int a=0;
		D res=0;
		while(f>0){
			P w=go(s,t,f);
			if(w.b==0)break;
			sls.pb(w.a);
			if(w.b>=inf/2)break;
			a+=w.b;
			f-=w.b;
			res+=w.a*w.b;
			fs.pb(a);
			vs.pb(res);
		}
		return mt(fs,vs,sls);
	}
};
