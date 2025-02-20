//stress-test してません
vc<pt> lower_convex(const vc<pt>&ps,bool onedge){
	vc<pt> res;
	assert(is_sorted(all(ps)));
	for(auto p:ps){
		int s;
		while((s=si(res))>=2){
			if(ccw(res[s-2],res[s-1],p)<=(onedge?-1:0))res.pop_back();
			else break;
		}
		res.pb(p);
	}
	return res;
}
vc<pt> upper_convex(const vc<pt>&ps,bool onedge){
	vc<pt> res;
	assert(is_sorted(all(ps)));
	for(auto p:ps){
		int s;
		while((s=si(res))>=2){
			if(ccw(res[s-2],res[s-1],p)>=(onedge?1:0))res.pop_back();
			else break;
		}
		res.pb(p);
	}
	return res;
}
vc<pt> convex(vc<pt> ps,bool onedge){
	mkuni(ps);
	if(si(ps)<=1)return ps;
	auto lw=lower_convex(ps,onedge);
	auto up=upper_convex(ps,onedge);
	lw.pop_back();
	rein(up);
	up.pop_back();
	pb(lw,up);
	return lw;
}

//ICPC WF 2021 K
vc<pt> merge_convex(const vc<pt>&a,const vc<pt>&b){
	assert(a[0]==MIN(a));
	assert(b[0]==MIN(b));
	int n=si(a),m=si(b);
	if(n==1)return b+a[0];
	if(m==1)return a+b[0];
	auto dirtype=[&](pt dir){
		return dir.x==0?dir.y<0:dir.x<0;
	};
	vc<pt> c(n+m);
	int i=0,j=0;
	rep(step,n+m){
		c[step]=a[i%n]+b[j%m];
		bool usea=true;
		if(i==n){
			usea=false;
		}else if(j<m){
			pt u=a[(i+1)%n]-a[i];
			pt v=b[(j+1)%m]-b[j];
			int ut=dirtype(u),vt=dirtype(v);
			if(ut!=vt)usea=ut<vt;
			else usea=ccw(u,v)>0;
		}
		if(usea)i++;
		else j++;
	}
	return c;
}

//↓onedge=true で壊れていると話題に
//辺 n-1->0 のあたりがやばいはず
//copied from old version
//USACO 2022 January ptlatinum C
//allow onedge vertices
vc<pt> convex(vc<pt> s,bool onedge){
	sort(all(s));
	s.erase(unique(all(s)),s.ed);
	sort(s.bg+1,s.ed,[&](pt a,pt b){
		int c=ccw(s[0],a,b);
		if(c)return c==1;
		else{
			if(onedge&&sgn(s[0].x-a.x)==0)
				return a.y>b.y;
			return bet(s[0],a,b)==2;
		}
	});
	vc<pt> t;
	rep(i,s.size()){
		int ss;
		while((ss=t.size())>=2){
			pt a=t[ss-2];
			pt b=t[ss-1];
			pt c=s[i];
			if(ccw(a,b,c)>=(onedge?0:1))
				break;
			t.pop_back();
		}
		t.pb(s[i]);
	}
	return t;
}

//UCUP 2-3-J
//O(N^2)
ld width(vc<pt> ps){
	ps=convex(ps,false);
	ld ans=inf;
	rep(i,si(ps)){
		pt dir=normalized(rot90(ps[(i+1)%si(ps)]-ps[i]));
		ld mn=inf,mx=-inf;
		for(auto p:ps){
			ld v=dot(p,dir);
			chmin(mn,v);
			chmax(mx,v);
		}
		chmin(ans,mx-mn);
	}
	return ans;
}

