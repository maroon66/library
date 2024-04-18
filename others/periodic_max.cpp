//MIPT-2016 Pre-Finals Workshop, Taiwan NTU Contest G
//上がって下がる
//最大や最小では同じ値を 2 個までとっても許される
//それ以外は隣あうところは全部違う値にしてくれ
template<class t,class F>
int periodic_max(const vc<t>&v,F f_){
	int n=v.size();
	if(n==1)return 0;
	
	auto f=[&](int i){
		return f_(v[i%n]);
	};
	
	auto h=f(0);
	bool up=h<f(1);
	
	int lb=0,ub=n;
	while(ub-lb>1){
		int m=(ub+lb)/2;
		if(f(m)<h){
			if(up)ub=m;
			else lb=m;
		}else{
			if(f(m)<f(m+1))lb=m;
			else ub=m;
		}
	}
	
	return f(lb)<f(ub)?ub%n:lb%n;
}

//上がって下がる
//最大や最小では同じ値を 2 個までとっても許される
//それ以外は隣あうところは全部違う値にしてくれ
//f(a,b) は単に calc(a)<calc(b) を返す
template<class F>
int periodic_max_cmp(int n,F f){
	if(n==1)return 0;
	
	//auto h=f(0);
	//bool up=h<f(1);
	bool up=f(0,1);
	
	int lb=0,ub=n;
	while(ub-lb>1){
		int m=(ub+lb)/2;
		//if(f(m)<h){
		if(f(m%n,0)){
			if(up)ub=m;
			else lb=m;
		}else{
			//if(f(m)<f(m+1))lb=m;
			if(f(m%n,(m+1)%n))lb=m;
			else ub=m;
		}
	}
	
	//return f(lb)<f(ub)?ub%n:lb%n;
	return f(lb%n,ub%n)?ub%n:lb%n;
}

