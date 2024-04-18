//CF Hello2022 I
//https://codeforces.com/blog/entry/98654?#comment-874702
//https://codeforces.com/contest/1621/submission/141619060
//construct lyndon decomposition online
//append(c) returns (start position of the lexmin suffix, start position of the maximum repetition of that)
template<class C>
struct lyndon_online{
	vc<C> s;
	int i,j,k;
	vi f,g;
	lyndon_online():i(0),j(0),k(0),f{0},g{0}{}
	pi append(C c){
		s.pb(c);
		while(i<si(s)){
			if(k==i){
				assert(j==k);
				i++;
			}else if(s[k]==s[i]){
				k++;
				i++;
			}else if(s[k]<s[i]){
				k=j;
				i++;
			}else{
				j+=(i-j)/(i-k)*(i-k);
				i=k=j;
			}
		}
		int x,y;
		if((i-j)%(i-k)==0){
			x=k;
			y=j;
		}else{
			x=f[k]+i-k;
			y=g[k]+i-k;
		}
		f.pb(x);
		g.pb(y);
		return pi(x,y);
	}
};