//GCJ2019R3D
//0-out,1-edge,2-in
int cont(const vc<pt>&a,pt b){
	int n=a.size();
	int res=0;
	rep(i,n){
		pt c=a[i],d=a[(i+1)%n];
		if(ccw(c,d,b)==0&&abs(bet(c,d,b))<=1)
			return 1;
		if(c.y>d.y)swap(c,d);
		if(sgn(b.y-c.y)>0&&sgn(d.y-b.y)>=0&&ccw(c,d,b)==1)
			res^=1;
	}
	return res*2;
}

//UCUP3-4-H
bool intersect_polygon(const vc<pt>&a,const vc<pt>&b){
	for(auto v:a)if(cont(b,v))return true;
	for(auto v:b)if(cont(a,v))return true;
	rep(i,si(a)){
		ln c(a[i],a[(i+1)%si(a)]);
		rep(j,si(b)){
			ln d(b[j],b[(j+1)%si(b)]);
			if(iss(c,d))return true;
		}
	}
	return false;
}

//AOJ1283
//convex cut
//なんか 1 とか 2 頂点だけ残ることがある
//UCUP 2-15-F
vc<pt> ccut(const vc<pt>&a,ln b){
	int n=a.size();
	vc<pt> c;
	rep(i,n){
		int j=(i+1)%n;
		int d=ccw(b,a[i]),e=ccw(b,a[j]);
		if(d>=0)c.pb(a[i]);
		if(d*e<0)c.pb(cll(b,ln(a[i],a[j])));
	}
	return c;
}

//UCUP3-30-A
pt mass_center(const vc<pt>&a){
	int n=si(a);
	assert(n>0);
	if(n==1)return a[0];
	if(n==2)return (a[0]+a[1])/2;
	pt num;
	ld den=0;
	rep(i,n-2){
		ld s=crs(a[n-1],a[i],a[i+1]);
		num+=(a[n-1]+a[i]+a[i+1])/3*s;
		den+=s;
	}
	return num/den;
}

//線分が polygon の中に含まれているかどうか
//polygon の周に乗っている場合も OK
//ICPC Yokohama 2022 J
bool within_polygon(const vc<pt>&ps,ln z){
	int n=si(ps);
	bool ok=true;
	int cnt=0;
	auto add=[&](ld num,ld den){
		if(den<0){
			num=-num;
			den=-den;
		}
		if(num<=0){
			cnt++;
		}else if(num>=den){
		}else{
			ok=false;
		}
	};
	rep(i,n){
		pt p=ps[i],q=ps[(i+1)%n],r=ps[(i+n-1)%n];
		int a=ccw(z,p),b=ccw(z,q);
		if(a*b==-1){
			auto [num,den]=clltf(z,ln(p,q));
			add(num,den);
		}
		if(a==0){
			int c=ccw(z,r);
			if(b!=c&&(b*c<0||ccw(p,q,r)>0))
				add(dot(ps[i]-z.a,dir(z)),norm(dir(z)));
		}
	}
	return ok&&cnt%2==1;
}

bool is_simple_polygon(const vc<pt>&ps){
	int n=si(ps);
	rep(i,n)rng(j,i+1,n){
		ln a(ps[i],ps[i+1]);
		ln b(ps[j],ps[(j+1)%n]);
		int z=iss(a,b);
		//0-no,1-yes(endpoint),2-yes(innner),3-overelap
		//if the two line touch like this
		// x----x----x
		//it returns 1
		
		if(z==0){
			//ok
		}else if(z==1){
			if(j==i+1||(i==0&&j==n-1)){
				//ok
			}else{
				return false;
			}
		}else{
			return false;
		}
	}
	return true;
}
//UCUP 3-4-H
vc<pt> generete_simple_polygon(int n){
	while(1){
		const int vmax=10;
		set<pi>xy;
		while(si(xy)<n){
			int x=rand_int(-vmax,vmax);
			int y=rand_int(-vmax,vmax);
			xy.emplace(x,y);
		}
		vc<pt> ps;
		for(auto [x,y]:xy)
			ps.eb(x,y);
		myshuffle(ps);
		
		if(is_simple_polygon(ps)){
			int a=area2(ps);
			if(a<0){
				rein(ps);
			}
			return ps;
		}
	}
}
bool intersect_polygon(const vc<pt>&a,const vc<pt>&b){
	for(auto v:a)if(cont(b,v))return true;
	for(auto v:b)if(cont(a,v))return true;
	rep(i,si(a)){
		ln c(a[i],a[(i+1)%si(a)]);
		rep(j,si(b)){
			ln d(b[j],b[(j+1)%si(b)]);
			if(iss(c,d))return true;
		}
	}
	return false;
}

