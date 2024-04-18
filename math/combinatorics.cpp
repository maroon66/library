//SRM 772 Hard
//sum (n choose i) i^k
//O(k log k)
mint sum_nci_i_kth(int n,int k){
	mint res;
	
	/*rep(i,n+1)
		res+=choose(n,i)*mint(i).pow(k);
	return res;*/
	
	if(n<=k){
		rep(i,n+1)
			res+=choose(n,i)*mint(i).pow(k);
	}else{
		mint nci=1,a=0;
		vc<mint> b(k+1);
		b[k]=1;
		per(i,k)
			b[i]=b[i+1]*(n-1-i)*invs[k-i];
		rep(i,k+1){
			a+=nci*mint(i).pow(k);
			mint w=a*b[i];
			if((k-i)%2==0)
				res+=w;
			else
				res-=w;
			nci*=(n-i);
			nci*=invs[i+1];
		}
		res*=mint(2).pow(n-k);
	}
	return res;
}

//SRM 772 Hard
//sum (n choose i)(i-1)(i-2)...(i-a)
//O(a+log n)
mint sum_nci_falling_factorial_power(int n,int a){
	mint res=0;
	
	/*rep(i,n+1){
		mint w=choose(n,i);
		rng(j,1,a+1)
			w*=i-j;
		res+=w;
	}
	return res;*/
	
	mint w=mint(2).pow(n)*fact[a];
	
	per(i,a+1){
		if(i%2==0)
			res+=w;
		else
			res-=w;
		w*=invs[a+1-i];
		w*=invs[2];
		w*=(n-(a-i));
	}
	
	return res;
}

//SRM 772 Hard
//sum (n choose i) / (i+a)
//O(a log n)
mint sum_nci_over_i_plus_a(int n,int a){
	mint res;
	
	/*rep(i,n+1){
		res+=choose(n,i)*invs[i+a];
	}
	return res;*/
	
	res=sum_nci_falling_factorial_power(n+a,a-1);
	
	if(a%2==0)res+=fact[a-1];
	else res-=fact[a-1];
	
	rng(i,1,a+1)
		res/=n+i;
	
	return res;
}
