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

