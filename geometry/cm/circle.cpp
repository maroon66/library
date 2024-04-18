using cr=pair<cm,ld>;
//AOJ0153
//0-no,1-edge,2-in
int cont(cr a,cm b){
	return sgn(a.b-abs(b-a.a))+1;
}
//AOJ0153
//0-no,1-touch,2-cross
int ids(cr a,ln b){
	return sgn(a.b-dsp(b,a.a))+1;
}
//AOJ0129 (touch以外)
//0-no(in),1-touch(in),2-cross,3-touch(out),4-no(out)
int ics(cr a,ln b){
	int c=ids(a,b);
	if(c<=1)return 4-c;
	return sgn(max(abs(b.a-a.a),abs(b.b-a.a))-a.b)+1;
}
//AOJ1183
//eval(b,t) が a と重なる t を c,d に入れる
//解の個数を返す
int ccl(cr a,ln b,ld&c,ld&d){
	cm e=dir(b);
	cm f=b.a-a.a;
	return qeq(norm(e),2*dot(e,f),norm(f)-a.b*a.b,c,d);
}
//AOJ0023
//0-apart,1-coinside,2-a<b,3-a<=b,4-a>b,5-a>=b,6-a touch b,7-a cross b
int icc(cr a,cr b){
	ld c=abs(a.a-b.a);
	if(sgn(c)==0&&sgn(a.b-b.b)==0)return 1;
	int d=sgn(c+a.b-b.b);
	if(d<=0)return d+3;
	int e=sgn(c+b.b-a.b);
	if(e<=0)return e+5;
	int f=sgn(a.b+b.b-c);
	if(f>=0)return f+6;
	return 0;
}
//AOJ2572
//args of two intersections r,l seen be a.a
//assume two circles cross
pair<ld,ld> ccc(cr a,cr b){
	ld c=arg(b.a-a.a);
	ld d=arg(a.b,abs(b.a-a.a),b.b);
	return make_pair(c-d,c+d);
}
cm cccl(cr a,cr b){
	ld c=ccc(a,b).b;
	return a.a+polar(a.b,c);
}
cm cccr(cr a,cr b){
	ld c=ccc(a,b).a;
	return a.a+polar(a.b,c);
}
//XXI Opencup GP of Siberia 5
vc<cm> ccc_list(cr a,cr b){
	int v=icc(a,b);
	if(v==0||v==1||v==2||v==4)return {};
	auto z=ccc(a,b);
	if(v==3||v==5||v==6){
		return {a.a+polar(a.b,z.a)};
	}else{
		return {a.a+polar(a.b,z.a),a.a+polar(a.b,z.b)};
	}
}
//原点中心,半径 r の円の x 座標 a 以下の領域の面積
ld cutarea_sub(ld r,ld a){
	ld theta=acos(max(min(a/r,ld(1)),ld(-1)));
	return sq(r)*(PI-theta)+a*r*sin(theta);
}
//2020 Multi-Uni Day5 J
//x座標 b 以上 c 以下の部分だけ取ってきて，その面積
ld cutarea(cr a,ld b,ld c){
	chmax(b,a.a.x-a.b);
	chmin(c,a.a.x+a.b);
	b-=a.a.x;
	c-=a.a.x;
	if(sgn(c-b)<=0)return 0;
	return cutarea_sub(a.b,c)-cutarea_sub(a.b,b);
}
//not verified
ld commonarea(cr a,cr b){
	int t=icc(a,b);
	if(t==0||t==6)return 0;
	if(t!=7)return sq(min(a.b,b.b))*PI;
	ld c=abs(b.a-a.a);
	ld d=arg(a.b,c,b.b);
	ld e=arg(b.b,c,a.b);
	return sq(a.b)*d+sq(b.b)*e-heron(a.b,c,b.b)*2;
}
//AOJ2572
//area of circular segment
ld cseg(ld a,ld b){
	return a*a*(b-sin(b))/2;
}
//AOJ2423
cr circumc(cm a,cm b,cm c){
	b-=a;
	c-=a;
	cm r=norm(b)*c-norm(c)*b;
	r=cm(r.y,-r.x)/(2*crs(b,c));
	return cr(a+r,abs(r));
}
//AOJ2423
cr mindisc(const vc<cm>& p,array<cm,3> q,int i,int j){
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
	cr c=mindisc(p,q,i+1,j);
	if(sgn(abs(c.a-p[i])-c.b)==1){
		assert(j<3);
		q[j]=p[i];
		return mindisc(p,q,i+1,j+1);
	}else
		return c;
}
cr mindisc(vc<cm> p){
	shuffle(all(p),mt19937());
	return mindisc(p,array<cm,3>(),0,0);
}
cm evaldir(cr a,ld b){
	return a.a+polar(a.b,b);
}
//assume p lies good
ld evalh(cr a,ld p){
	p-=a.a.x;
	assert(sgn(abs(p)-a.b)<=0);
	return sqrt(max<ld>(sq(a.b)-sq(p),0));
}
cr readcr(){
	int a,b,c;cin>>a>>b>>c;
	return cr(cm(a,b),c);
}
