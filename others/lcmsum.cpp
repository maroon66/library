//全区間のLCMの和
//Luogu MX-X10 F
mint107 LCMsum(vi a){
	/*int n=si(a);
	mint107 res;
	rep(i,n){
		int val=1;
		rng(j,i,n){
			val=lcm(val,a[j]);
			res+=val;
		}
	}
	return res;*/
	mint107 res;
	vc<pair<int,mint107>> buf;
	for(auto v:a){
		for(auto&[key,val]:buf){
			key=lcm(key,v);
		}
		buf.eb(v,1);
		unique_freq(buf);
		for(auto [key,val]:buf)
			res+=key*val;
	}
	return res;
}
