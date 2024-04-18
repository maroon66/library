//stress-tested
pair<pt,pt> farthest_convex(const vc<pt>&ps){
	assert(si(ps)>=1);
	int n=si(ps);
	if(n==1)return mp(ps[0],ps[0]);
	ld mx=-1;
	int i=0,j=-1;
	rng(k,1,n)if(chmax(mx,norm(ps[i],ps[k])))j=k;
	int x=i,y=j;
	rep(_,n){
		int p=(i+1)%n,q=(j+1)%n;
		if(ccw(ps[p]-ps[i],ps[q]-ps[j])>0)j=q;
		else i=p;
		if(chmax(mx,norm(ps[i],ps[j]))){
			x=i;
			y=j;
		}
	}
	return mp(ps[x],ps[y]);
}

//NOT VERIFIED...

void sorted_to_convex(const vc<P>&ps,vc<P>&conv){
	assert(si(ps)>=1);
	conv.clear();
	if(si(ps)==1){
		conv.pb(ps[0]);
		return;
	}
	static vc<P> buf[2];
	rep(k,2)buf[k].clear();
	auto add=[&](int k,const P&p){
		while(si(buf[k])>=2){
			int s=si(buf[k]);
			if(ccw(buf[k][s-2],buf[k][s-1],p)>0)break;
			buf[k].pop_back();
		}
		buf[k].pb(p);
	};
	rep(i,si(ps))add(0,ps[i]);
	per(i,si(ps))add(1,ps[i]);
	rep(k,2)rep(i,si(buf[k])-1)conv.pb(buf[k][i]);
}

//assume sorted
pair<P,P> farthest(const vc<P>&ps){
	assert(is_sorted(all(ps)));
	static vc<P> conv;
	sorted_to_convex(ps,conv);
	return farthest_convex(conv);
}

//assume convex
//UCUP 2-14-C
//a,b の間の距離の最大値の2乗
ld maxdist(const vc<pt>&a,const vc<pt>&b){
	int n=si(a),m=si(b);
	if(n<=2||m<=2){
		ld res=0;
		for(auto p:a)for(auto q:b)chmax(res,norm(p,q));
		return res;
	}
	int i=MINi(a).b,j=MAXi(b).b;
	ld mx=norm(a[i],b[j]);
	rep(_,n+m){
		int p=(i+1)%n,q=(j+1)%m;
		if(ccw(a[p]-a[i],b[q]-b[j])>0)j=q;
		else i=p;
		chmax(mx,norm(a[i],b[j]));
	}
	return mx;
}