//壊れています！
//rabs やめてね
//int128!! (for cmpfrac)
//simple polygon と直線が与えられる
//simple polygon (周含む) と直線の交わる場所を列挙する
//有理数で返す
//ソート済み，2個ずつペア
//特に長さ 0 の部分に対しては l=r が入っている
//UCUP 2-Prime-43
//おい rabs はまずいだろ srabs に変更
vc<pi> overlap_ranges(const vc<pt>&ps,ln z){
	const int n=si(ps);
	vc<pi> res;
	rep(i,n){
		pt p=ps[i],q=ps[(i+1)%n],r=ps[(i+n-1)%n];
		int a=ccw(z,p),b=ccw(z,q);
		if(a*b==-1){
			res.pb(clltf(z,ln(p,q)));
		}
		if(a==0){
			pi u(srabs(ps[i]-z.a),srabs(dir(z)));
			int c=ccw(z,r);
			if(b!=c&&(b*c<0||ccw(p,q,r)>0))
				res.pb(u);
			if(b*c==1&&ccw(p,q,r)>0){
				res.pb(u);
				res.pb(u);
			}
		}
	}
	assert(si(res)%2==0);
	soin(res,[&](pi a,pi b){
		return cmpfrac(a.a,a.b,b.a,b.b)<0;
	});
	return res;
}

//ICPCWF2017 A
//simple polygon と直線の交点
//交点というのは， polygon 内部（周上含む）と外部が入れ替わる点
//z 上の座標 t のリストで返す
vc<ld> cplt(const vc<pt>&ps,ln z){
	int n=si(ps);
	vc<ld> pos;
	rep(i,n){
		pt p=ps[i],q=ps[(i+1)%n],r=ps[(i+n-1)%n];
		int a=ccw(z,p),b=ccw(z,q);
		if(a*b==-1)	pos.pb(cllt(z,ln(p,q)));
		if(a==0){
			int c=ccw(z,r);
			if(b!=c&&(b*c<0||ccw(p,q,r)>0))
				pos.pb(dot(ps[i]-z.a,dir(z))/norm(dir(z)));
		}
	}
	sort(all(pos));
	return pos;
}

//O(N)
//stress-tested
//allow onedge vertices?
vc<pt> upper_convex(const vc<pt>&ps,bool onedge){
	vc<pt> res;
	assert(is_sorted(all(ps)));
	for(auto p:ps){
		int s;
		while((s=si(res))>=2){
			if(ccw(res[s-2],res[s-1],p)>=(onedge?1:0))res.pop_back();
			else break;
		}
		res.pb(p);
	}
	return res;
}

vc<pt> halfpint(vc<ln> s){
	sort(all(s),[&](ln a,ln b){
		int c=argcmp(dir(a),dir(b));
		if(c)return c<0;
		return ccw(b,a.a)>0;
	});
	s.erase(unique(all(s),[&](ln a,ln b){
		return argcmp(dir(a),dir(b))==0;
	}),s.ed);
	int n=si(s);
	vi cur;
	rep(ii,n*2){
		int i=ii%n,m;
		while((m=si(cur))>=2){
			if(ccw(s[i],cll(s[cur[m-2]],s[cur[m-1]]))>0)break;
			cur.pop_back();
		}
		cur.pb(i);
	}
	vi cnt(n);
	for(auto i:cur)cnt[i]++;
	vc<ln> t;
	rep(i,n)if(cnt[i]==2)t.pb(s[i]);
	int m=si(t);
	vc<pt> res(m);
	rep(i,m)res[i]=cll(t[i],t[(i+1)%m]);
	return res;
}
//UCUP 3-15-M
vc<pt> common_convex(const vc<pt>&a,const vc<pt>&b){
	if(si(a)<=2||si(b)<=2)return {};
	vc<ln> ls;
	rep(i,si(a))ls.eb(a[i],a[(i+1)%si(a)]);
	rep(i,si(b))ls.eb(b[i],b[(i+1)%si(b)]);
	return halfpint(ls);
}

