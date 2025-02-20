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
//UCUP 3-27-D
template<class F>
auto golden_min(ld a,ld b,int itr,F f){
	assert(a<=b);
	const ld gold=(3-sqrtl(5))/2;
	ld c=a+gold*(b-a),d=b-gold*(b-a);
	auto fc=f(c),fd=f(d);
	rep(_,itr){
		if(fc>fd){
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


//thanks to maspy
// returns: {fx, x}
// [L, R) での極小値をひとつ求める、単峰は不要
template <typename T, bool MINIMIZE, typename F>
pair<T, ll> fibonacci_search(F f, ll L, ll R) {
  assert(L < R);
  --R;
  ll a = L, b = L + 1, c = L + 2, d = L + 3;
  int n = 0;
  while (d < R) { b = c, c = d, d = b + c - a, ++n; }
  auto get = [&](ll x) -> T {
    if (R < x) return infLL;
    return (MINIMIZE ? f(x) : -f(x));
  };
  T ya = get(a), yb = get(b), yc = get(c), yd = get(d);
  // この中で極小ならば全体でも極小、を維持する
  rep(_,n) {
    if (yb <= yc) {
      d = c, c = b, b = a + d - c;
      yd = yc, yc = yb, yb = get(b);
    } else {
      a = b, b = c, c = a + d - b;
      ya = yb, yb = yc, yc = get(c);
    }
  }
  ll x = a;
  T y = ya;
  if (chmin(y, yb)) x = b;
  if (chmin(y, yc)) x = c;
  if (chmin(y, yd)) x = d;
  if (MINIMIZE) return {y, x};
  return {-y, x};
}
