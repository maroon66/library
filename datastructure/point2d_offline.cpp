//aq の順番注意!
//offline
//ROI2018 day2 D
//Yandex Cup 2023 Semifinal F (TLE...)
struct point2d{
	struct query{
		int x,i,l,r;
		bool operator<(const query&rhs)const{
			return pi(x,-i)<pi(rhs.x,-rhs.i);
		}
	};
	vi ys,ans;
	vc<query> qs;
	point2d(const vc<pi>&ps){
		for(auto p:ps)
			ys.pb(p.b);
		mkuni(ys);
		for(auto p:ps)
			qs.pb({p.a,-1,lwb(ys,p.b),-1});
	}
	void aq(int x1,int x2,int y1,int y2){
		int i=si(ans);
		ans.pb(0);
		y1=lwb(ys,y1);
		y2=lwb(ys,y2);
		qs.pb({x1,i*2,y1,y2});
		qs.pb({x2,i*2+1,y1,y2});
	}
	vi calc(){
		sort(all(qs));
		BIT<int> bit(si(ys));
		for(auto q:qs){
			if(q.i==-1){
				bit.add(q.l,1);
			}else{
				int w=bit.sum(q.l,q.r);
				if(q.i%2==0)ans[q.i/2]-=w;
				else ans[q.i/2]+=w;
			}
		}
		return ans;
	}
};

//各点に重みついてる
//inf+N が値域に収まる
//aq(x,y) は境界を含む [-inf,x][-inf,y] を集めてくる
//下限なし，定数倍のため
//offline
//CF433E
//XXII Opencup GP of Daejeon F
struct point2d{
	struct query{
		int x,i,y;
		bool operator<(const query&rhs)const{
			return pi(x,-i)<pi(rhs.x,-rhs.i);
		}
	};
	vi ys;
	vc<ll> ans;
	vc<query> qs;
	point2d(const vc<tuple<int,int,int>>&ps){
		for(auto p:ps){
			int x,y,c;tie(x,y,c)=p;
			if(c)ys.pb(y);
		}
		mkuni(ys);
		for(auto p:ps){
			int x,y,c;tie(x,y,c)=p;
			if(c)qs.pb({x,c,lwb(ys,y)});
		}
	}
	//x,y inclusive
	void aq(int x,int y){
		x++;y++;
		int i=si(ans);
		ans.pb(0);
		y=lwb(ys,y);
		qs.pb({x,inf+i,y-1});
	}
	vc<ll> calc(){
		sort(all(qs));
		BIT<ll> bit(si(ys));
		for(auto q:qs){
			if(q.i<inf){
				bit.add(q.y,q.i);
			}else{
				ans[q.i-inf]+=bit.get(q.y);
			}
		}
		return ans;
	}
};

//重み付きの点を足して長方形取得
//offline
//Multiuni2023-7-B
struct point2d{
	struct query{
		int x,i,l,r;
		bool operator<(const query&rhs)const{
			return pi(x,-i)<pi(rhs.x,-rhs.i);
		}
	};
	vi ys;
	vc<ll> ans;
	vc<query> qs;
	point2d(const vc<tuple<int,int,int>>&ps){
		for(auto p:ps){
			int x,y,c;tie(x,y,c)=p;
			if(c)ys.pb(y);
		}
		mkuni(ys);
		for(auto p:ps){
			int x,y,c;tie(x,y,c)=p;
			if(c)qs.pb({x,c,lwb(ys,y),-1});
		}
	}
	void aq(int x1,int x2,int y1,int y2){
		int i=si(ans);
		ans.pb(0);
		y1=lwb(ys,y1);
		y2=lwb(ys,y2);
		qs.pb({x1,inf+i*2,y1,y2});
		qs.pb({x2,inf+i*2+1,y1,y2});
	}
	vc<ll> calc(){
		sort(all(qs));
		BIT<ll> bit(si(ys));
		for(auto q:qs){
			if(q.i<inf){
				bit.add(q.l,q.i);
			}else{
				q.i-=inf;
				ll w=bit.sum(q.l,q.r);
				if(q.i%2==0)ans[q.i/2]-=w;
				else ans[q.i/2]+=w;
			}
		}
		return ans;
	}
};

//長方形領域に加算，点取得
//query のソート関数が微妙にほかと違う
//work without define int ll
//Yandex Cup 2023 Semifinal F (TLE...)
struct point2d_2{
	struct query{
		int x,i,l,r;
		bool operator<(const query&rhs)const{
			return pi(x,i)<pi(rhs.x,rhs.i);
		}
	};
	vi ys;
	vc<ll> ans;
	vc<query> qs;
	point2d_2(const vc<pi>&ps){
		for(auto p:ps)
			ys.pb(p.b);
		mkuni(ys);
		ans.resize(si(ps));
		rep(i,si(ps))
			qs.pb({ps[i].a,inf+i,lwb(ys,ps[i].b),-1});
	}
	void aq(int x1,int x2,int y1,int y2,int w){
		assert(x1<=x2);
		assert(y1<=y2);
		y1=lwb(ys,y1);
		y2=lwb(ys,y2);
		qs.pb({x1,w,y1,y2});
		qs.pb({x2,-w,y1,y2});
	}
	vc<ll> calc(){
		sort(all(qs));
		BIT<ll> bit(si(ys));
		for(auto q:qs){
			if(q.i<inf){
				bit.add_range(q.l,q.r,q.i);
			}else{
				ans[q.i-inf]=bit.get(q.l);
			}
		}
		return ans;
	}
};