//Bytedance Camp 2022 Day1 F
//inside polygon

//Multiuni2023-3-C
//凸多角形を細分して，どの領域にあるかを求める
//正確に言うと，自分を含む領域の辺を返してくれる
//ドロネーとかと組み合わせるのもありかもしれない

//FHC 2022 Round3 E
//ll/long double 両方で verify
//convex polygon (convex である必要ない?) のリストが与えられる
//頂点は反時計周りに入っている　
//これらの polygon は交わることがない
//(x,y)から真上方向に光線を伸ばしたとき最初に当たる polygon の index を返す
//外側からあたった場合は i*2+1
//内側なら i*2
//polygon の各線分は，x 座標の半開区間に対応している
//故に，例えばある polygon がどの polygon の内側にあるかを知りたい場合は
//(x,y) の max の頂点で ask する
struct N{
	//多分線分同士が端点以外で交わらないというケースだけ動くコード
	//線分に接するみたいな状況が発生すると話が変わってきてしまう
	pt a,b;
	int j;
	N(const pt&aa,const pt&bb,int jj):a(aa),b(bb),j(jj){}
	bool operator<(const N&r)const{
		if(a==r.a)return ccw(a,b,r.b)>0;
		else if(sgn(a.x,r.a.x)<=0)return ccw(a,b,r.a)>0;
		else return ccw(r.a,r.b,a)<0;
	}
};
ostream& operator<<(ostream&os,const N&v){
	return os<<"{"<<v.a<<" "<<v.b<<"}";
}
vi vertical_ray(vvc<pt> cvs,vc<pt> ask){
	struct Q{
		ld x;
		int t,i;
		bool operator<(const Q&rhs)const{
			int z=sgn(x,rhs.x);
			if(z)return z<0;
			else return t<rhs.t;
		}
	};
	vc<Q> qs;
	vc<N> rw;
	rep(cid,si(cvs)){
		const auto&vs=cvs[cid];
		rep(vid,si(vs)){
			pt a=vs[vid],b=vs[(vid+1)%si(vs)];
			int j=cid*2;
			if(sgn(a.x,b.x)>0){
				swap(a,b);
			}else{
				j++;
			}
			if(sgn(a.x,b.x)<0){
				int i=si(rw);
				rw.eb(a,b,j);
				qs.pb({a.x,1,i});
				qs.pb({b.x,0,i});
			}
		}
	}
	using S=set<N>;
	S s;
	vc<S::iterator> itrs(si(rw),s.ed);
	int m=si(ask);
	vi ans(m);
	rep(i,m){
		qs.pb({ask[i].x,2,i});
	}
	sort(all(qs));
	for(const auto&w:qs){
		if(w.t==0){//del
			int i=w.i;
			s.erase(itrs[i]);
		}else if(w.t==1){//add
			int i=w.i;
			auto [itr,ok]=s.insert(rw[i]);
			itrs[i]=itr;
			assert(ok);
		}else if(w.t==2){
			int i=w.i;
			auto itr=s.lower_bound(N(ask[i],ask[i],-1));
			if(itr==s.ed)ans[i]=-1;
			else ans[i]=itr->j;
		}else assert(false);
	}
	return ans;
}
