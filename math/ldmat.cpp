//sweep がやばいと話題に
//modmat を参考に直そうね
//多分直ってる

using ld=long double;
const ld eps=1e-9;
bool is0(ld x){return -eps<=x&&x<=eps;}

//左c列をsweepしする
//aの先頭rank行に要素が入っている感じ
//掃き出しに使った列の番号が返る
vi sweep(vvc<ld>&a,int c=-1){
	if(a.empty())return {};
	if(c==-1)c=a[0].size();
	int h=a.size(),w=a[0].size(),r=0;
	vi res;
	rep(i,c){
		if(r==h)break;
		pair<ld,int> mx(0,-1);
		rng(j,r,h)chmax(mx,make_pair(abs(a[j][i]),j));
		if(is0(mx.a))continue;
		swap(a[r],a[mx.b]);
		rep(j,h)if(j!=r){
			ld z=-a[j][i]/a[r][i];
			rng(k,i,w)
				a[j][k]+=a[r][k]*z;
		}
		res.pb(i);
		r++;
	}
	return res;
}

//解を一つ見つける
//解空間？kernelでも使っとけ
//aが空のときに備えてaの列の個数wを与える
//解が見つからないときは {} を返す
vc<ld> lineareq(vvc<ld> a,int w,vc<ld> b){
	int h=a.size();
	rep(i,h)a[i].pb(b[i]);
	vi idx=sweep(a,w);
	rng(i,idx.size(),h)
		if(!is0(a[i][w]))return {};
	vc<ld> res(w);
	rep(i,idx.size())
		res[idx[i]]=a[i][w]/a[i][idx[i]];
	return res;
}

//ker(a)を返す
//aが空のときに備えてaの列の個数wを与える
vvc<ld> kernel(const vvc<ld>&a,int w){
	int h=a.size();
	vvc<ld> b(w,vc<ld>(h+w));
	rep(i,h)rep(j,w)b[j][i]=a[i][j];
	rep(i,w)b[i][h+i]=1;
	int r=sweep(b,h).size();
	vvc<ld> res;
	rng(i,r,w)res.eb(b[i].bg+h,b[i].ed);
	return res;
}

//正方行列でないと動かない
vvc<ld> mult(const vvc<ld>&a,const vvc<ld>&b){
	int n=si(a);
	vvc<ld> c(n,vc<ld>(n,0));
	rep(i,n)rep(j,n)rep(k,n)c[i][j]+=a[i][k]*b[k][j];
	return c;
}

vvc<ld> mpow(vvc<ld>&a,int x){
	int n=si(a);
	vvc<ld> res(n,vc<ld>(n));
	rep(i,n)res[i][i]=1;
	while(x){
		if(x&1)res=mult(res,a);
		a=mult(a,a);
		x>>=1;
	}
	return res;
}

vvc<ld> inverse(vvc<ld> a){
	int n=si(a);
	rep(i,n){
		a[i].resize(2*n);
		a[i][n+i]=1;
	}
	auto tmp=sweep(a,n);
	assert(si(tmp)==n);
	vvc<ld> b(n,vc<ld>(n));
	rep(i,n)rep(j,n)b[i][j]=a[i][n+j]/a[i][i];
	return b;
}

vc<ld> multvec(const vvc<ld>&a,const vc<ld>&b){
	int n=si(a),m=si(b);
	vc<ld> c(n);
	rep(i,n)rep(j,m)c[i]+=a[i][j]*b[j];
	return c;
}

void showm(const vvc<ld>&a){
	for(auto v:a)
		cerr<<v<<endl;
}
