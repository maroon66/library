int sgn(int a){
	return a>0?1:(a<0?-1:0);
}

int rabs(pi a){return max(abs(a.a),abs(a.b));}

int dot(pi a,pi b){
	return a.a*b.a+a.b*b.b;
}

pi add(pi a,pi b){
	return pi(a.a+b.a,a.b+b.b);
}
pi sub(pi a,pi b){
	return pi(a.a-b.a,a.b-b.b);
}

int crs(pi a,pi b){
	return a.a*b.b-a.b*b.a;
}

int ccw(pi a,pi b){
	return sgn(crs(a,b));
}

int ccw(pi a,pi b,pi c){
	return sgn(crs(sub(b,a),sub(c,a)));
}
//同じ x 座標には複数存在しないようにする
//stress-tested
//EC Final 2021 K (>=, no points on edges)
void add_convex_upper(vc<pi>&ps,pi v){
	if(si(ps)){
		assert(ps.back()<=v);
		if(ps.back().a==v.a)ps.pop_back();
	}
	while(si(ps)>=2&&ccw(ps[si(ps)-2],ps[si(ps)-1],v)>=0)ps.pop_back();
	ps.pb(v);
}
vc<pi> cat_convex_upper(vc<pi> a,const vc<pi>&b){
	for(auto v:b)add_convex_upper(a,v);
	return a;
}
//(a.a/a.b), (b.a/b.b) を比較
//分母が正のケースだけ stress-tested
int fcmp(pi a,pi b){
	if(a.b<0)a=-a;
	if(b.b<0)b=-b;
	int z=a.a*b.b-a.b*b.a;
	return z<0?-1:z==0?0:1;
}

//IOI2023 2B
int sgn(__int128_t a){
	return a>0?1:(a<0?-1:0);
}

__int128_t crs(pi a,pi b){
	return (__int128_t)a.a*b.b-(__int128_t)a.b*b.a;
}

int ccw(pi a,pi b){
	return sgn(crs(a,b));
}

int ccw(pi a,pi b,pi c){
	return sgn(crs(b-a,c-a));
}

//x 座標の昇順に追加しているとして，上に凸になるようにしている
void add_convex(vc<pi>&ps,pi v){
	while(si(ps)>=2&&ccw(ps[si(ps)-2],ps[si(ps)-1],v)>=0)ps.pop_back();
	ps.pb(v);
}
//IOI 2023 2B ここまで

int tri2(pi a,pi b,pi c){
	return abs(crs(sub(b,a),sub(c,a)));
}

int argtype(pi a){
	if(sgn(a.b)==0)return a.a<0?1:0;
	return a.b<0?0:1;
}
int argcmp(pi a,pi b){
	int at=argtype(a),bt=argtype(b);
	if(at!=bt)return at<bt?-1:1;
	return -ccw(a,b);
}
bool argless(pi a,pi b){
	return argcmp(a,b)<0;
}


//ここから下 verify してない
int dot(pi a,pi b){
	return a.a*b.a+a.b*b.b;
}

int norm(pi a){
	return sq(a.a)+sq(a.b);
}

int bet(pi a,pi b,pi c){
	pi d=sub(b,a);
	int e=dot(d,sub(c,a));
	if(sgn(e)<=0)return sgn(e)-1;
	return sgn(e-norm(d))+1;
}

//GCJ2021 R3C
//XXI Opencup GP of Nanjing F (oonedge=true)
//allow onedge vertices?
//同一直線上に並んでいると 0,N-1,N-2...2,1 となるよ
vc<pi> convex(vc<pi> s,bool onedge){
	swap(s[0],*min_element(all(s)));
	sort(s.bg+1,s.ed,[&](pi a,pi b){
		int c=ccw(s[0],a,b);
		if(c)return c==1;
		else return bet(s[0],a,b)==2;
	});
	{
		int j=si(s);
		while(j>1&&ccw(s[0],s[j-1],s.back())==0)j--;
		reverse(s.bg+j,s.ed);
	}
	vc<pi> t;
	rep(i,s.size()){
		int ss;
		while((ss=t.size())>=2){
			pi a=t[ss-2];
			pi b=t[ss-1];
			pi c=s[i];
			if(ccw(a,b,c)>=(onedge?0:1))
				break;
			t.pop_back();
		}
		t.pb(s[i]);
	}
	return t;
}
