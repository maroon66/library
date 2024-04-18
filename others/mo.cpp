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
