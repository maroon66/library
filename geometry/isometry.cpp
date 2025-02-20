using isometry=pair<pt,pt>;
pt isoapplied(const pt&v,const isometry&a){
	return v*a.a+a.b;
}
void isoapply(pt&v,const isometry&a){
	v=isoapplied(v,a);
}
isometry get_isometry(pair<pt,pt> from,pair<pt,pt> to){
	pt a=from.b-from.a;
	pt b=to.b-to.a;
	pt r=b*a.conj()/a.norm();
	pt off=to.a-from.a*r;
	isometry res(r,off);
	assert(isoapplied(from.a,res)==to.a);
	assert(isoapplied(from.b,res)==to.b);
	return res;
}
