
int inversions(vi perm){
	int n=si(perm),ans=0;
	BIT<int> bit(n);
	rep(i,n){
		ans+=i-bit.get(perm[i]);
		bit.add(perm[i],1);
	}
	return ans;
}
