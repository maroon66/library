//strictly increasing sequence
int getlis(const vi&x){
	int n=si(x),ans=0;
	vi buf(n,inf);
	for(auto v:x){
		int i=lwb(buf,v);
		chmax(ans,i+1);
		buf[i]=v;
	}
	return ans;
}

//weakly increasing sequence
//ARC179F
vi getlis(vi x){
	int n=si(x),ans=0;
	vi pre(n,-1),buf(n,inf),idx(n,-1);
	rep(i,n){
		int j=lwb(buf,x[i]+1);
		chmax(ans,j+1);
		buf[j]=x[i];
		idx[j]=i;
		if(j>0)pre[i]=idx[j-1];
	}
	vi res;
	if(ans)for(int i=idx[ans-1];i>=0;i=pre[i])res.pb(i);
	rein(res);
	return res;
}
