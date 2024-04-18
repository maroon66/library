//XXII Opencup GP of Daejeon
template<class N>
struct point2d{
	vc<pi> xy,yx;
	const int n,s,L;
	vc<pi> uni(vc<pi> rw){
		mkuni(rw);
		return rw;
	}
	vec2d<N> a;
	vec2d<pi> cut;
	point2d(const vc<pi>&rw):xy(uni(rw)),n(si(xy)),s(minp2(n)),L(topbit(s)),
		a(s*2,s*(L+1)*2),cut(s,L*n+s){
		yx=xy;
		for(auto&[x,y]:yx)swap(x,y);
		sort(all(yx));
		rng(i,1,s*2){
			int lv=topbit(i);
			int w=s>>lv;
			a.init(i,w*2);
		}
		vi cnt(2*s);
		rng(i,1,s){
			int lv=topbit(i);
			int w=s>>lv;
			int l=w*(i&mask(lv)),r=l+w;
			cut.init(i,max<int>(min(r,n)-l,0)+1);
			cut[i][0]=pi(0,0);
		}
		for(auto [y,x]:yx){
			int i=lwb(xy,pi(x,y))+s;
			while(i){
				cnt[i]++;
				if(i<s)cut[i][cnt[i]]=pi(cnt[i*2],cnt[i*2+1]);
				i>>=1;
			}
		}
	}
	void point_set_single(int i,int w,int j,const N&v){
		j+=w;
		a[i][j]=v;
		while(j>>=1)a[i][j]=N::merge(a[i][j*2],a[i][j*2+1]);
	}
	void point_set(pi p,const N&v){
		int i=1,j=lwb(yx,pi(p.b,p.a)),l=0,r=s;
		rep(lv,L+1){
			point_set_single(i,r-l,j,v);
			if(lv<L){
				int m=(l+r)/2;
				if(m<=n&&xy[m-1]<p){
					l=m;
					j=cut[i][j].b;
					i=i*2+1;
				}else{
					r=m;
					j=cut[i][j].a;
					i=i*2;
				}
			}
		}
	}
	N ask_single(int i,int w,int l,int r){
		N res;
		for(l+=w,r+=w;l<r;l>>=1,r>>=1){
			if(l&1)res=N::merge(res,a[i][l]);
			if(r&1)res=N::merge(res,a[i][r-1]);
			l++;
		}
		return res;
	}
	N askdfs(int i,int l,int r,int xl,int xr,int yl,int yr){
		if(xr<=l||r<=xl)return N();
		if(xl<=l&&r<=xr)return ask_single(i,r-l,yl,yr);
		return N::merge(
			askdfs(i*2+0,l,(l+r)/2,xl,xr,cut[i][yl].a,cut[i][yr].a),
			askdfs(i*2+1,(l+r)/2,r,xl,xr,cut[i][yl].b,cut[i][yr].b)
			);
	}
	N ask(int xl,int xr,int yl,int yr){
		xl=lwb(xy,pi(xl,-inf));
		xr=lwb(xy,pi(xr,-inf));
		yl=lwb(yx,pi(yl,-inf));
		yr=lwb(yx,pi(yr,-inf));
		return askdfs(1,0,s,xl,xr,yl,yr);
	}
};
