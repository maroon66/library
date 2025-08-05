//最初に区間の集合 A と座標幅を与える
//また別の区間の集合 B を持っている
//B に区間を add/del していく
//add/del がちゃんとしている (del の前に add がある) 状態でないと壊れます
//B のいずれかの区間と共通部分（長さ正）をもつ区間 (in A) のうち番号最小のものを返す
//stress-tested
struct range_detector{
	struct N{
		vi idx;
		int I,B;
		int Imin,Bsum,val;
		N():B(0){}
	};
	int n,s,m;
	vc<pi> ls;
	vc<bool> avail;
	vc<N> x;
	void single(int i){
		x[i].I=inf;
		while(si(x[i].idx)){
			int j=x[i].idx.back();
			if(avail[j]){
				x[i].I=j;
				break;
			}else{
				x[i].idx.pop_back();
			}
		}
		x[i].Imin=x[i].I;
		x[i].Bsum=x[i].B;
		x[i].val=x[i].B?x[i].I:inf;
	}
	void upd(int i){
		single(i);
		if(i<s){
			rep(k,2){
				int j=i*2+k;
				chmin(x[i].Imin,x[j].Imin);
				x[i].Bsum+=x[j].Bsum;
				if(x[j].Bsum)chmin(x[i].val,x[i].I);
				if(x[i].B)chmin(x[i].val,x[j].Imin);
				chmin(x[i].val,x[j].val);
			}
		}
	}
	range_detector(int nn,vc<pi> raw):n(nn),s(minp2(n)),m(si(raw)),ls(raw),avail(m,true),x(2*s){
		per(v,si(ls)){
			auto [l,r]=ls[v];
			segeasy(s,l,r,[&](int i){
				x[i].idx.pb(v);
			});
		}
		gnr(i,1,2*s)upd(i);
	}
	void addB(int l,int r){
		segeasy2(s,l,r,
		[&](int i){
			x[i].B++;
			upd(i);
		},
		[&](int i){
			upd(i);
			});
	}
	void delB(int l,int r){
		segeasy2(s,l,r,
		[&](int i){
			x[i].B--;
			upd(i);
		},
		[&](int i){
			upd(i);
			});
	}
	int getmin(){
		int res=x[1].val;
		if(res==inf)return -1;
		else return res;
	}
	void delA(int v){
		assert(avail[v]);
		avail[v]=false;
		auto [l,r]=ls[v];
		segeasy2(s,l,r,
		[&](int i){
			upd(i);
		},
		[&](int i){
			upd(i);
			});
	}
};
