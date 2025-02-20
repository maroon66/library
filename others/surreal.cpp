//i -> j (i>j) のみ
//ABC229H
template<int L>
vi surreal(vc<array<vi,2>> g){
	auto sub=[&](int l,int r){
		per(lv,L+1){
			int v=((l>>lv)+1)<<lv;
			if(v<r)return v;
		}
		assert(false);
	};
	int n=si(g);
	vi res(n);
	rep(i,n){
		int mx=-inf,mn=inf;
		for(auto j:g[i][0])chmax(mx,res[j]);
		for(auto j:g[i][1])chmin(mn,res[j]);
		assert(mx<mn);
		if(mx<0&&0<mn){
			res[i]=0;
		}else if(mn<=0){
			res[i]=-sub(-mn,-mx);
		}else if(0<=mx){
			res[i]=sub(mx,mn);
		}
	}
	return res;
}

//i -> j (i<j) のみ
//UCUP 3-16-D (* は 0 or 1)
/*
合計 $a \cdot (*) + b$ ($a,b\in\mathbb{Z}$) なら解けるよ
先手勝ち $\iff (a \bmod 2) \cdot \varepsilon + b > 0$

$\{負 \mid *\}=0$
$\{* \mid 正\}=0$
$\{* \mid *\}=0$
$\{0 \mid 0\}=*$

$\{0,* \mid 1\} = 1/2$
$\{0,* \mid \} = 1$
$\{* \mid \} = 0$

$\{0,* \mid 0,*\} = *2$ らしいです！ ($*2$ は nim 2)

$\{0 \mid 1+* \} = 1$ 説

今挑戦中 (→AC)
$*$ が左に置いてあったら $-\varepsilon$
$*$ が右に置いてあったら $+\varepsilon$
として，max(左) >= min(右) になってしまうケースは $\{x\mid x\}$ ($x$: 数) のみと信じてトライ


定理7.7
$x$ を数として，
左 $x + *l_1, \ldots, x + *l_a$
右 $x + *r_1, \ldots, x + *r_b$
とする
ここで $mex(l_1,\ldots,l_a) = mex(r_1,\ldots,r_b) = m$ なら，
$\{ 左 \mid 右 \} = x + *m$ になる
mex が異なる場合は知らん
*/
template<int L>
vc<pi> surreal(vc<array<vi,2>> g){
	auto showsurreal=[&](pi ab){
		auto [a,b]=ab;
		int w=gcd(a,1<<L);
		cerr<<a/w<<"/"<<(1<<L)/w<<" + "<<b<<"*"<<endl;
	};
	auto sub=[&](int l,int r){
		per(lv,L+1){
			int v=((l>>lv)+1)<<lv;
			if(v<r)return v;
		}
		assert(false);
	};
	int n=si(g);
	rep(i,n)rep(k,2)for(auto j:g[i][k])assert(i<j);
	vc<pi> res(n);
	per(i,n){
		pi mx(-inf,0),mn(inf,0);
		for(auto j:g[i][0])chmax(mx,pi(res[j].a,-res[j].b));
		for(auto j:g[i][1])chmin(mn,pi(res[j].a,res[j].b));
		if(mx>mn){
			showtable(i);
			rep(k,2){
				cerr<<"LR"[k]<<endl;
				for(auto j:g[i][k]){
					showtable(j);
					showsurreal(res[j]);
				}
			}
			assert(false);
		}
		assert(mx<=mn);
		if(mx==mn){
			{
				int lstar=0,rstar=0;
				for(auto j:g[i][0])if(res[j].a==mx.a)chmax(lstar,res[j].b);
				for(auto j:g[i][1])if(res[j].a==mn.a)chmax(rstar,res[j].b);
				if(lstar||rstar){
					showtable(i);
					rep(k,2){
						cerr<<"LR"[k]<<endl;
						for(auto j:g[i][k]){
							showtable(j);
							showsurreal(res[j]);
						}
					}
					assert(false);
				}
			}
			assert(mx.b==0);
			res[i]=pi(mx.a,1);
		}else{
			if(mx.a==mn.a)assert(mx.b==-1&&mn.b==1);
			int x=mx.a+mx.b,y=mn.a+mn.b;
			if(x<0&&0<y){
				res[i].a=0;
			}else if(y<=0){
				res[i].a=-sub(-y,-x);
			}else if(0<=x){
				res[i].a=sub(x,y);
			}
		}
	}
	return res;
}
