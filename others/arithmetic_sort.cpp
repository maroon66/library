//UCUP 2-27-F

vi arithmetic_sort(vi a,int d,int k){
	int n=si(a);
	soin(a);
	int head=1;
	vi to=vid(n);
	int cur=0;
	vi res(k);
	rep(i,k){
		res[i]=a[cur];
		a[cur]+=d;
		int nx=to[cur];
		if(head<n&&a[nx]>a[head]){
			to[cur]=head;
			to[head]=nx;
			head++;
		}
		cur=to[cur];
	}
	return res;
}

template<class F>
void arithmetic_sort_do(vi a,int d,int k,F f){
	int n=si(a);
	soin(a);
	int head=1;
	vi to=vid(n);
	int cur=0;
	rep(i,k){
		f(a[cur]);
		a[cur]+=d;
		int nx=to[cur];
		if(head<n&&a[nx]>a[head]){
			to[cur]=head;
			to[head]=nx;
			head++;
		}
		cur=to[cur];
	}
}
