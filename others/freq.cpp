template<class t>
vc<pair<t,int>> to_freq(vc<t> a){
	sort(all(a));
	vc<pair<t,int>> res;
	for(auto v:a){
		if(res.empty()||res.back().a!=v)res.eb(v,0);
		res.back().b++;
	}
	return res;
}
vc<pi> to_freq(vi a){
	sort(all(a));
	vc<pi> res;
	for(auto v:a){
		if(res.empty()||res.back().a!=v)res.eb(v,0);
		res.back().b++;
	}
	return res;
}
template<class F>
void do_freq(vi a,F f){
	int n=si(a);
	sort(all(a));
	for(int i=0;i<n;){
		int j=i;
		while(j<n&&a[i]==a[j])j++;
		f(a[i],j-i);
		i=j;
	}
}
template<class t,class u>
//vc<pi> (val,freq) みたいなやつの val を unique する
void unique_freq(vc<pair<t,u>>&a){
	sort(all(a));
	int s=0;
	rep(i,si(a)){
		auto [p,c]=a[i];
		if(i==0||a[s-1].a<p){
			a[s++]=a[i];
		}else{
			a[s-1].b+=c;
		}
	}
	a.resize(s);
}
//Yukicoder 2575
template<class t,class u,class s>
int lwb_by_first(const vc<pair<t,u>>&a,const s&v){
	auto itr=lower_bound(all(a),v,[&](const pair<t,u>&p,const s&x){return p.a<x;});
	if(itr==a.ed||itr->a!=v)return -1;
	return itr-a.bg;
}
