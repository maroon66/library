/*using ld=ll;
const ld eps=1e-9;
int sgn(ld a){return a<-eps?-1:(a>eps?1:0);}
int sgn(ld a,ld b){return sgn(a-b);}*/

//ll で verify (TTPC2022 N)
//frac<ll>約分なしで verify (ABC301)
struct pt{
	ld x,y,z;
	pt():x(0),y(0),z(0){}
	pt(ld xx,ld yy,ld zz):x(xx),y(yy),z(zz){}
	pt operator-()const{return pt(-x,-y,-z);}
	pt&operator+=(pt r){x+=r.x;y+=r.y;z+=r.z;return *this;}
	pt&operator-=(pt r){x-=r.x;y-=r.y;z-=r.z;return *this;}
	pt&operator*=(ld r){x*=r;y*=r;z*=r;return *this;}
	pt&operator/=(ld r){x/=r;y/=r;z/=r;return *this;}
	pt operator+(pt r)const{return pt(*this)+=r;}
	pt operator-(pt r)const{return pt(*this)-=r;}
	pt operator*(ld r)const{return pt(*this)*=r;}
	pt operator/(ld r)const{return pt(*this)/=r;}
	//ld length(){return sqrt(length2());}
	ld length2(){return sq(x)+sq(y)+sq(z);}
	int cmp(const pt&a,const pt&b)const{
		int sx=sgn(a.x-b.x),sy=sgn(a.y-b.y),sz=sgn(a.z-b.z);
		return sx?sx:sy?sy:sz;
	}
	bool operator<(pt r)const{return cmp(*this,r)<0;}
	bool operator==(pt r)const{return cmp(*this,r)==0;}
};
ostream& operator<<(ostream&os, pt a){
	return os<<"("<<a.x<<","<<a.y<<","<<a.z<<")";
}
istream& operator>>(istream&is, pt&a){
	return is>>a.x>>a.y>>a.z;
}
ld dot(pt a,pt b){
	return a.x*b.x+a.y*b.y+a.z*b.z;
}
ld norm(pt a){
	return dot(a,a);
}
ld norm(pt a,pt b){
	return norm(a-b);
}
bool is0(pt a){
	return sgn(a.x)==0&&sgn(a.y)==0&&sgn(a.z)==0;
}
pt crs(pt a,pt b){
	return pt(
		a.y*b.z-a.z*b.y,
		a.z*b.x-a.x*b.z,
		a.x*b.y-a.y*b.x
	);
}
pt crs(pt a,pt b,pt c){
	return crs(b-a,c-a);
}
ld det(pt a,pt b,pt c){
	return dot(crs(a,b),c);
}
//vol>0 -> a から見て b,c,d が時計まわり
ld det(pt a,pt b,pt c,pt d){
	return det(b-a,c-a,d-a);
}
//TTPC2022 N
using T=tuple<pt,pt,pt>;
//面を返す; 全部三角形
//外側から見たとき，反時計まわりに列挙
vc<T> convex(vc<pt> ps){
	vc<pt> ls;
	for(auto p:ps){//同一平面上にない 4 点を見つける
		if(si(ls)<=1){
			ls.pb(p);//assume no duplicate points
		}else if(si(ls)==2){
			if(!is0(crs(ls[0],ls[1],p)))ls.pb(p);
		}else if(si(ls)==3){
			if(sgn(det(ls[0],ls[1],ls[2],p)))ls.pb(p);
		}
	}
	assert(si(ls)==4);
	vc<T> res;
	rep(i,4)rng(j,i+1,4)rng(k,j+1,4){
		pt a=ls[i],b=ls[j],c=ls[k],d=ls[6-i-j-k];
		if(det(d,a,b,c)<0)swap(a,b);
		res.eb(a,b,c);
	}
	for(auto p:ps){
		vc<T> nx;
		vc<pair<pt,pt>> del;
		for(auto [a,b,c]:res){
			if(sgn(det(p,a,b,c))<0){
				del.eb(a,b);
				del.eb(b,c);
				del.eb(c,a);
			}else{
				nx.eb(a,b,c);
			}
		}
		sort(all(del));
		for(auto [a,b]:del)
			if(!binary_search(all(del),mp(b,a)))
				nx.eb(a,b,p);
		res.swap(nx);
	}
	return res;
}
//TTPC2022 ここまで


