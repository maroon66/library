//UCUP 3-31-F
template<class t>
struct range_set{
	const t none;
	template<class u>
	range_set(u val):none(val){}
	map<pi,t> s;
	template<class F>
	void del(int l,int r,F f){
		assert(l<=r);
		if(l>=r)return;
		auto itr=s.lower_bound(pi(l,-inf));
		if(itr!=s.bg){
			itr--;
			auto [x,y]=itr->a;
			assert(x<l);
			if(l<y){
				t val=itr->b;
				if(r<y){
					f(l,r,val);
					itr=s.erase(itr);
					add(x,l,val);
					add(r,y,val);
					return;
				}else{
					f(l,y,val);
					itr=s.erase(itr);
					add(x,l,val);
				}
			}else{
				itr++;
			}
		}
		while(itr!=s.ed){
			auto [x,y]=itr->a;
			if(r<=x)break;
			t val=itr->b;
			if(y<=r){
				f(x,y,val);
				itr=s.erase(itr);
			}else{
				f(x,r,val);
				itr=s.erase(itr);
				add(r,y,val);
				break;
			}
		}
	}
	void add(int l,int r,t val){
		assert(l<=r);
		if(l<r)s[pi(l,r)]=val;
	}
	t get(int i){
		auto itr=s.lower_bound(pi(i+1,-inf));
		if(itr!=s.bg){
			itr--;
			auto [x,y]=itr->a;
			assert(x<=i);
			if(i<y){
				return itr->b;
			}
		}
		return none;
	}
};
