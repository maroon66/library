//Petrozavodsk 2019w Day9 G
//CF Pinely3-G
vi kmp(const string&s){
	int n=s.size();
	vi v(n+1);
	v[0]=-1;
	rng(i,1,n+1){
		int j=v[i-1];
		while(j>=0&&s[j]!=s[i-1])
			j=v[j];
		v[i]=j+1;
	}
	return v;
}

//2020 Multi-Uni Contest Day8 H
//does a contains b?
bool contain(const string&a,const string&b){
	vi x=kmp(b);
	
	int cur=0;
	for(auto c:a){
		while(cur>=0&&b[cur]!=c)cur=x[cur];
		cur++;
		if(cur==si(b)) return true;
	}
	return false;
}

vi periods(const string&s){
	int n=s.size();
	vi x=kmp(s),v;
	for(int i=x[n];i!=-1;i=x[i])
		v.pb(n-i);
	return v;
}

template<class S>
int period(const S&s){
	int n=s.size();
	static vi v;
	v.resize(n+1);
	rep(i,n+1)v[i]=0;
	v[0]=-1;
	rng(i,1,n+1){
		int j=v[i-1];
		while(j>=0&&s[j]!=s[i-1])
			j=v[j];
		v[i]=j+1;
	}
	return n-v[n];
}

//真の "K"MP
//Xmas2015D
//serial link を使う
//serial link というのは，diff のちがうところまで伸ばしたリンク
//diff というのは，len-len(border) のことかな
//このコードだと，x が mp で，y が kmp
