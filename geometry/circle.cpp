//cm,cr を pt,C に書き換えたあとコンパイルだけ通した，verify はしてない(バカ？)
//Contest 2: PKU Contest 1, PTZ Summer 2022 Day 4
struct C{
	pt c;
	ld r;
	pt eval(ld a){
		return c+pt::polar(r,a);
	}
	bool operator==(const C&rhs)const{
		return c==rhs.c&&sgn(r,rhs.r)==0;
	}
};
istream& operator>>(istream& is,C& c){
	return is>>c.c>>c.r;
}
ostream& operator<<(ostream& os,const C& c){
	return os<<"C{"<<c.c<<","<<c.r<<"}";
}

//AOJ0153
//0-no,1-edge,2-in
int cont(C a,pt b){
	return sgn(a.r-abs(b-a.c))+1;
}
//AOJ0153 円じゃなくて円盤
//0-no,1-touch,2-cross
int ids(C a,ln b){
	return sgn(a.r-dsp(b,a.c))+1;
}
//AOJ0129 (touch以外)
//0-no(in),1-touch(in),2-cross,3-touch(out),4-no(out)
int ics(C a,ln b){
	int c=ids(a,b);
	if(c<=1)return 4-c;
	return sgn(max(abs(b.a-a.c),abs(b.b-a.c))-a.r)+1;
}
//AOJ1183
//eval(b,t) が a と重なる t を c,d に入れる
//解の個数を返す
int ccl(C a,ln b,ld&c,ld&d){
	pt e=dir(b);
	pt f=b.a-a.c;
	return qeq(norm(e),2*dot(e,f),norm(f)-a.r*a.r,c,d);
}
//AOJ0023
//Contest 2: PKU Contest 1, PTZ Summer 2022 Day 4 (2,7)
//0-apart,1-coincide,2-a<b,3-a<=b,4-a>b,5-a>=b,6-a touch b,7-a cross b
int icc(C a,C b){
	ld c=abs(a.c-b.c);
	if(sgn(c)==0&&sgn(a.r-b.r)==0)return 1;
	int d=sgn(c+a.r-b.r);
	if(d<=0)return d+3;
	int e=sgn(c+b.r-a.r);
	if(e<=0)return e+5;
	int f=sgn(a.r+b.r-c);
	if(f>=0)return f+6;
	return 0;
}
//AOJ2572
//Contest 2: PKU Contest 1, PTZ Summer 2022 Day 4
//args of two intersections r,l seen by a.c
//assume two circles cross
pair<ld,ld> ccc(C a,C b){
	ld c=arg(b.c-a.c);
	ld d=arg(a.r,abs(b.c-a.c),b.r);
	return make_pair(c-d,c+d);
}
//XXI Opencup GP of Siberia 5
vc<pt> ccc_list(C a,C b){
	int v=icc(a,b);
	if(v==0||v==1||v==2||v==4)return {};
	auto z=ccc(a,b);
	if(v==3||v==5||v==6){
		return {a.c+pt::polar(a.r,z.a)};
	}else{
		return {a.c+pt::polar(a.r,z.a),a.c+pt::polar(a.r,z.b)};
	}
}
//Contest 2: PKU Contest 1, PTZ Summer 2022 Day 4
//単位円で偏角 0-x で y*(-dx) を積分した値
ld cutareaarg(ld x){
	return (x-sin(2*x)/2)/2;
}
//Contest 2: PKU Contest 1, PTZ Summer 2022 Day 4
ld cutareaarg(ld l,ld r){
	return cutareaarg(r)-cutareaarg(l);
}
//Contest 4, PTZ 2022 Winter Day 6 (ICPC Camp Day 1) J
ld cutareaarg(C c,ld l,ld r){
	l=normarg(l);
	r=normarg(r);
	return (l<=r?cutareaarg(l,r):cutareaarg(l,PI)+cutareaarg(-PI,r))
			*sq(c.r)-c.c.y*c.r*(cos(r)-cos(l));
}
//Osijek Day9 E
//a から c への接線
//a から見たときの arg
pair<ld,ld> tangent_args(C c,pt a){
	assert(!cont(c,a));
	pt dif=c.c-a;
	ld w=asin(max<ld>(-1,min<ld>(1,c.r/abs(dif))));
	ld k=arg(dif);
	return mp(normarg(k-w),normarg(k+w));
}
//円 a の偏角 b の位置で接する直線
ln tanln(C a,ld b){
	pt c=a.eval(b);
	return ln(c,c+pt::polar(1,b+PI/2));
}
//AOJ2201
//Contest 4, PTZ 2022 Winter Day 6 (ICPC Camp Day 1)
//a と b の共通外接線を引く
//a の接点の偏角を返す
pair<ld,ld> tangent_points_ext(C a,C b){
	pt dif=b.c-a.c;
	ld w=acos(max<ld>(-1,min<ld>(1,(a.r-b.r)/abs(dif))));
	ld k=arg(dif);
	return mp(normarg(k-w),normarg(k+w));
}
pair<ln,ln> tangent_line_ext(C a,C b){
	auto [p,q]=tangent_points_ext(a,b);
	return mp(tanln(a,p),tanln(a,q));
}
//AOJ2201
//内接線
pair<ld,ld> tangent_points_in(C a,C b){
	pt dif=b.c-a.c;
	ld w=acos(min<ld>(1,(a.r+b.r)/abs(dif)));
	ld k=arg(dif);
	return mp(normarg(k-w),normarg(k+w));
}
pair<ln,ln> tangent_line_in(C a,C b){
	auto [p,q]=tangent_points_in(a,b);
	return mp(tanln(a,p),tanln(a,q));
}
//AOJ2201(内接外接あたりの verify はできてない)
vc<ln> tangent_lines(C a,C b){
	vc<ln> res;
	int v=icc(a,b);
	if(v==0||v==3||v==5||v==6||v==7){//out
		auto [p,q]=tangent_line_ext(a,b);
		res.pb(p);
		if(v!=3&&v!=5)res.pb(q); //内接しているときは外接線は 1 つ
	}
	if(v==0||v==6){//in
		auto [p,q]=tangent_line_in(a,b);
		res.pb(p);
		if(v!=6)res.pb(q); //外接しているときは内接線は 1 つ
	}
	return res;
}

