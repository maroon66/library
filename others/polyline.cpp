//与えられた数列に対応する折れ線を返す
//差分は +,- と変化する
//さらに，差分の絶対値を見た時，x>=y<=z となることがない
vi polyline(vi a){
	vi x;
	for(auto v:a){
		while(si(x)>=2){
			int s=si(x);
			int c=x[s-2],d=x[s-1];
			if(inc(c,d,v)||inc(v,d,c)){
				x.pop_back();
			}else if(s>=3){
				int b=x[s-3];
				if(abs(b-c)>=abs(c-d)&&abs(c-d)<=abs(d-v)){
					x.pop_back();
					x.pop_back();
				}else{
					break;
				}
			}else{
				break;
			}
		}
		if(x.empty()||x.back()!=v)x.pb(v);
	}
	return x;
}
