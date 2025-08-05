using ln=pair<pt,pt>;
pt dir(ln a){return a.b-a.a;}
pt eval(ln a,ld b){return a.a+dir(a)*b;}
ld crs(ln a,pt b){return crs(a.a,a.b,b);}
int ccw(ln a,pt b){return ccw(a.a,a.b,b);}
int bet(ln a,pt b){return bet(a.a,a.b,b);}
ld projt(ln a,pt b){
	pt c=dir(a);
	return dot(c,b-a.a)/norm(c);
}
pt proj(ln a,pt b){
	pt c=dir(a);
	return a.a+c*dot(c,b-a.a)/norm(c);
}
pt refl(ln a,pt b){
	return proj(a,b)*2-b;
}
//AOJ1157
//0-no,1-yes(endpoint),2-yes(innner),3-overelap
//if the two line touch like this
// x----x----x
//it returns 1
int iss(ln a,ln b){
	int c1=ccw(a.a,a.b,b.a),c2=ccw(a.a,a.b,b.b);
	int d1=ccw(b.a,b.b,a.a),d2=ccw(b.a,b.b,a.b);
	if(c1||c2||d1||d2)return 1-max(c1*c2,d1*d2);
	int f=bet(a.a,a.b,b.a),g=bet(a.a,a.b,b.b);
	if(max(f,g)==-2||min(f,g)==2)return 0;
	if(max(f,g)==-1||min(f,g)==1)return 1;
	return 3;
}
//segment a intersects line b?
//endpoints inclusive
bool isl(ln a,ln b){
	int d1=ccw(b.a,b.b,a.a),d2=ccw(b.a,b.b,a.b);
	return d1*d2<=0;
}
//並行でない->true, というだけ
//直線が一致とかは考えてないことに注意
bool ill(ln a,ln b){
	return ccw(dir(a),dir(b));
}
ld cllt(ln a,ln b){
	return crs(b.a,b.b,a.a)/crs(dir(a),dir(b));
}
//ICPC Yokohama 2022 J
//ICPC Yokohama 2024 J (分母を正に統一)
pair<ld,ld> clltf(ln a,ln b){
	ld num=crs(b.a,b.b,a.a);
	ld den=crs(dir(a),dir(b));
	if(den<0){
		num=-num;
		den=-den;
	}
	return mp(num,den);
}
//AOJ1033
pt cll(ln a,ln b){
	return eval(a,crs(b.a,b.b,a.a)/crs(dir(a),dir(b)));
}
//UCUP3-4-H
bool isp(ln a,pt b){
	return ccw(a,b)==0&&inc(-1,bet(a.a,a.b,b),1);
}
#ifdef GEOF
//AOJ2201
ld dlp(ln a,pt b){
	return abs(crs(a,b)/abs(dir(a)));
}
//AOJ0153
ld dsp(ln a,pt b){
	pt c=proj(a,b);
	if(abs(bet(a.a,a.b,c))<=1)return abs(b-c);
	return min(abs(b-a.a),abs(b-a.b));
}
//ABC314H
//b から最も近い a 上の点を返す
pt dsp_tar(ln a,pt b){
	pt c=proj(a,b);
	if(abs(bet(a.a,a.b,c))<=1)return c;
	return abs(b-a.a)<abs(b-a.b)?a.a:a.b;
}
//AOJ1157
ld dss(ln a,ln b){
	if(iss(a,b))return 0;
	return min({dsp(a,b.a),dsp(a,b.b),dsp(b,a.a),dsp(b,a.b)});
}
//AOJ2160
//反時計回り方向に伸びる垂直二等分線
ln vbis(pt a,pt b){
	pt c=(a+b)*ld(0.5),d=b-a;
	return ln(c,pt(c.x-d.y,c.y+d.x));
}
ld cutareat(ln z,ld l,ld r){
	pt a=eval(z,l);
	pt b=eval(z,r);
	return -(b.x-a.x)*(a.y+b.y)/2;
}
#endif
//ABC286H
//simple polygon と線分が交わるか
//接している場合も true
/*
bool icl(const vc<pt>&ps,ln z){
	int n=si(ps);
	rep(i,n){
		pt p=ps[i],q=ps[(i+1)%n];
		if(iss(z,ln(p,q)))return true;
	}
	return cont(ps,z.a);
}*/
