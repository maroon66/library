//original: kactl (https://github.com/kth-competitive-programming/kactl/blob/main/content/geometry/FastDelaunay.h)
//https://cp-algorithms.com/geometry/delaunay.html#quad-edge-data-structure
//stress-tested, FHC2021 Final E
const pt none(inf,inf);
struct Quad{
	using Q=Quad*;
	Q rot,onx=0;
	pt u=none;
	bool vis=false;
	pt&v(){return rev()->u;}
	Q&rev(){return rot->rot;}
	Q&opr(){return rot->onx->rot;}
	Q&lnx(){return rev()->opr();}
	Q&rnx(){return rev()->onx;}
} *head;
using Q=Quad::Q;//p が円の内部に strict に入っているか
bool incircle(pt a,pt b,pt c,pt p){//assume ccw(a,b,c)>0
	__int128 a2=norm(a-=p),b2=norm(b-=p),c2=norm(c-=p);
	return a2*crs(b,c)+b2*crs(c,a)+c2*crs(a,b)>0;
}//座標の 4 乗オーダーの値が出てくると話題に(sgn 禁止)
Q make(pt u,pt v){
	Q q=head?head:new Quad{new Quad{new Quad{new Quad{0}}}};
	head=q->onx;
	q->rev()->rev()=q;
	rep(i,4){
		q->onx=i%2==0?q:q->rev();
		q->u=i==0?u:i==2?v:none;
		q=q->rot;
	}
	return q;
}
void splice(Q a,Q b){
	swap(a->onx->rot->onx,b->onx->rot->onx);
	swap(a->onx,b->onx);
}
Q connect(Q a,Q b){
	Q q=make(a->v(),b->u);
	splice(q,a->lnx());
	splice(q->rev(),b);
	return q;
}
void del(Q q){
	splice(q,q->opr());
	splice(q->rev(),q->rev()->opr());
	rep(i,4){
		q->vis=false;
		q=q->rot;
	}
	q->onx=head; head=q;
}
vc<pt> delauney(const vc<pt>&ps,int type){// no duplicate points please...
	if(si(ps)==1)return {};
	assert(is_sorted(all(ps)));
	//多分だけど…
	//lf: 左下の点から反時計まわりに進む辺
	//rt: 右下の点から時計まわりに進む辺
	auto rec=[&](auto self,int l,int r)->pair<Q,Q>{
		if(r-l==2){
			Q a=make(ps[l],ps[l+1]);
			return {a,a->rev()};
		}else if(r-l==3){
			Q a=make(ps[l],ps[l+1]),b=make(ps[l+1],ps[l+2]);
			splice(a->rev(),b);
			int z=ccw(ps[l],ps[l+1],ps[l+2]);
			if(z==0) return {a,b->rev()};
			Q c=connect(b,a);
			if(z==1) return {a,b->rev()};
			else return {c->rev(),c};
		}else{
			auto [lf,a]=self(self,l,(l+r)/2);
			auto [b,rt]=self(self,(l+r)/2,r);
			while(1){if(ccw(b->u,a->u,a->v())>0)a=a->lnx();
				else if(ccw(a->u,b->u,b->v())<0)b=b->rnx();
				else break;	}
			Q k=connect(b->rev(),a);
			if(k->v()==lf->u)lf=k->rev();
			if(k->u==rt->u)rt=k;
			while(1){
				Q lc=k->rnx(),rc=k->opr();
				bool lv=ccw(k->v(),k->u,lc->v())>0;
				bool rv=ccw(k->v(),k->u,rc->v())>0;
				while(lv&&incircle(k->v(),k->u,lc->v(),lc->onx->v()))
					del((lc=lc->onx)->opr());
				while(rv&&incircle(k->v(),k->u,rc->v(),rc->opr()->v()))
					del((rc=rc->opr())->onx);
				if(!lv&&!rv)break;
				if(!lv||(rv&&incircle(lc->v(),lc->u,rc->u,rc->v())))
					k=connect(rc,k->rev());
				else
					k=connect(k->rev(),lc->rev());
			}
			return {lf,rt};
		}
	};
	auto [lf,rt]=rec(rec,0,si(ps));
	vc<Q> buf;
	auto dfs=[&](auto self,Q q,bool in)->void{
		if(q->vis)return;
		q->vis=true;
		buf.pb(q);
		self(self,q->lnx(),in);
		if(in)self(self,q->rev(),in);
	};
	dfs(dfs,rt,false);
	int s=si(buf);
	dfs(dfs,lf,true);
	vc<pt> res;
	if(type==0){//triangulation
		rng(i,s,si(buf))res.pb(buf[i]->u);
	}else if(type==1){//edge
		for(auto q:buf)if(q->u<q->v()){res.pb(q->u),res.eb(q->v());}
	}else assert(false);
	for(auto q:buf)if(q->vis)del(q);
	return res;
}

//各点について最も近い別の点の index を返す
//タイブレークは index の小さい方を優先
//stress-tested (UCUP 2023-2 I (TLE))
vi nearest_list(const vc<pt>&ps){
	int n=si(ps);assert(n>=2);
	auto vs=ps;mkuni(vs);
	vi pos(n);
	rep(i,n)pos[i]=lwb(vs,ps[i]);
	int m=si(vs);
	vi cand(m,-1),res(n,-1);
	per(i,n)cand[pos[i]]=i;
	rep(i,n){
		if(cand[pos[i]]!=i){
			res[i]=cand[pos[i]];
			if(res[cand[pos[i]]]==-1)res[cand[pos[i]]]=i;
		}
	}
	vi buf(m,-1);
	auto es=delauney(vs,1);
	for(int k=0;k<si(es);k+=2){
		pt a=es[k],b=es[k+1];
		ld val=norm(a,b);
		int i=lwb(vs,a);
		int j=lwb(vs,b);
		rep(_,2){
			if(buf[i]==-1)buf[i]=j;
			else{
				ld cur=norm(vs[i],vs[buf[i]]);
				if(val<cur||(val==cur&&cand[j]<cand[buf[i]]))
					buf[i]=j;
			}
			swap(i,j);
		}
	}
	rep(i,n)if(res[i]==-1)res[i]=cand[buf[pos[i]]];
	return res;
}
