using ln=pair<cm,cm>;
cm dir(ln a){return a.b-a.a;}
cm eval(ln a,ld b){return a.a+dir(a)*b;}
int bet(ln a,cm b){return bet(a.a,a.b,b);}
int ccw(ln a,cm b){return ccw(a.a,a.b,b);}
cm proj(ln a,cm b){
	cm c=dir(a);
	return a.a+c*dot(c,b-a.a)/norm(c);
}
cm refl(ln a,cm b){
	return ld(2)*proj(a,b)-b;
}
//AOJ0153
ld dsp(ln a,cm b){
	cm c=proj(a,b);
	if(abs(bet(a.a,a.b,c))<=1)return abs(b-c);
	return min(abs(b-a.a),abs(b-a.b));
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
//AOJ1033
cm cll(ln a,ln b){
	return eval(a,crs(b.a,b.b,a.a)/crs(dir(a),dir(b)));
}
//AOJ1157
ld dss(ln a,ln b){
	if(iss(a,b))return 0;
	return min({dsp(a,b.a),dsp(a,b.b),dsp(b,a.a),dsp(b,a.b)});
}
//AOJ2160
//反時計回り方向に伸びる垂直二等分線
ln vbis(cm a,cm b){
	cm c=(a+b)*ld(0.5),d=b-a;
	return ln(c,cm(c.x-d.y,c.y+d.x));
}
