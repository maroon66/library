//strictly increasing sequence
int getlis(vi x){
	int n=si(x),ans=0;
	vi buf(n,inf);
	for(auto v:x){
		int i=lwb(buf,v);
		chmax(ans,i+1);
		buf[i]=v;
	}
	return ans;
}