//極大な円だけ残す，重複も消す
//AOJ1047
//Contest 4, PTZ 2022 Winter Day 6 (ICPC Camp Day 1) J
//Contest 5, PTZ 2021 Summer Day 2, NAC 2021 B
vc<C> simplify_circle_sets(const vc<C>&rw){
	vc<C> cs;
	for(auto cur:rw){
		bool ok=true;
		rep(j,si(cs)){
			int w=icc(cs[j],cur);
			if(w==1||w==4||w==5){
				ok=false;
			}
		}
		if(ok){
			rep(j,si(cs)){
				int w=icc(cs[j],cur);
				if(w==1||w==2||w==3){
					cs.erase(cs.bg+j--);
				}
			}
			cs.pb(cur);
		}
	}
	return cs;
}

//極小な円だけ残す，重複も消す
//UCUP3-31-H
//verifyできているかは怪しい
vc<C> simplify_circle_sets(const vc<C>&rw){
	vc<C> cs;
	for(auto cur:rw){
		bool ok=true;
		rep(j,si(cs)){
			int w=icc(cs[j],cur);
			if(w==1||w==2||w==3){
				ok=false;
			}
		}
		if(ok){
			rep(j,si(cs)){
				int w=icc(cs[j],cur);
				if(w==1||w==4||w==5){
					cs.erase(cs.bg+j--);
				}
			}
			cs.pb(cur);
		}
	}
	return cs;
}

