int inversions(const vi&perm){
	int n=si(perm),ans=0;
	BIT<int> bit(n);
	rep(i,n){
		ans+=i-bit.get(perm[i]);
		bit.add(perm[i],1);
	}
	return ans;
}

int inversions(vi a){
	vi vs=a;
	mkuni(vs);
	int n=si(vs),ans=0;
	BIT<int> bit(n);
	rep(i,n){
		int j=lwb(vs,a[i]);
		ans+=i-bit.get(j);
		bit.add(j,1);
	}
	return ans;
}
