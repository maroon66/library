
int sgn(int a){
	return a>0?1:(a<0?-1:0);
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

vc<pi> lower_convex(vi a){
	vc<pi> res;
	rep(i,si(a)){
		while(si(res)>=2){
			int s=si(res);
			if(ccw(res[s-2],res[s-1],pi(i,a[i]))>0)break;
			res.pop_back();
		}
		res.eb(i,a[i]);
	}
	return res;
}
