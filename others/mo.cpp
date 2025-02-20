template<class Q>void sort_mo(vc<Q>&qs){
	const int B=334;
	sort(all(qs),[&](const Q&a,const Q&b){
		int x=a.l/B,y=b.l/B;
		if(x!=y)return x<y;
		return a.r<b.r;
	});
}


	auto clear=[&](){
		x.clear();
	};
	auto add=[&](int i){
		x.add(idbuf[i],1);
	};
	auto del=[&](int i){
		x.add(idbuf[i],-1);
	};
	vc<ll> ans(q);
	sort_mo(qs);
	int block=-1,b=0,e=0;
	for(auto z:qs){
		{
			int cur=z.l/B;
			if(block!=cur){
				block=cur;
				b=e=block*B;
				clear();
			}
		}
		while(z.l<b){
			add(--b);
		}
		while(e<z.r){
			add(e++);
		}
		while(b<z.l){
			del(b++);
		}
		while(z.r<e){
			del(--e);
		}
		int p0=x.getid(a[z.l]);
		int p1=x.getid(-a[z.r]);
		x.add(p0,1);
		x.add(p1,1);
		ans[z.i]=x.getans(z.k);
		x.add(p0,-1);
		x.add(p1,-1);
	}


//push pop で実装
//Codechef 2022 June Lunchtime E
	int block=-1,b=0,e=0;
	for(auto z:qs){
		{
			int cur=z.l/B;
			if(block!=cur){
				block=cur;
				b=e=(block+1)*B;
				init();
			}
		}
		
		if(z.r<=(block+1)*B){
			rng(i,z.l,z.r)push(i);
			res[z.i]=ans[t];
			rng(i,z.l,z.r)pop();
		}else{
			assert(e<=z.r);
			while(e<z.r)push(e++);
			assert(z.l<=b);
			rng(i,z.l,b)push(i);
			res[z.i]=ans[t];
			rng(i,z.l,b)pop();
		}
	}
	
	
//query [l,r)
//sum l<=i<j<r f(i,j)
//(allowsamelr=true だと l=r も含めて数える)
//もちろんこれだと NQ 回必要なので
//left (i,x)
//sum 0<=j<i f(x,j)
//right (i,x)
//sum 0<=j<i f(j,x)
//に答えてもらう
//prefix に対してクエリを投げる形に限定しているわけです
//push(0),push(1),...,push(n-1)
//で進めていく
//left(x),right(x) で答えをもらう
//f(i,i) の扱いに注意してね
//6th Turing Cup B
//stress-tested
template<class RAW,class PUSH,class LEFT,class RIGHT>
auto mo_by_prefix_query(vc<RAW> raw,vc<pi> lr,bool allowsamelr,PUSH push,LEFT left,RIGHT right){
	int n=si(raw);
	using t=typename invoke_result<LEFT,RAW>::type;
	static_assert(is_same<t,typename invoke_result<RIGHT,RAW>::type>::value);
	
	int m=si(lr);
	// N^2/B/2+MB/2 回のクエリ
	const int B=max<int>(n/sqrt(max<int>(m,1)),1);
	
	vi idx=vid(m);
	{
		vi cnt(n+1);
		for(auto [l,r]:lr)cnt[r]++;
		vi off=presum<int>(cnt);
		vi tmp(m);
		for(auto i:idx)tmp[off[lr[i].b]++]=i;
		tmp.swap(idx);
	}
	{
		vi cnt(n/B+1);
		for(auto [l,r]:lr)cnt[l/B]++;
		vi off=presum<int>(cnt);
		vi tmp(m);
		for(auto i:idx)tmp[off[lr[i].a/B]++]=i;
		tmp.swap(idx);
	}
	
	vvc<T> lq(n+1),rq(n+1);
	{
		int b=0,e=0;
		rep(i,m){
			auto [l,r]=lr[idx[i]];
			rq[b].eb(e,r,i);
			e=r;
			lq[e].eb(b,l,i);
			b=l;
		}
	}
	
	vc<t> prer(n+1),prel(n+1);
	vc<t> dif(m);
	rep(i,n+1){
		for(auto [l,r,k]:lq[i]){
			rng(j,l,r)dif[k]+=left(raw[j]);
			rng(j,r,l)dif[k]-=left(raw[j]);
		}
		for(auto [l,r,k]:rq[i]){
			rng(j,l,r)dif[k]+=right(raw[j]);
			rng(j,r,l)dif[k]-=right(raw[j]);
		}
		if(i<n){
			if(allowsamelr){
				prel[i+1]=prel[i]+left(raw[i]);
				push(raw[i]);
				prer[i+1]=prer[i]+right(raw[i]);
			}else{
				prer[i+1]=prer[i]+right(raw[i]);
				push(raw[i]);
				prel[i+1]=prel[i]+left(raw[i]);
			}
		}
	}
	
	rep(j,m-1)dif[j+1]+=dif[j];
	vc<t> ans(m);
	rep(i,m){
		auto [l,r]=lr[idx[i]];
		ans[idx[i]]=prer[r]+prel[l]-dif[i];
	}
	
	return ans;
}
