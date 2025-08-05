//ICPC Yokohama 2024 J
//同じ位置に点があっても動く
pair<vi,vi> rotate_points_init(const vc<pt>&ps){
	int n=si(ps);
	vi idx=vid(n);
	stable_sort(all(idx),[&](int i,int j){return ps[i]>ps[j];});
	vi pos=invperm(idx);
	return mp(move(idx),move(pos));
}
template<class F>
void rotate_points_work(const vc<pt>&ps,vi&idx,vi&pos,F f){
	int n=si(ps),s=0;
	vi cnt(n);
	for(int i=0;i<n;){
		int j=i;
		while(j<n&&ps[idx[i]]==ps[idx[j]])j++;
		cnt[idx[i]]=j-i;
		i=j;
		s++;
	}
	
	struct Event{
		pt v;
		ll a,b;
		int i,j;
		bool operator<(const Event&r)const{
			int s=argcmp(v,r.v);// (-PI,PI] で計算することを仮定
			if(s)return s<0;
			if(a!=r.a)return a<r.a;
			return b<r.b;
		}
	};
	vc<Event> evs(s*(s-1)/2);evs.clear();
	rep(i,n)rep(j,n)if(cnt[i]&&cnt[j]&&ps[i]!=ps[j]){
		pt dir=normdir(ps[j]-ps[i]);
		evs.pb({rot90(dir),dot(ps[i],dir),dot(ps[j],dir),i,j});
	}
	
	sort(all(evs));
	//half -> eve.resize(s*(s-1)/2);
	
	for(auto eve:evs){
		int i=eve.i,j=eve.j;
		assert(pos[i]+cnt[i]==pos[j]);
		int head=pos[i];
		per(x,cnt[i])rep(y,cnt[j]){
			auto do_swap=[&](){
				swap(idx[head+x+y],idx[head+x+y+1]);
				swap(pos[idx[head+x+y]],pos[idx[head+x+y+1]]);
			};
			f(head+x+y,do_swap);
		}
	}
}
//main
	auto [idx,pos]=rotate_points_init(ps);

	//列のi,i+1番目をswapする
	rotate_points_work(ps,idx,pos,[&](int i,auto do_swap){
		//work
		do_swap();
		//work
	});
