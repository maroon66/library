//[l,r)
//GCJ 2023 D D
//UCUP 3-27-H
void common_ranges(vc<pi>&a,const vc<pi>&b){
	static vi buf;buf.clear();
	for(auto [l,r]:a){
		buf.pb(l*2);
		buf.pb(r*2+1);
	}
	for(auto [l,r]:b){
		buf.pb(l*2);
		buf.pb(r*2+1);
	}
	sort(all(buf));
	a.clear();
	int left,cur=0;
	for(auto xk:buf){
		int x=xk>>1,k=xk&1;
		if(k==0){
			cur++;
			if(cur==2)left=x;
		}else{
			if(cur==2&&left<x)a.eb(left,x);
			cur--;
		}
	}
}


//[l,r)
//Yandex Finals 2024 C
void meld(vc<pi>&vs){
	soin(vs);
	int s=0;
	for(auto&[l,r]:vs){
		if(s&&vs[s-1].b>=l){
			chmax(vs[s-1].b,r);
		}else{
			vs[s++]={l,r};
		}
	}
	vs.resize(s);
}

//inclusie range
//差分 2 以下の range をまとめる
void meld(vc<pi>&vs){
	soin(vs);
	int s=0;
	for(auto&[l,r]:vs){
		if(s&&vs[s-1].b+2>=l){
			chmax(vs[s-1].b,r);
		}else{
			vs[s++]={l,r};
		}
	}
	vs.resize(s);
}

//UCUP2-7-E
void meld(vc<R>&vs){
	soin(vs);
	int s=0;
	for(auto&[l,r]:vs){
		if(s&&vs[s-1].r+1>=l){
			chmax(vs[s-1].r,r);
		}else{
			vs[s++]={l,r};
		}
	}
	vs.resize(s);
}
