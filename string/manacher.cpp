//Petrozavodsk 2019w Day9 G
//Petrozavodsk 2020S Day4 G
//aba->121
template<class S>
vi manacher(const S&s){
	int n=s.size();
	vi v(n);
	int j=0;
	rep(i,n){
		int k=j*2-i;
		if(k>=0&&i+v[k]<j+v[j])
			v[i]=v[k];
		else{
			v[i]=j+v[j]-i;
			j=i;
			while(i+v[i]<n&&i-v[i]>=0&&s[i-v[i]]==s[i+v[i]])
				v[i]++;
		}
	}
	return v;
}

template<class S>
vi manacher_even(const S&s,int none){
	S t;
	t.reserve(si(s)*2+1);
	t.pb(none);
	for(auto c:s){
		t.pb(c);
		t.pb(none);
	}
	vi a=manacher(t),b;
	rep(i,si(s)+1)
		b.pb(a[i*2]/2);
	return b;
}

//XXII Opencup GP of Yuquan
template<class S>
vi manacher_oddeven(const S&s,int none){
	S t;
	t.reserve(si(s)*2-1);
	rep(i,si(s)){
		t.pb(s[i]);
		if(i+1<si(s))t.pb(none);
	}
	return manacher(t);
}

//XXII Opencup GP of Yuquan
bool ispal(const vi&t,int l,int r){
	int mid=(l+r-1);
	return t[mid]>=r-l;
}

//UCUP 2-27-G
//aabaa -> [0,1,2,1,0,5,0,1,2,1,0]
template<class S>
vi diameter(const S&s,int none){
	int n=si(s);
	S t;
	t.reserve(n*2+1);
	t.pb(none);
	rep(i,n){
		t.pb(s[i]);
		t.pb(none);
	}
	return manacher(t)-1;
}
