const int vmax=300010;
mint pf[vmax];
void initpf(){
	pf[0]=1;
	rng(i,1,vmax){
		for(int k=1;;k++){
			{
				int s=k*(3*k-1)/2;
				if(s>i)break;
				if(k&1)pf[i]+=pf[i-s];
				else pf[i]-=pf[i-s];
			}
			{
				int s=k*(3*k+1)/2;
				if(s>i)break;
				if(k&1)pf[i]+=pf[i-s];
				else pf[i]-=pf[i-s];
			}
		}
	}
}

//[0,n)
Poly<mint> partition_number(int n){
	Poly<mint> w(n);
	w[0]=1;
	for(int k=1;;k++){
		{
			int s=k*(3*k-1)/2;
			if(s>=n)break;
			if(k&1)w[s]=-1;
			else w[s]=1;
		}
		{
			int s=k*(3*k+1)/2;
			if(s>=n)break;
			if(k&1)w[s]=-1;
			else w[s]=1;
		}
	}
	return w.inv(n);
}