//UCUP3-31-H
//verifyできているかは怪しい
pair<vc<ld>,vi> range_freq_list(vc<pair<ld,ld>> lr){
	int cur=0;
	vc<pair<ld,int>> qs;
	for(auto [l,r]:lr){
		l=normarg_nonnegative(l);
		r=normarg_nonnegative(r);
		qs.eb(l,1);
		qs.eb(r,-1);
		if(l>r)cur++;
	}
	sort(all(qs));
	vc<ld> pos{0};
	vi val{cur};
	for(auto [p,v]:qs){
		pos.pb(p);
		val.pb(val.back()+v);
	}
	pos.pb(2*PI);
	return mp(pos,val);
}

//UCUP3-31-H
//verifyできているかは怪しい
ld common_area_cs(vc<C> cs){
	assert(si(cs));
	cs=simplify_circle_sets(cs);
	int n=si(cs);
	rep(i,n)rng(j,i+1,n){
		int w=icc(cs[i],cs[j]);
		if(w!=7)return 0;
	}
	if(n==1)return cs[0].area();
	ld ans=0;
	rep(i,n){
		vc<pair<ld,ld>> ls;
		rep(j,n)if(i!=j){
			ls.eb(ccc(cs[i],cs[j]));
		}
		auto [pos,val]=range_freq_list(ls);
		rep(j,si(val)){
			if(val[j]==n-1){
				ld l=pos[j],r=pos[j+1];
				ans+=cutareaarg(cs[i],l,r);
			}
		}
	}
	return ans;
}

//ByteCamp 2022 Day3 A
//原点中心,半径 r の円の x 座標 a 以下の領域の面積
ld cutarea_sub(ld r,ld a){
	ld theta=acos(max(min(a/r,ld(1)),ld(-1)));
	return sq(r)*(PI-theta)+a*r*sin(theta);
}
//2020 Multi-Uni Day5 J
//x座標 b 以上 c 以下の部分だけ取ってきて，その面積
ld cutarea(C a,ld b,ld c){
	chmax(b,a.c.x-a.r);
	chmin(c,a.c.x+a.r);
	b-=a.c.x;
	c-=a.c.x;
	if(sgn(c-b)<=0)return 0;
	return cutarea_sub(a.r,c)-cutarea_sub(a.r,b);
}

//AOJ2423
C circumc(pt a,pt b,pt c){
	b-=a;
	c-=a;
	pt r=c*norm(b)-b*norm(c);
	r=pt(r.y,-r.x)/(2*crs(b,c));
	return {a+r,abs(r)};
}
//AOJ2423
C mindisc(const vc<pt>& p,array<pt,3> q,int i,int j){
	if(i==int(p.size())){
		if(j==0)
			return {{0,0},-1};
		else if(j==1)
			return {q[0],0};
		else if(j==2)
			return {(q[0]+q[1])*ld(0.5),abs(q[0]-q[1])/2};
		else if(j==3)
			return circumc(q[0],q[1],q[2]);
		else
			assert(false);
	}
	C c=mindisc(p,q,i+1,j);
	if(sgn(abs(c.c-p[i])-c.r)==1){
		assert(j<3);
		q[j]=p[i];
		return mindisc(p,q,i+1,j+1);
	}else
		return c;
}
C mindisc(vc<pt> p){
	shuffle(all(p),mt19937());
	return mindisc(p,array<pt,3>(),0,0);
}

//CF773F
//p の中で k 点以上含むような円の最小半径

//https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_H&lang=ja
ld common_area_cp(C c,vc<pt> ps){
	ld ans=0;
	rep(i,si(ps)){
		pt a=ps[i],b=ps[(i+1)%si(ps)];
		ln z(a,b);
		ld d,e;
		int cnt=ccl(c,z,d,e);
		auto sector=[&](ld l,ld r){
			if(sgn(l,r)==0)return;
			l=arg(eval(z,l)-c.c);
			r=arg(eval(z,r)-c.c);
			ld dif=normarg(r-l);
			ans+=sq(c.r)*dif;
		};
		if(cnt<=1){
			sector(0,1);
		}else{
			chmax(d,0);
			chmin(e,1);
			if(d<=e){
				sector(0,d);
				sector(e,1);
				ans+=crs(c.c,eval(z,d),eval(z,e));
			}else{
				sector(0,1);
			}
		}
	}
	return ans/2;
}

