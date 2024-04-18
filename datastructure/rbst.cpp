//ノードの push は副作用なし!
//いちいち merge することで全てを解決する
//各ノードは半開区間に対応すると思って、split の際にはその範囲で破壊されるかどうかを判定する（特に、左端で破壊されるなら true、右端で破壊されるなら false）
//ARC033 C
//AOJ DSL2G
template<class N>
struct rbst{
	struct N2{
		N rw,mg;
		int s,l,r;
		template<class t>
		N2(t v):rw(v),mg(v),s(1),l(-1),r(-1){}
	};
	vc<N2> x;
	int upd(int i){
		x[i].mg=x[i].rw.parent();
		x[i].s=1;
		int l=x[i].l,r=x[i].r;
		if(l!=-1){
			x[i].mg=N::merge(x[l].mg,x[i].mg);
			x[i].s+=x[l].s;
		}
		if(r!=-1){
			x[i].mg=N::merge(x[i].mg,x[r].mg);
			x[i].s+=x[r].s;
		}
		return i;
	}
	void push(int i){
		x[i].mg.push(x[i].rw);
		int l=x[i].l,r=x[i].r;
		if(l!=-1)
			x[i].mg.push(x[l].mg);
		if(r!=-1)
			x[i].mg.push(x[r].mg);
	}
	template<class...Args>
	int nn(Args...args){
		int res=x.size();
		x.eb(args...);
		return res;
	}
	template<class t>
	int build(vc<t> v){
		vi cur;
		for(auto z:v)cur.pb(nn(z));
		while(cur.size()>1){
			int s=cur.size();
			vi nx((s+1)/2);
			for(int i=0;i<s;i+=2){
				if(i+1<s)nx[i/2]=merge(cur[i],cur[i+1]);
				else nx[i/2]=cur[i];
			}
			swap(nx,cur);
		}
		return cur[0];
	}
	int merge(int a,int b){
		if(a==-1)return b;
		if(b==-1)return a;
		int s=x[a].s+x[b].s,w=rand_int(0,s-1);
		if(w<x[a].s){
			push(a);
			x[a].r=merge(x[a].r,b);
			return upd(a);
		}else{
			push(b);
			x[b].l=merge(a,x[b].l);
			return upd(b);
		}
	}
	template<class F,class...Args>
	pi split(int i,F f,Args&&...args){
		if(i==-1)return pi(-1,-1);
		if(!(x[i].mg.*f)(args...))return pi(i,-1);
		push(i);
		pi z=split(x[i].l,f,forward<Args>(args)...);
		if(z.b!=-1){
			x[i].l=z.b;
			return pi(z.a,upd(i));
		}
		if((x[i].rw.*f)(args...)){
			x[i].l=-1;
			return pi(z.a,upd(i));
		}
		z=split(x[i].r,f,forward<Args>(args)...);
		x[i].r=z.a;
		return pi(upd(i),z.b);
	}
	pi splitsize(int i,int s){
		if(i==-1)return pi(-1,-1);
		if(x[i].s<=s)return pi(i,-1);
		int l=x[i].l,ls=l==-1?0:x[l].s;
		push(i);
		if(s<ls){
			pi z=splitsize(l,s);
			x[i].l=z.b;
			return pi(z.a,upd(i));
		}
		if(s==ls){
			x[i].l=-1;
			return pi(l,upd(i));
		}
		pi z=splitsize(x[i].r,s-(ls+1));
		x[i].r=z.a;
		return pi(upd(i),z.b);
	}
	N& operator[](int i){
		return x[i].mg;
	}
	//NOT VERIFIED
	void dfs(int i,vc<N>&buf){
		if(i==-1)return;
		push(i);
		upd(i);
		dfs(x[i].l,buf);
		buf.pb(x[i].rw);
		dfs(x[i].r,buf);
	}
	//NOT VERIFIED
	vc<N> enumerate(int i){
		vc<N> res;
		dfs(i,res);
		return res;
	}
	//NOT VERIFIED
	int right(int i){
		if(x[i].r==-1)return i;
		else return right(x[i].r);
	}
	//NOT VERIFIED
	int left(int i){
		if(x[i].l==-1)return i;
		else return left(x[i].l);
	}
};

//区間 add,sum
struct N{
	int lz,sum,len;
	N(int s=0,int l=1):lz(0),sum(s),len(l){}
	N parent(){
		return N(sum,len);
	}
	static N merge(N a,N b){
		return N(a.sum+b.sum,a.len+b.len);
	}
	void add(int v){
		lz+=v;
		sum+=len*v;
	}
	void push(N&x){
		x.add(lz);
	}
};
