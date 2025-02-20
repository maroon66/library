//find the indices of row maxima
//the smallest index is used in case of tie
//f の戻り地が小数のときは比較関数のあたりの精度が怪しいが，大体は動くのではないだろうか？
//cmp(i,j,k) = f(i,j)<f(i,k)
//-inf とかを入れて下三角だと思うとかでもいいはず (VERIFY:KUPC2021F)
//-inf の値は全部同じじゃないとダメ (正確には各行で広義単調減少であるべき)
//-inf の扱いに注意 
//例: 数列と凸関数の畳込みで，凸関数の値がない部分を inf と思い込むとする
//このときすべてを inf に丸めると壊れる
//値がない部分は傾き inf と思う必要がある → int128 (KUPC2021F)
//最終手段で int128 を召喚
//→ f ではなく cmp を与える形式にしたことでこれを回避
//f がいい感じの性質を満たしていないと WA どころか RE でぶっ壊れる
//VERIFY
//Codechef CHEFAOR (needs optimization!)
//Petrozavodsk Camp 2010S Kyiv NU Contest I (D=long double,double で動作確認)
//GCJ 2020 WF D (D=long double)
//CF694F
template<class F>
vi smawk(F cmp,int n,int m){
	vi ans(n,-1);
	auto upd=[&](int r,int c){
		if(ans[r]==-1||cmp(r,ans[r],c)){
			ans[r]=c;
		}
	};
	auto rec=[&](auto self,int*const rs,int x,int*const cs,int y)->void{
		const int t=8;
		if(x<=t||y<=t){
			rep(i,x)rep(j,y)upd(rs[i],cs[j]);
			return;
		}
		if(x<y){
			int s=0;
			rep(i,y){
				int c=cs[i];
				while(s&&cmp(rs[s-1],cs[s-1],c))
					s--;
				if(s<x)
					cs[s++]=c;
			}
			y=s;
		}
		auto a=rs+x,b=cs+y;
		int z=0;
		for(int i=1;i<x;i+=2)
			a[z++]=rs[i];
		rep(i,y)
			b[i]=cs[i];
		self(self,a,z,b,y);
		int k=0;
		for(int i=0;i<x;i+=2){
			int to=i+1<x?ans[rs[i+1]]:cs[y-1];
			while(1){
				upd(rs[i],cs[k]);
				if(cs[k]==to)break;
				k++;
			}
		}
	};
	static int rs[1234567];//at least n*2
	static int cs[1235467];//at least max(m,n*2)
	rep(i,n)rs[i]=i;
	rep(i,m)cs[i]=i;
	rec(rec,rs,n,cs,m);
	return ans;
}

//Codechef 2022 August Lunchtime E
vi max_convolution_with_upper_convex(const vi&conv,const vi&a){
	int n=si(conv),m=si(a);
	auto cmp=[&](int i,int j,int k){
		assert(j<k);
		if(i<k)return false;
		if(i-j>=n)return true;
		return conv[i-j]+a[j]<conv[i-k]+a[k];
	};
	vi idx=smawk(cmp,n+m-1,m);
	vi res(n+m-1);
	rep(i,n+m-1)res[i]=conv[i-idx[i]]+a[idx[i]];
	return res;
}
//SEERC 2022 L
//CF999 I
vi min_convolution_with_lower_convex(const vi&conv,const vi&a){
	int n=si(conv),m=si(a);
	auto cmp=[&](int i,int j,int k){
		assert(j<k);
		if(i<k)return false;
		if(i-j>=n)return true;
		return conv[i-j]+a[j]>conv[i-k]+a[k];
	};
	vi idx=smawk(cmp,n+m-1,m);
	vi res(n+m-1);
	rep(i,n+m-1)res[i]=conv[i-idx[i]]+a[idx[i]];
	return res;
}

//stress-tested
//inf 周りも多分大丈夫
template<class F>
vc<ll> min_convolution_with_cost(vc<ll> a,F cost){
	int n=si(a);
	auto cmp=[&](int i,int j,int k){
		assert(j<k);
		if(i<=k)return false;
		return -(a[j]+cost(j,i))<-(a[k]+cost(k,i));
	};
	vi idx=smawk(cmp,n,n);
	vc<ll> res(n,infLL);
	rep(i,n)if(idx[i]<i)res[i]=a[idx[i]]+cost(idx[i],i);
	return res;
}

//UCUP 2023-3 I
const int nmax=6005;
int cost[nmax][nmax];
//cost[i][j] (i>j) のみをつかう
vi min_convolution_with_cost(vi a){
	int n=si(a);
	auto cmp=[&](int i,int j,int k){
		assert(j<k);
		if(i<=k)return false;
		return -(a[j]+cost[j][i])<-(a[k]+cost[k][i]);
	};
	vi idx=smawk(cmp,n,n);
	vi res(n,inf);
	rep(i,n)if(idx[i]<i)res[i]=a[idx[i]]+cost[idx[i]][i];
	return res;
}
