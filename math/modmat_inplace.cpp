template<class t>
void mat0(vvc<t>&a,int n,int m){
	static vvc<t> pool;
	while(si(a)<n){
		if(pool.empty())a.resize(n);
		else a.pb(gpp(pool));
	}
	while(si(a)>n)pool.pb(gpp(a));
	rep(i,n){
		a[i].resize(m);
		fill(all(a[i]),0);
	}
}

//VERIFY: Yukicoder No.1773
int matrank(const vvc<mint>&rw){
	if(rw.empty())return 0;
	int h=rw.size(),w=rw[0].size(),r=0;
	static vvc<mint> a;
	mat0(a,h,w);
	rep(i,h)rep(j,w)a[i][j]=rw[i][j];
	rep(i,w){
		if(r==h)break;
		rng(j,r,h)if(a[j][i].v){
			swap(a[r],a[j]);
			break;
		}
		if(a[r][i].v==0)continue;
		rng(j,r+1,h){
			mint z=-a[j][i]/a[r][i];
			rng(k,i,w)
				a[j][k]+=a[r][k]*z;
		}
		r++;
	}
	return r;
}

//VERIFY: yosupo
mint det(const vvc<mint>&rw){
	const int n=rw.size();
	static vvc<mint> a;
	mat0(a,n,n);
	rep(i,n)rep(j,n)a[i][j]=rw[i][j];
	mint ans(1);
	rep(i,n){
		rng(j,i+1,n)if(a[j][i]){
			ans=-ans;
			swap(a[j],a[i]);
			break;
		}
		if(a[i][i]==0)return 0;
		ans*=a[i][i];
		mint z=a[i][i].inv();
		rng(j,i+1,n){
			mint w=-a[j][i]*z;
			rng(k,i,n)
				a[j][k]+=a[i][k]*w;
		}
	}
	return ans;
}

//inplace
//左c列をsweepしする
//aの先頭rank行に要素が入っている感じ
//Yuhao Du Contest 11 (Ucup1-10 G)
//https://uoj.ac/problem/785
int sweep(vvc<mint>&a,vi*col=nullptr,int c=-1){
	if(a.empty())return 0;
	if(c==-1)c=a[0].size();
	int h=a.size(),w=a[0].size(),r=0;
	rep(i,c){
		if(r==h)break;
		rng(j,r,h)if(a[j][i]){
			swap(a[r],a[j]);
			break;
		}
		if(a[r][i]==0)continue;
		mint u=a[r][i].inv();
		rep(j,h)if(j!=r){
		//rng(j,r+1,h){ //for speed up (sweep に使った列に複数の non-zero があっても良い場合)
			mint z=-a[j][i]*u;
			rng(k,i,w)
				a[j][k]+=a[r][k]*z;
		}
		if(col)col->pb(i);
		r++;
	}
	return r;
}

//https://uoj.ac/problem/785
bool inverse(vvc<mint>&a){
	int n=si(a);
	rep(i,n){
		assert(si(a[i])==n);
		a[i].resize(2*n);
	}
	static vi row;
	row.resize(n);rep(i,n)row[i]=i;
	rep(i,n){
		rng(j,i,n)if(a[j][i]){
			swap(a[i],a[j]);
			swap(row[i],row[j]);
			break;
		}
		if(a[i][i]==0)return false;
		mint u=a[i][i].inv();
		a[i][n+i]=1;
		rng(k,i,n+i+1)a[i][k]*=u;
		rep(j,n)if(j!=i){
			mint z=-a[j][i];
			rng(k,i,n+i+1)a[j][k]+=a[i][k]*z;
		}
	}
	rep(i,n){
		rep(j,n)a[i][row[j]]=a[i][n+j];
		a[i].resize(n);
	}
	return true;
}

//https://uoj.ac/problem/785
//mod を減らして定数倍を，改善…
void mmm(vvc<mint>&c,const vvc<mint>&a,const vvc<mint>&b){
	int n=si(a),m=si(a[0]),l=si(b[0]);
	assert(m==si(b));
	mat0(c,n,l);
	static vvc<ull> tmp;mat0(tmp,n,l);
	static const int L=15;
	rep(i,n)rep(j,m)rep(k,l)tmp[i][k]+=ull(a[i][j].v>>L)*(b[j][k].v);
	rep(i,n)rep(k,l)tmp[i][k]=(tmp[i][k]%mint::mod)<<L;
	rep(i,n)rep(j,m)rep(k,l)tmp[i][k]+=ull(a[i][j].v&mask(L))*(b[j][k].v);
	rep(i,n)rep(k,l)c[i][k]=tmp[i][k];
}

//メモリを全部vectorからarrayに変えたけど速くならない…
//modintの計算がボトルネックだとあんま変わらないのかな
//inverseはullに変えても速くない，なぜ？

//Yukicoder No.1774
struct Harvey{
	const int n;
	vvc<mint> a,inv;
	Harvey(const vvc<mint>&ini):n(si(ini)),a(ini),inv(ini){
		bool tmp=inverse(inv);
		assert(tmp);
	}
	using T=tuple<int,int,mint>;
	vc<T> buf;
	void add(int i,int j,mint v){
		buf.eb(i,j,v);
	}
	vi pos;
	vvc<mint> dif,invss,invst,invts,w,z;
	//add の結果を使って inv を更新しようとする
	//正則なら更新に成功し，true が返る
	//非正則なら a/inv はもとのまま false が返る
	bool upd(){
		pos.clear();
		for(auto [i,j,v]:buf){
			pos.pb(i);
			pos.pb(j);
		}
		mkuni(pos);
		int s=si(pos);
		
		mat0(dif,s,s);
		for(auto [i,j,v]:buf)
			dif[lwb(pos,i)][lwb(pos,j)]+=v;
		mat0(invss,s,s);
		rep(i,s)rep(j,s)invss[i][j]=inv[pos[i]][pos[j]];
		
		mmm(w,dif,invss);
		rep(i,s)w[i][i]+=1;
		if(!inverse(w)){
			buf.clear();
			return false;
		}
		
		mat0(invst,s,n);
		rep(i,s)rep(j,n)invst[i][j]=inv[pos[i]][j];
		mat0(invts,n,s);
		rep(i,n)rep(j,s)invts[i][j]=inv[i][pos[j]];
		
		mmm(z,w,dif);
		mmm(w,invts,z);
		mmm(z,w,invst);
		
		rep(i,n)rep(j,n)
			inv[i][j]-=z[i][j];
		
		for(auto [i,j,v]:buf)
			a[i][j]+=v;
		
		buf.clear();
		return true;
	}
};
