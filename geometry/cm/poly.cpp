//AOJ1183
bool ips(const vc<cm>&a,ln b){
	int n=a.size();
	rep(i,n){
		cm c=a[(i+n-1)%n],d=a[i],e=a[(i+1)%n];
		if(iss(ln(c,d),b)==2)return 1;
		if(ccw(b,d)==0&&bet(b.a,b.b,d)==0){
			if(ccw(c,d,e)==1)return 1;
			if(ccw(b,c)*ccw(b,e)<0)return 1;
		}
	}
	return 0;
}

//GCJ2019R3D
//0-out,1-edge,2-in
int cont(const vc<cm>&a,cm b){
	int n=a.size();
	int res=0;
	rep(i,n){
		cm c=a[i],d=a[(i+1)%n];
		if(ccw(c,d,b)==0&&abs(bet(c,d,b))<=1)
			return 1;
		if(c.y>d.y)swap(c,d);
		if(sgn(b.y-c.y)>0&&sgn(d.y-b.y)>=0&&ccw(c,d,b)==1)
			res^=1;
	}
	return res*2;
}

ld area2(const vc<cm>&a){
	int n=a.size();
	ld b=0;
	rep(i,n)
		b+=crs(a[i],a[(i+1)%n]);
	return b;
}

//AOJ0068
//TCO 2017 final Hard
//allow onedge vertices?
vc<cm> convex(vc<cm> s,bool onedge){
	swap(s[0],*min_element(all(s),cmless));
	sort(s.bg+1,s.ed,[&](cm a,cm b){
		int c=ccw(s[0],a,b);
		if(c)return c==1;
		else{
			if(onedge&&sgn(s[0].x-a.x)==0)
				return a.y>b.y;
			return bet(s[0],a,b)==2;
		}
	});
	vc<cm> t;
	rep(i,s.size()){
		int ss;
		while((ss=t.size())>=2){
			cm a=t[ss-2];
			cm b=t[ss-1];
			cm c=s[i];
			if(ccw(a,b,c)>=(onedge?0:1))
				break;
			t.pop_back();
		}
		t.pb(s[i]);
	}
	return t;
}

//JAGSUMMER2019 E
//assume the ansewer is bounded
vc<cm> halfpint(vc<ln> s){
	sort(all(s),[&](ln a,ln b){
		int c=argcmp(dir(a),dir(b));
		if(c)return c<0;
		return ccw(b,a.a)>0;
	});
	s.erase(unique(all(s),[&](ln a,ln b){
		return argcmp(dir(a),dir(b))==0;
	}),s.ed);
	int n=s.size();
	vi cur;
	rep(ii,n*2){
		int i=ii%n,m;
		while((m=cur.size())>=2){
			if(ccw(s[i],cll(s[cur[m-2]],s[cur[m-1]]))>0)
				break;
			cur.pop_back();
		}
		cur.pb(i);
	}
	vi cnt(n);
	for(auto i:cur)cnt[i]++;
	vc<ln> t;
	rep(i,n)if(cnt[i]==2)
		t.pb(s[i]);
	int m=t.size();
	vc<cm> res(m);
	rep(i,m)
		res[i]=(cll(t[i],t[(i+1)%m]));
	return res;
}

//AOJ1283
//convex cut
//なんか 1 とか 2 頂点だけ残ることがある
vc<cm> ccut(vc<cm> a,ln b){
	int n=a.size();
	vc<cm> c;
	rep(i,n){
		int j=(i+1)%n;
		int d=ccw(b,a[i]),e=ccw(b,a[j]);
		if(d>=0)c.pb(a[i]);
		if(d*e<0)c.pb(cll(b,ln(a[i],a[j])));
	}
	return c;
}
