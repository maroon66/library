//USACO 2022 February A
//k==0 (x,y),(x,z) (y<z)
//k==1 (x,y),(z,y) (x<z)
//endpoints inclusive
//同方向の線分が重なるようなケースは対応しておりません
struct Line{
	int k,x,y,z;
};
void lines_connected_components(vc<Line> ls,unionfind&uf,vi mapping){
	int n=si(ls);
	
	vi ys;
	for(auto w:ls){
		if(w.k==0){
			assert(w.y<w.z);
		}else if(w.k==1){
			assert(w.x<w.z);
			ys.pb(w.y);
		}else assert(0);
	}
	mkuni(ys);
	int len=si(ys);
	
	struct N{
		bool has;
		int idx;
	};
	int s=1,h=0;while(s<len){s*=2;h++;}
	vc<N> w(s*2,{false,-1});
	auto upd=[&](int i){
		assert(w[i].idx==-1);
		assert(i<s);
		w[i].has=w[i*2].has|w[i*2+1].has;
	};
	auto apply=[&](int i,int v){
		if(!w[i].has)return;
		if(w[i].idx==-1){
			w[i].idx=v;
		}else{
			uf.unite(w[i].idx,v);
		}
	};
	auto prop=[&](int i){
		assert(i<s);
		if(w[i].idx!=-1){
			assert(w[i].has);
			rep(k,2)apply(i*2+k,w[i].idx);
			w[i].idx=-1;
		}
	};
	
	struct Query{
		int x,t,l,r,i;
		bool operator<(const Query&rhs)const{
			return pi(x,t)<pi(rhs.x,rhs.t);
		}
	};
	vc<Query> qs;
	
	rep(i,n){
		if(ls[i].k==0){
			qs.pb({ls[i].x,0,lwb(ys,ls[i].y),lwb(ys,ls[i].z+1),mapping[i]});
		}else{
			qs.pb({ls[i].x,-1,lwb(ys,ls[i].y),-1,mapping[i]});
			qs.pb({ls[i].z,1,lwb(ys,ls[i].y),-1,mapping[i]});
		}
	}
	
	sort(all(qs));
	for(auto z:qs){
		if(z.t==0){
			for(int l=z.l+s,r=z.r+s;l<r;l>>=1,r>>=1){
				if (l&1){
					apply(l,z.i);
					l++;
				}
				if (r&1){
					r--;
					apply(r,z.i);
				}
			}
		}else if(z.t==-1){
			int pos=s+z.l;
			rep(i,h)prop(pos>>(h-i));
			w[pos]=N{true,-1};
			per(i,h)upd(pos>>(h-i));
		}else if(z.t==1){
			int pos=s+z.l;
			rep(i,h)prop(pos>>(h-i));
			if(w[pos].idx!=-1){
				uf.unite(w[pos].idx,z.i);
			}
			w[pos]={false,-1};
			per(i,h)upd(pos>>(h-i));
		}
	}
}
