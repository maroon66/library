struct trie{
	const int L;
	struct node{
		map<int,node*> c;
		node(){}
	};
	
	vc<node> buf;
	using np=node*;
	int head;
	np root;
	
	trie(int L_,int n):L(L_),buf(n*L+1),head(0),root(nn()){}

	np nn(){
		return buf.data()+head++;
	}

	void add(np x,const vi&s,int i){
		if(i==L)return;
		if(!x->c.contains(s[i]))
			x->c[s[i]]=nn();
		add(x->c[s[i]],s,i+1);
	}
	
	void add(const vi&s){
		assert(si(s)==L);
		add(root,s,0);
	}
	
	vi calc(np x,int i){
		if(i==L){
			return vi{0};
		}else{
			vi res;
			for(auto [key,to]:x->c){
				vi w=calc(to,i+1);
				rep(j,si(w))w[j]+=j;
				pb(res,w);
			}
			soin(res);
			return res;
		}
	}
};

//UCUP 3-23-M
	vc<node> t(tot);
	int head=0;
	auto nn=[&](){
		return head++;
	};
	auto nx=[&](int i,int c){
		if(t[i].to[c]==-1){
			t[i].to[c]=nn();
		}
		return t[i].to[c];
	};
	
	int root=nn();
	
	vi ansbuf;
	for(auto s:ss){
		int len=si(s);
		vi idx(len+1);
		idx[0]=root;
		rep(i,len){
			idx[i+1]=nx(idx[i],s[i]-'a');
		}
		rng(i,1,len+1){
			int z=++t[idx[i]].s;
			work(z);
		}
		
		ansbuf.pb(ans);
	}
