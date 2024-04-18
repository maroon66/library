//最大重み max matching
//K 本のマッチングが求めたいなら whlie(arg()) のあたりをいじる
//min l[i]=Z とし、l'[i]=l[i]-Z とおくと、
//min sum l'[i] + sum r[i] + K*Z s.t. l'[i] + r[j] + Z >= C(e(i,j))
//の解になっている
//ちなみにこれは K 本の最大重みマッチングの双対問題
//マッチングは to,ot から得られる
//マッチングの辺では l'[i] + r[j] + Z = C(e(i,j)) が成り立つはず
//左頂点数を N 右頂点数を M 辺数を E マッチングサイズを S としたとき，O(S(E+S(N+M))) くらいかな
//定数倍は軽いと思う
//多重辺があっても動くはず（未検証）
//VERIFY: yosupo
template<class E,class D>
struct hungarian{
	int n,m;
	const vvc<E>&g;
	vc<D> l,r,q;
	vi to,ot,p;
	vc<char> lu,ru;
	int sz=0;
	D ans=0;
	void L(int i){
		lu[i]=1;
		for(auto e:g[i])if(!ru[e.to]){
			D w=l[i]+r[e.to]-e.cost;
			if(q[e.to]>w){
				p[e.to]=i;
				q[e.to]=w;
			}
		}
	}
	bool arg(){
		fill(all(p),-1);
		fill(all(q),inf);
		fill(all(lu),0);
		fill(all(ru),0);
		rep(i,n)if(to[i]==-1)L(i);
		while(1){
			int x=-1;
			rep(i,m)if(!ru[i]&&p[i]!=-1&&(x==-1||q[x]>q[i]))x=i;
			if(x==-1)return 0;
			D w=q[x];
			rep(i,n)if(lu[i])l[i]-=w;
			rep(i,m)if(ru[i])r[i]+=w;else if(p[i]!=-1)q[i]-=w;
			if(ot[x]==-1){
				while(x!=-1){
					ot[x]=p[x];
					swap(to[ot[x]],x);
				}
				sz++;
				return 1;
			}
			ru[x]=1;
			L(ot[x]);
		}
	}
	hungarian(int nn,int mm,const vvc<E>&gg):n(nn),m(mm),g(gg),
		l(n),r(m),q(m),to(n,-1),ot(m,-1),p(m),lu(n),ru(m){
		D mx=-inf;
		rep(i,n)for(auto e:g[i])chmax(mx,e.cost);
		fill(all(l),mx);
		while(arg())chmax(ans,get());
	}
	D get(){
		D mn=*min_element(all(l));
		return accumulate(all(l),accumulate(all(r),(-n+sz)*mn));
	}
};
struct E{
	int to,cost;
};
