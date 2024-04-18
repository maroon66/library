//UCUP 1-21 E
template<class F>
auto ternary_max(F f,int l,int r){
	while(r-l>=3){
		int m1=(l+l+r)/3;
		int m2=(l+r+r)/3;
		if(f(m1)<f(m2))l=m1;
		else r=m2;
	}
	auto val=f(l);
	int pos=l;
	rng(v,l+1,r+1)if(chmax(val,f(v)))pos=v;
	return mp(val,pos);
}

template<class F>
auto ternary_min(int l,int r,F f){
	while(r-l>=3){
		int m1=(l+l+r)/3;
		int m2=(l+r+r)/3;
		if(f(m1)>f(m2))l=m1;
		else r=m2;
	}
	auto val=f(l);
	int pos=l;
	rng(v,l+1,r+1)if(chmin(val,f(v)))pos=v;
	return mp(val,pos);
}

//UCUP 2-4-K
template<int L,class F>
auto ternary_min(F f,ld l,ld r){
	rep(_,L){
		ld m1=(l+l+r)/3;
		ld m2=(l+r+r)/3;
		if(f(m1)>f(m2))l=m1;
		else r=m2;
	}
	return mp(f(l),l);
}
//誤差基準で黄金探索
//CF930F
template<class F>
auto golden_max(ld a,ld b,ld allow,F f){
	assert(a<=b);
	const ld gold=(3-sqrtl(5))/2;
	ld c=a+gold*(b-a),d=b-gold*(b-a);
	auto fc=f(c),fd=f(d);
	while(b-a>allow){
		if(fc<fd){
			a=c;
			c=d;
			fc=fd;
			d=b-gold*(b-a);
			fd=f(d);
		}else{
			b=d;
			d=c;
			fd=fc;
			c=a+gold*(b-a);
			fc=f(c);
		}
	}
	return mp(f(c),c);
}
