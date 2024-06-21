//長さ N,要素 0-M-1 のソート列を全探索し，F を呼ぶ
template<class F>
void sorted_array_rec(int n,int m,F f){
	vi a(n);
	auto rec=[&](auto self,int i)->void{
		if(i==n)f(a);
		else{
			int last=i?a[i-1]:0;
			rng(nx,last,m){
				a[i]=nx;
				self(self,i+1);
			}
		}
	};
	rec(rec,0);
}

//UCUP 2-27-B
//[0,m) の値を distinct に n 個取ってきたやつを並べる
template<class F>
void sorted_distinct_rec(int n,int m,F f){
	sorted_array_rec(n,m-n+1,[&](vi a){
		rep(i,n)a[i]+=i;
		f(a);
	});
}

//vs から k 要素選んだ tuple すべてに対して f を呼ぶ
//ARC177 E
template<class t,class F>
void tuple_rec(const vc<t>&vs,int k,F f){
	int n=si(vs);
	if(k<=n){
		vc<t> a(k);
		sorted_array_rec(k,n-k+1,[&](const vi&idx){
			rep(i,k)a[i]=vs[idx[i]+i];
			f(a);
		});
	}
};

template<class F>
void array_rec(int n,int m,F f){
	vi a(n);
	function<void(int)> rec=[&](int i)->void{
		if(i==n)f(a);
		else{
			rep(nx,m){
				a[i]=nx;
				rec(i+1);
			}
		}
	};
	rec(0);
}

//辞書順に呼ばれる
template<class F>
void given_sum_array_rec(int n,F f){
	assert(n>0);
	vi a;
	auto dfs=[&](auto self,int s)->void{
		assert(s>0);
		int mn=1;
		if(si(a))mn=a.back();
		assert(mn<=s);
		int mx=s/2;
		rng(v,mn,mx+1){
			a.pb(v);
			self(self,s-v);
			a.pop_back();
		}
		a.pb(s);
		f(a);
		a.pop_back();
	};
	dfs(dfs,n);
}

template<class F>
void given_sum_array_rec(int n,F f){
	vi a(n),b;
	auto dfs=[&](auto self,int i,int s)->void{
		if(i==n){
			assert(s==0);
			//f(a);
			
			//positive numbers
			b.clear();
			for(auto v:a)if(v)b.pb(v);
			f(b);
		}else{
			int mn=0;
			if(i)mn=a[i-1];
			if(i==n-1){
				assert(mn<=s);
				mn=s;
			}
			int mx=s/(n-i);
			rng(v,mn,mx+1){
				a[i]=v;
				self(self,i+1,s-v);
			}
		}
	};
	dfs(dfs,0,n);
}

template<class F> void permrep(int n,F f){
	vi p(n);iota(all(p),0);
	do{
		f(p);
	}while(next_permutation(all(p)));
};

//all true -> true, otherwise false
template<class F> bool permrep_and(int n,F f){
	vi p(n);iota(all(p),0);
	do{
		if(!f(p))return false;
	}while(next_permutation(all(p)));
	return true;
};
//all false -> false, otherwise true
template<class F> bool permrep_or(int n,F f){
	vi p(n);iota(all(p),0);
	do{
		if(f(p))return true;
	}while(next_permutation(all(p)));
	return false;
};
