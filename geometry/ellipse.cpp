//長径 a ,短径 b ,長径の角度 t の楕円
//Xmas2010E
struct ellipse{
	ld a,b,t;
	A get_eq(){
		A res{};
		auto add=[&](ld p,ld q,ld r){
			r=sq(r);
			res[0]+=sq(p)/r;
			res[1]+=2*p*q/r;
			res[2]+=sq(q)/r;
		};
		add(cos(t),sin(t),a);
		add(cos(t+PI/2),sin(t+PI/2),b);
		return res;
	}
	ld eval_dist(ld theta){
		theta-=t;
		ld cur=sq(cos(theta)/a)+sq(sin(theta)/b);
		return sqrt(1/cur);
	}
	//assume -PI<=theta<=PI
	ld eval_area_sub(ld theta){
		theta-=t;
		ld c=cos(theta)/a,d=sin(theta)/b;
		ld phi=sgn(theta-PI)==0?PI:(sgn(theta+PI)==0?-PI:atan2(d,c));
		return phi/2*a*b;
	}
	ld eval_area(ld lw,ld up){
		return eval_area_sub(up)-eval_area_sub(lw);
	}
};
