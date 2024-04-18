//requires add/min seglazy
//verify yosupo
//https://codeforces.com/blog/entry/78898
struct permutation_tree{
	struct Node{//k=0,1,2,3 = leaf,linear(inc),linear(dec),prime
		int k,l,r,lo,hi;
		vc<Node*> c;
	};
	using np=Node*;
	int n,vs=0;
	unique_ptr<Node[]> w;
	np nn(int k,int l,int r,int lo,int hi){
		w[vs]={k,l,r,lo,hi,{}};
		return w.get()+vs++;
	}
	np root;
	permutation_tree(vi p):n(si(p)),w(new Node[n*2]){
		vi tmp(n);iota(all(tmp),0);
		seglazy<N> seg(tmp);
		vc<np> s;
		vi x{-1},y{-1};
		rep(i,n){
			while(1){
				int j=x.back();
				if(j==-1||p[j]<p[i])break;
				x.pop_back();
				seg.ch(x.back()+1,j+1,&N::add,p[j]-p[i]);
			}
			while(1){
				int j=y.back();
				if(j==-1||p[j]>p[i])break;
				y.pop_back();
				seg.ch(y.back()+1,j+1,&N::add,p[i]-p[j]);
			}
			x.pb(i);
			y.pb(i);
			np cur=nn(0,i,i+1,p[i],p[i]+1);
			while(si(s)){
				{
					np a=s.back();
					if(a->hi==cur->lo){
						s.pop_back();
						if(a->k==1){
							a->r=cur->r;
							a->hi=cur->hi;
							a->c.pb(cur);
							cur=a;
						}else{
							np nx=nn(1,a->l,cur->r,a->lo,cur->hi);
							nx->c.pb(a);
							nx->c.pb(cur);
							cur=nx;
						}
						continue;
					}
					if(a->lo==cur->hi){
						s.pop_back();
						if(a->k==2){
							a->r=cur->r;
							a->lo=cur->lo;
							a->c.pb(cur);
							cur=a;
						}else{
							np nx=nn(2,a->l,cur->r,cur->lo,a->hi);
							nx->c.pb(a);
							nx->c.pb(cur);
							cur=nx;
						}
						continue;
					}
				}
				if(seg.composite(0,cur->l).mn>i)break;
				vc<np> c;
				c.pb(cur);
				int l=cur->l,r=cur->r,lo=cur->lo,hi=cur->hi;
				while(1){
					np a=s.back();s.pop_back();
					c.pb(a);
					l=a->l;
					chmin(lo,a->lo);
					chmax(hi,a->hi);
					if(r-l==hi-lo)break;
				}
				reverse(all(c));
				cur=nn(3,l,r,lo,hi);
				cur->c.swap(c);
			}
			s.pb(cur);
		}
		root=s[0];
	}
	vi getpar(){
		vi res(vs,-1);
		rep(i,vs)for(auto j:w[i].c)
			res[j-w.get()]=i;
		return res;
	}
};
