ll fld(ll a, ll b) { // floored division
	return a / b - ((a ^ b) < 0 && a % b); }
ll cld(ll a, ll b) { // ceiled division
	return a / b + ((a ^ b) > 0 && a % b); }
ll rem(ll a,ll b){
	return a-fld(a,b)*b;}

//+a,-b の回数を求めてやる
pi abnum(int a,int b,int n){
	assert(1<=a);
	assert(1<=b);
	int x=find_max_true(0,n+1,[&](int z){
		return z+(a*z)/b<=n;
	});
	int y=(a*x)/b;
	int cur=x*a-y*b;
	n-=x+y;
	if(n){
		cur+=a;
		x++;
		n--;
		assert(cur/b>=n);
		y+=n;
		n=0;
	}
	return pi(x,y);
}

//0,...,a+b-1 という座標を考える
//0 からスタートし，+a or -b (できる方が一意に定まる)を実行
//合計 n 回移動する
//移動元の座標が [0,c) に入っているなら ax,bx, 入っていないなら ay,by をかける
template<class N>
N gauss_sum_monoid_super(int a,int b,int c,int n,N ax,N ay,N bx,N by){
	assert(1<=a);
	assert(1<=b);
	assert(gcd(a,b)==1);
	assert(inc(0,c,a+b));
	assert(0<=n);
	
	auto x_pow=[&](N&x,N v,int k){
		while(k){
			if(k&1)x=N::merge(x,v);
			if(k>>=1)v=N::merge(v,v);
		}
	};
	auto pow_x=[&](N v,int k,N&x){
		while(k){
			if(k&1)x=N::merge(v,x);
			if(k>>=1)v=N::merge(v,v);
		}
	};
	
	auto [an,bn]=abnum(a,b,n);
	N ans;
	while(an||bn){
		assert(an>=0);
		assert(bn>=0);
		assert(inc(0,c,a+b));
		if(a==0){
			assert(b==1);
			assert(bn==0);
			pow_x(0<c?ax:ay,an,ans);
			break;
		}
		if(b==0){
			assert(a==1);
			assert(an==0);
			pow_x(0<c?bx:by,bn,ans);
			break;
		}
		assert(a>0);
		assert(b>0);
		if(a<b){
			{
				int fin=(a*an-b*bn);
				int p=fin/a,q=fin%a;
				int u=min(p,cld(c-q,a));
				pow_x(ay,p-u,ans);
				pow_x(ax,u,ans);
				an-=p;
				fin-=a*p;
				assert(an>=0);
			}
			int p=b/a,q=b%a;
			if(c<=q){
				pow_x(ay,p,bx);
				pow_x(ay,p,by);
			}else{
				auto sub=[&](int z){
					int u=min(p,cld(c-z,a));
					N res=z+p*a<c?bx:by;
					pow_x(ay,p-u,res);
					pow_x(ax,u,res);
					return res;
				};
				int cnx=q+(c-q)%a;
				N bxnx=sub(q);
				N bynx=sub(cnx);
				bx=bxnx;
				by=bynx;
				c=cnx;
			}
			b-=a*p;
			an-=bn*p;
		}else{
			{
				int fin=(a*an-b*bn);
				int p=(a+b-1-fin)/b;
				int u=min(p,cld(fin+b*p+1-c,b));
				pow_x(bx,p-u,ans);
				pow_x(by,u,ans);
				bn-=p;
				fin+=b*p;
				assert(bn>=0);
				ans=N::merge(fin-a<c?ax:ay,ans);
				an--;
				fin-=a;
				assert(an>=0);
			}
			int p=a/b,q=a%b;
			if(c<=b+q){
				x_pow(ax,by,p);
				x_pow(ay,by,p);
			}else{
				auto sub=[&](int z){
					N res=z<c?ax:ay;
					int u=min(p,cld(z+a+1-c,b));
					x_pow(res,by,u);
					x_pow(res,bx,p-u);
					return res;
				};
				int cnx=(c-q)%b;
				N axnx=sub(0);
				N aynx=sub(cnx);
				ax=axnx;
				ay=aynx;
				by=bx;
				c=cnx;
			}
			a-=b*p;
			bn-=an*p;
		}
	}
	return ans;
}

struct N{
	M m;
	N():m(M::getid()){}
	N(const M&mm):m(mm){}
	static N merge(const N&a,const N&b){
		return b.m*a.m;
	}
};

M randM(){
	M m;
	rep(i,S)rep(j,S)m[i][j]=rand_int(mint::mod);
	return m;
}

struct W{
	string s;
	W(){}
	W(const string&ss):s(ss){}
	static W merge(const W&a,const W&b){
		return a.s+b.s;
	}
};
