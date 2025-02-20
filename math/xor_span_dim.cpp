//multiuni2024-8-C
int xor_span_dim(vi x){
	vi y;
	for(auto v:x){
		for(auto w:y)chmin(v,v^w);
		if(v)y.pb(v);
	}
	return si(y);
}
