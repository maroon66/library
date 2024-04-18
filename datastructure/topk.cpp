//https://pjudge.ac/contest/892/problem/21622
//CF872E
struct TopK{
	int k,ans;
	multiset<int> lw,up;
	TopK(int kk):k(kk),ans(0){}
	void add(int v){
		ans+=v;
		up.insert(v);
		if(si(up)>k){
			auto itr=up.bg;
			ans-=*itr;
			lw.insert(*itr);
			up.erase(itr);
		}
	}
	void del(int v){
		auto itr=lw.find(v);
		if(itr==lw.ed){
			itr=up.find(v);
			assert(itr!=up.ed);
			ans-=*itr;
			up.erase(itr);
			if(si(lw)){
				itr=prev(lw.ed);
				ans+=*itr;
				up.insert(*itr);
				lw.erase(itr);
			}
		}else{
			lw.erase(itr);
		}
	}
	int enough(){
		assert(si(up)<=k);
		return si(up)==k;
	}
	int getval(){
		//assert(si(up)>=k);
		assert(si(up)==k);
		return ans;
	}
	void changek(int newk){
		assert(0<=newk);
		k=newk;
		while(si(up)<k){
			if(si(lw)){
				auto itr=prev(lw.ed);
				up.insert(*itr);
				ans+=*itr;
				lw.erase(itr);
			}else break;
		}
		while(si(up)>k){
			auto itr=up.bg;
			ans-=*itr;
			lw.insert(*itr);
			up.erase(itr);
		}
	}
};