//ABC314H
//偏角の区間があるので最大でどれだけ重なっているかを返す
int maxcommon(vc<pair<ld,ld>> lr){
	int cur=0;
	vc<pair<ld,int>> qs;
	for(auto [l,r]:lr){
		l=normarg(l);
		r=normarg(r);
		qs.eb(l,1);
		qs.eb(r,-1);
		if(l>r)cur++;
	}
	sort(all(qs));
	int ans=cur;
	for(auto [p,v]:qs){
		cur+=v;
		chmax(ans,cur);
	}
	return ans;
}


//Multiuni2023-10-E
//偏角 [0,2PI) で物事を管理
//polar(1,0) との dot 最大が ini
//偏角を増やしていって最大を取る円が変わるたびに change に記録されている
struct Hull{
	C ini;
	vc<pair<ld,C>> change;
};
Hull calc(vc<C> cs){
	int n=si(cs);
	if(n==1){
		Hull res;
		res.ini=cs[0];
		return res;
	}else{
		auto L=calc(vc<C>(cs.bg,cs.bg+n/2));
		auto R=calc(vc<C>(cs.bg+n/2,cs.ed));
		
		C a=L.ini,b=R.ini;
		vc<pair<ld,C>> res;
		{
			int i=0,j=0;
			ld cur=0;
			while(1){
				ld nx=2*PI;
				int which=-1;
				if(i<si(L.change)&&chmin(nx,L.change[i].a))which=0;
				if(j<si(R.change)&&chmin(nx,R.change[j].a))which=1;
				
				//偏角 [cur,nx) では a,b いずれかの円が突っ張っています
				//必要であればこの区間を分割してみよう！
				
				int t=icc(a,b);
				//only a
				if(t==1||t==4||t==5){
					res.eb(cur,a);
				}else if(t==2||t==3){
					res.eb(cur,b);
				}else{
					auto [u,v]=tangent_points_ext(a,b);
					ld A=normarg_nonnegative(v);
					ld B=normarg_nonnegative(u);
					
					bool flip=A>B;
					if(flip){
						swap(A,B);
						swap(a,b);
					}
					if(B<cur){
						res.eb(cur,b);
					}else if(A<cur){
						res.eb(cur,a);
						if(B<nx){
							res.eb(B,b);
						}
					}else{
						res.eb(cur,b);
						if(A<nx){
							res.eb(A,a);
							if(B<nx){
								res.eb(B,b);
							}
						}else{
							//do nothing
						}
					}
					if(flip){
						swap(A,B);
						swap(a,b);
					}
				}
				
				if(which==-1)break;
				else if(which==0)a=L.change[i++].b;
				else if(which==1)b=R.change[j++].b;
				else assert(0);
				cur=nx;
			}
		}
		int s=0;
		rep(i,si(res)){
			if(s>0&&res[s-1].b==res[i].b){
				continue;
			}
			res[s++]=res[i];
		}
		res.resize(s);
		C ini=res[0].b;
		res.erase(res.bg);
		return Hull{ini,res};
	}
}


//p が円の内部に入っているか
//座標の 4 乗オーダーの値が出てくる
//CGR26H (>=0 で円上もチェックできているつもりだが未確認)
bool incircle(pt a,pt b,pt c,pt p){
	if(ccw(a,b,c)<0)swap(b,c);
	assert(ccw(a,b,c)>0);
	__int128 a2=norm(a-=p),b2=norm(b-=p),c2=norm(c-=p);
	return a2*crs(b,c)+b2*crs(c,a)+c2*crs(a,b)>=0;
}
