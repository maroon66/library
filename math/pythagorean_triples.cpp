using T=tuple<int,int,int>;
vc<T> pythagorean_triples(int n){
	vc<T> ans;
	function<void(int,int,int)> dfs=[&](int a,int b,int c){
		if(c>n)return;
		ans.eb(min(a,b),max(a,b),c);
		rep(i,3){
			dfs(a+b*2+c*2,a*2+b+c*2,a*2+b*2+c*3);
			if(i==0){
				a=-a;
			}else if(i==1){
				a=-a;
				b=-b;
			}
		}
	};
	dfs(3,4,5);
	return ans;
}
