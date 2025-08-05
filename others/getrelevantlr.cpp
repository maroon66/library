//assume z is strictly ascending
//z に重要な点が入っていて
//問題を解くべき区間 [l,r] というのは，[l,r] 内の重要な点の個数の密度が 1/k 以上
//という状況の時，座標圧縮に似たことができて，必要な区間を列挙する．
//線形時間
//おそらく必要な区間を半開区間で返している
//たとえば z={1} のときは，[0,3) を返す
//UCUP 3-33-H
vc<pi> getlr(int k,vi z){
	auto getval=[&](int i){return i*k-z[i];};
	vc<pi> hs;
	rep(i,si(z)){
		int lw=getval(i),up=lw+k-1;
		while(si(hs)&&hs.back().a<=up){
			chmin(lw,hs.back().a);
			chmax(up,hs.back().b);
			hs.pop_back();
		}
		hs.eb(lw,up);
	}
	int head=0;
	vc<pi> lr;
	for(auto [lw,up]:hs){
		int l=z[head]-(up-getval(head));
		while(head<si(z)&&lw<=getval(head))head++;
		int r=z[head-1]+1+(getval(head-1)+k-1-lw);
		lr.eb(l,r);
	}
	return lr;
}
