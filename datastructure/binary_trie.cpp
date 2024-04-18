JSC2023 D
struct trie{
	struct node{
		node*c[2];
		int cnt;
		node(){
			rep(k,2)c[k]=0;
			cnt=0;
		}
		void upd(){
			cnt=0;
			rep(k,2)if(c[k])cnt+=c[k]->cnt;
		}
	};
	
	const int L;
	vc<node> buf;
	using np=node*;
	int head;
	np root;
	
	trie(int Lini,int n):L(Lini),buf(n*L+1),head(0),root(nn()){}

	np nn(){
		return buf.data()+head++;
	}

	void add(np x,int v,int i){
		if(i==0){
			x->cnt++;
			return;
		}
		i--;
		int k=(v>>i)&1;
		if(x->c[k]==0)
			x->c[k]=nn();
		add(x->c[k],v,i);
		x->upd();
	}
	
	void add(int v){add(root,v,L);}
	
	void rem(np x,int v,int i){
		assert(x);
		assert(x->cnt);
		if(i==0){
			x->cnt--;
			return;
		}
		i--;
		int k=(v>>i)&1;
		rem(x->c[k],v,i);
		x->upd();
	}
	
	void rem(int v){rem(root,v,L);}
	
	int getxormn(np x,int v,int cur,int i){
		if(!x||x->cnt==0)return -1;
		if(i==0)return cur;
		i--;
		rep(z,2){
			int k=((v>>i)&1)^z;
			int w=getxormn(x->c[k],v,cur+(k<<i),i);
			if(w>=0)return w;
		}
		assert(false);
	}
	
	//xor が mn になる相方を探してくる
	int getxormn(int v){
		return getxormn(root,v,0,L);
	}
	
	vi rec(np x,int cur,int i,ll&ans){
		if(i==0)return {cur};
		i--;
		vi res[2];
		rep(k,2)if(x->c[k]){
			res[k]=rec(x->c[k],cur+(k<<i),i,ans);
		}
		int large=si(res[0])<si(res[1]);
		if(si(res[large^1])==0)return res[large];
		int mn=1<<30;
		for(auto v:res[large^1]){
			chmin(mn,v^getxormn(x->c[large],v,cur+(large<<i),i));
		}
		ans+=mn;
		res[large].insert(res[large].ed,all(res[large^1]));
		return move(res[large]);
	}
	
	ll get_xor_mst(){
		ll ans=0;
		rec(root,0,L,ans);
		return ans;
	}
};

//USACO 2020 December Platinum C (区間が交差しないケースだけ)
//add(b,e) で [b,e) の値を追加している
struct trie{
	const int L;
	//cnt: そのノード以下全部，を追加した個数
	//sum: そのノード以下にある値の個数
	struct node{
		node*c[2];
		int a,cnt;
		node(){
			rep(k,2)c[k]=0;
			a=0;
			cnt=0;
		}
		void upd(int lv){
			cnt=a<<lv;
			rep(k,2)if(c[k])cnt+=c[k]->cnt;
		}
		void add(int lv){
			a++;
			cnt+=1<<lv;
		}
	};
	
	vc<node> buf;
	using np=node*;
	int head;
	np root;
	
	trie(int L_,int n):L(L_),buf(n*L*4+1),head(0),root(nn()){}

	np nn(){
		return buf.data()+head++;
	}

	void add(np x,int b,int e,int l,int r,int i){
		if(b<=l&&r<=e){
			x->add(i);
			return;
		}
		rep(k,2){
			int l2=(l+(k==0?l:r))/2;
			int r2=((k==0?l:r)+r)/2;
			if(b<r2&&l2<e){
				if(x->c[k]==0)
					x->c[k]=nn();
				add(x->c[k],b,e,l2,r2,i-1);
			}
		}
		x->upd(i);
	}
	
	void add(int b,int e){add(root,b,e,0,1<<L,L);}
};
