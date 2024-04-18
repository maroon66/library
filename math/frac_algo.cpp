//linear_sieve
//initmu
//fdiv,cdiv

bool cmpf(pi a,pi b){
	return a.a*b.b<a.b*b.a;
}

//p/q <= num/den, 0<=p, 1<=q<=n
//の形で表される有理数の個数を返す
//O(N)
int count_frac(int n,int num,int den){
	int ans=0;
	rng(v,1,n+1){
		int w=max<int>((num*v+den)/den,0);
		int s=0;
		for(auto m:ms){
			if(v*m>n)break;
			s+=mu[m];
		}
		ans+=w*s;
	}
	return ans;
}

//ARC123F
//num/den < p/q, (0<=p,1<=q<=n)
//の形で表される既約分数を小さい方から len 個返す
//O(N+Len)
vc<pi> enum_frac(int len,int n,int num,int den){
	pi l(fdiv(num,den),1);
	pi r(l.a+1,1);
	rng(v,2,n+1){
		pi w(fdiv(num*v,den),v);
		if(cmpf(l,w))l=w;
		w.a++;
		if(cmpf(w,r))r=w;
	}
	vc<pi> res(len);
	rep(i,len){
		res[i]=r;
		
		if(l.b>r.b){
			int d=cdiv(l.b-r.b,r.b);
			l.a-=r.a*d;
			l.b-=r.b*d;
		}
		if(l.b==r.b){
			assert(l.b==1);
			l.a++;
			r.b++;
		}else{
			tie(l,r)=mp(r,pi(r.a-l.a,r.b-l.b));
		}
		{
			int d=fdiv(n-r.b,l.b);
			r.a+=l.a*d;
			r.b+=l.b*d;
		}
	}
	return res;
}

//↓int とか書いてある箇所がちらほらあるがどこまで大丈夫かわからない
//I に書き換えたほうが安全だと思う（は？）

//find the upper left convex of (x,(ax+b)%c) (0<=x<n)
//返る配列は log(c) くらいのサイズ
//xy が頂点を表している(maxを最初達成するところで切れる)(xy が strict convex であるとは限らない)
//step がその間で何ステップあるか
//si(xy)==si(step)+1
//return (xy,step)
//1<=c
template<class I>
pair<vc<pair<I,I>>,vc<I>> upper_left_convex(I a,I b,I c){
	a=(a%c+c)%c;
	b=(b%c+c)%c;
	auto [g,inva]=modinv<I>(a,c);
	vc<pair<I,I>> xy;
	auto addxy=[&](I y){
		assert(y%g==0);
		assert(y+b<c);
		I x=y/g*inva%c;
		xy.eb(x,y+b);
	};
	addxy(0);
	vc<I> step;
	I h=a,w=c,z=c-b,off=0;
	while(h){
		I p=(z-1)/h,q=z-p*h;
		if(p){
			addxy(off+=p*h);
			step.pb(p);
		}
		w%=h;
		if(w==0)break;
		I u=((h-q)/w+1)*w;
		if(u<h){
			addxy(off+=h-u);
			step.pb(1);
			q=q+u-h;
		}
		h%=w;
		z=q;
	}
	return mp(xy,step);
}

//Codechef 2021 August Lunchtime ABCD
//0<=p<=q
//1<=q
//find the largest x/y s.t. x/y<=p/q and y<=m
//return (x,y)
template<class I>
pair<I,I> largest(I p,I q,I m){
	{
		I g=gcd(p,q);
		p/=g;
		q/=g;
	}
	if(q<=m)return mp(p,q);
	I ansx=0,ansy=1;
	auto upd=[&](I x,I y){
		if(ansx*y<ansy*x){
			ansx=x;
			ansy=y;
		}
	};
	auto [xy,step]=upper_left_convex(p,q-m*p%q,q);
	rep(i,si(xy)){
		int x=xy[i].a;
		if(x<m){
			//do nothing
		}else if(xy[i-1].a<m){
			int d=(x-xy[i-1].a)/step[i-1];
			x=(m-1-xy[i-1].a)/d*d+xy[i-1].a;
		}else break;
		x=m-x;
		upd(p*x/q,x);
	}
	/*{
		dmp2(p,q,m);
		dmp2(ansx,ansy);
		assert(inc(1,ansy,m));
		assert(ansx*q<=ansy*p);
		rng(y,1,m+1){
			int x=y*p/q;
			if(not (x*ansy<=y*ansx)){
				dmp2(x,y);
			}
			assert(x*ansy<=y*ansx);
		}
	}*/
	return mp(ansx,ansy);
}