//KUPC2018 L
//ld=ll だけ
//直線を方向で分類する
//for frac 約分なし ABC301G
/*
pt normdir(pt a){
	assert(a.x.b==a.y.b&&a.x.b==a.y.b);
	int g=gcd(gcd(a.x.a,a.y.a),a.z.a);
	return pt(a.x.a/g,a.y.a/g,a.z.a/g);
}*/
ld maxaxis(pt a){
	ld ax=abs(a.x),ay=abs(a.y),az=abs(a.z);
	if(ax>=ay&&ax>=az)return a.x;
	else if(ay>=az)return a.y;
	else return a.z;
}
pt normdir(pt a){
	if(is0(a))return a;
	int g=gcd(gcd(a.x,a.y),a.z);
	return a/g;
}
//方向でさらに +- の区別をつけない
pt normdirabs(pt a){
	a=normdir(a);
	return max(a,-a);
}
/*
//三角形 a-b-c の法線を返す
//単位ベクトル
pt getnorm(pt a,pt b,pt c){
	pt res=crs(b-a,c-a);
	res/=res.length();
	return res;
}*/
//ABC301G
bool sameline(pt a,pt b,pt c){
	return is0(crs(a,b,c));
}
//ABC301G
bool sameplane(pt a,pt b,pt c,pt d){
	return sgn(det(a,b,c,d))==0;
}
//4点が同一平面上にあることを仮定
//ABC301G
bool ill(pt a,pt b,pt c,pt d,pt&res){
	pt p=crs(a,c,d),q=crs(b,c,d);
	ld ax=abs(p.x),ay=abs(p.y),az=abs(p.z);
	ld r,s;
	if(ax>=ay&&ax>=az)r=p.x,s=q.x;
	else if(ay>=az)r=p.y,s=q.y;
	else r=p.z,s=q.z;
	if(r!=s){
		res=(b*r-a*s)/(r-s);//V^3/V^2
		return true;
	}else{
		return false;
	}
}
//UCUP2-3-J
pt anotheraxis(pt v){
	ld ax=abs(v.x),ay=abs(v.y),az=abs(v.z);
	if(ax<=ay&&ax<=az)return pt(0,-v.z,v.y);
	else if(ay<=az)return pt(v.z,0,-v.x);
	else return pt(-v.y,v.x,0);
}

vc<pt> goodset(const vc<pt>&ps){
	vc<pt> cur;
	rep(i,si(ps)){
		if(si(cur)==0){
			cur.pb(ps[i]);
		}else if(si(cur)==1){
			if(sgn((cur[0]-ps[i]).length2()))
				cur.pb(ps[i]);
		}else if(si(cur)==2){
			if(!sameline(cur[0],cur[1],ps[i]))
				cur.pb(ps[i]);
		}else if(si(cur)==3){
			if(!sameplane(cur[0],cur[1],cur[2],ps[i]))
				cur.pb(ps[i]);
		}else if(si(cur)==4){
			//do nothing
		}
	}
	return cur;
}

using T=tuple<pt,pt,pt>;
//面を返す; 全部三角形
//外側から見たとき，反時計まわりに列挙
//UCUP-2-20-D
ld abs(pt v){
	return sqrt(norm(v));
}
//dir,thre -> dot(dir,v)<=thre の領域からなる凸包
vc<T> halfpint(vc<pair<pt,ld>> hps){
	//assume bounded
	
	vc<T> res;
	
	rep(root,si(hps)){
		auto [X,off]=hps[root];
		auto Y=anotheraxis(X);
		Y/=abs(Y);
		auto Z=crs(X,Y);
		assert(sgn(abs(Z),1)==0);
		
		vc<geo2d::ln> ls;
		//for(auto [dir,thre]:hps){
		rep(i,si(hps))if(i!=root){
			auto [dir,thre]=hps[i];
			ld x=dot(dir,X);
			ld y=dot(dir,Y);
			ld z=dot(dir,Z);
			
			geo2d::pt u(y,z);
			//dot(u,v)+x*off<=thre
			if(u==geo2d::pt(0,0)){
				if(x*off>thre)goto done;
			}else{
				geo2d::pt a=u*((thre-x*off)/norm(u));
				auto b=a+geo2d::rot90(u);
				ls.eb(a,b);
			}
		}
		{
			//dmp2(X,off);
			//dmp(ls);
			auto w=geo2d::halfpint(ls);
			
			auto ch=[&](geo2d::pt q){
				return X*off+Y*q.x+Z*q.y;
			};
			
			rng(i,1,si(w)-1){
				res.eb(ch(w[i+1]),ch(w[i]),ch(w[0]));
			}
		}
		
		done:;
	}
	
	vc<pt> vs;
	for(auto [a,b,c]:res){
		vs.pb(a);
		vs.pb(b);
		vs.pb(c);
	}
	mkuni(vs);
	
	auto rectify=[&](pt v){
		int i=lwb(vs,v);
		assert(i<si(vs)&&vs[i]==v);
		return vs[i];
	};
	
	vc<T> tmp;
	for(auto [a,b,c]:res){
		if(a==b||b==c||c==a)continue;
		tmp.eb(rectify(a),rectify(b),rectify(c));
	}
	
	return tmp;
}
