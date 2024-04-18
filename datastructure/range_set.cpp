template<class t>
struct range_set{
	const t none;
	range_set(t val):none(val){}
	map<pi,t> s;
	void set(int l,int r,t val){
		assert(l<=r);
		if(l>=r)return;
		auto itr=s.lower_bound(pi(r,-inf));
		while(itr!=s.bg){
			--itr;
			auto [x,y]=itr->a;
			assert(x<r);
			if(y<=l)break;
			t z=itr->b;
			itr=s.erase(itr);
			//common
			int u=max(l,x);
			int v=min(r,y);
			if(v<y)itr=s.emplace(pi(v,y),z).a;
			if(x<u){
				s[pi(x,u)]=z;
				break;
			}
		}
		s[pi(l,r)]=val;
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
