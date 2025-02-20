const int S=2010;
using B=bitset<S>;

bool add(vc<B>&vs,B x){
	for(const auto&v:vs){
		int i=v._Find_first();
		if(i<S&&x[i])x^=v;
	}
	if(x.any()){
		vs.pb(x);
		return true;
	}else{
		return false;
	}
}

//左c列をsweepしする
//aの先頭rank行に要素が入っている感じ
//掃き出しに使った列の番号が返る
vi sweep(vc<B>&a,int w,int c=-1){
	if(a.empty())return {};
	if(c==-1)c=w;
	int h=si(a),r=0;
	vi res;
	rep(i,c){
		if(r==h)break;
		rng(j,r,h)if(a[j][i]){
			swap(a[r],a[j]);
			break;
		}
		if(!a[r][i])continue;
		rep(j,h)if(j!=r&&a[j][i]){
			a[j]^=a[r];
		}
		res.pb(i);
		r++;
	}
	return res;
}

pair<bool,B> lineareq(vc<B> a,int w,B b){
	int h=si(a);
	rep(i,h)
		a[i][w]=b[i];
	vi idx=sweep(a,w+1,w);
	cerr<<si(idx)<<endl;
	rng(i,si(idx),h)
		if(a[i][w])
			return mp(false,B());
	B res;
	rep(i,si(idx))
		res[idx[i]]=a[i][w];
	return mp(true,res);
}

//ls の subset であって xor=tar になるものを発見して返す
vc<ull> find_subset(vc<ull> ls,ull tar){
	assert(si(ls)+1<=S);
	const int n=64;
	vc<B> a(n);
	rep(i,n){
		rep(j,si(ls))if((ls[j]>>i)&1)a[i][j]=1;
	}
	B b=tar;
	auto [ok,sol]=lineareq(a,si(ls),b);
	assert(ok);
	vc<ull> res;
	rep(i,si(ls))if(sol[i])res.pb(ls[i]);
	return res;
}
