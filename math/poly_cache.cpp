//FFT の結果をキャッシュして持っておく
//rw に生データ，buf に fft 後のデータ
//どちらかが空でもよい
//si(buf) は 0 or si(rw) 以上の 2 冪
//Codechef 2021 January Lunchtime EXPGROUP
//yosupo product of polynomial sequence
//Yukicoder 2166
//メモリ食いまくり
//ABC278H
//Osijek Day6 A
//Yukicoder 2575 (a<b<c の畳込みがほしいときはここへ)
struct F{
	int n;
	vc<mint> rw,buf;
	F():n(0){}
	F(const vc<mint>&given):rw(given){
		n=si(rw);
		assert(n>0);
	}
	F(initializer_list<mint> init):rw(all(init)){
		n=si(rw);
		assert(n>0);
	}
	int size()const{return n;}
	bool empty()const{return n==0;}
	void assume_have(){
		if(rw.empty()){
			int s=minp2(n);
			assert(si(buf)>=s);
			rw.resize(s);
			rep(i,s)rw[i]=buf[i].v;
			inplace_fmt(rw,true);
			rw.resize(n);
		}
		assert(si(rw)==n);
	}
	vc<mint> getrw(){
		if(n==0)return {};
		assume_have();
		return rw;
	}
	void prepare(int len){
		if(si(buf)<len)assume_have();
		if(buf.empty()){
			int s=minp2(n);
			buf.resize(s);
			rep(i,n)buf[i]=rw[i];
			inplace_fmt(buf,false);
		}
		while(si(buf)<len){
			int s=si(buf);
			buf.resize(s*2);
			rep(i,n)buf[s+i]=rw[i];
			half_fmt(s,buf.data()+s);
		}
	}
	void copy_from(F&a){
		n=a.n;
		rw=a.rw;
		if(si(a.buf)){
			int s=minp2(n);
			buf.resize(s);
			rep(i,s)buf[i]=a.buf[i];
		}else buf.clear();
	}
	void init_from_sum(F&a,F&b){
		if(a.empty())return copy_from(b);
		if(b.empty())return copy_from(a);
		n=max(a.n,b.n);
		if(si(a.rw)&&si(b.rw)){
			rw.resize(n);
			rep(i,n){
				rw[i]=0;
				if(i<a.n)rw[i]+=a.rw[i];
				if(i<b.n)rw[i]+=b.rw[i];
			}
		}else rw.clear();
		int s=minp2(n);
		if(si(a.buf)>=s&&si(b.buf)>=s){
			buf.resize(s);
			rep(i,s)buf[i]=mint(a.buf[i].v)+mint(b.buf[i].v);
		}else buf.clear();
		if(rw.empty()&&buf.empty()){
			/*
			a.prepare(n);
			b.prepare(n);
			buf.resize(s);
			rep(i,s)buf[i]=mint(a.buf[i].v)+mint(b.buf[i].v);
			*/
			//こっちのほうが微小にメモリが少なくなる?
			a.assume_have();
			b.assume_have();
			rw.assign(n,0);
			rep(i,a.n)rw[i]+=a.rw[i];
			rep(i,b.n)rw[i]+=b.rw[i];
		}
	}
	//Osijek Day6 A (n=0,middle=true)
	void init_from_product(F&a,F&b,bool middle=false){
		if(a.n==0||b.n==0){
			n=0;
			rw.clear();
			buf.clear();
			return;
		}
		assert(a.n>0);
		assert(b.n>0);
		if(middle){
			assert(a.n>=b.n);
			n=a.n;
		}else{
			n=a.n+b.n-1;
		}
		rw.clear();
		int s=minp2(n);
		a.prepare(n);
		b.prepare(n);
		buf.resize(s);
		rep(i,s)buf[i]=a.buf[i]*b.buf[i];
	}
	F operator*(F&b){
		F res;
		res.init_from_product(*this,b);
		return res;
	}
	F operator*(F&&b){
		F res;
		res.init_from_product(*this,b);
		return res;
	}
	F operator+(F&b){
		F res;
		res.init_from_sum(*this,b);
		return res;
	}
	F operator+(F&&b){
		F res;
		res.init_from_sum(*this,b);
		return res;
	}
	F& operator*=(F&b){
		return *this=(*this)*b;
	}
	F& operator+=(F&b){
		return *this=(*this)+b;
	}
	F& operator+=(F&&b){
		return *this=(*this)+b;
	}
	void freememory(){
		n=0;
		vc<mint>().swap(rw);
		vc<mint>().swap(buf);
	}
	//middle product の情報をつくる
	//a,b,c が x の関数だとする
	//ans+=[x^deg(a)] abc だとしよう
	//ab の係数上位 deg(c)+1 項を d とおけば
	//ans+=[x^deg(d)] dc とできるので，この d を求めよう
	//単にサイズを広げない prod をするだけ
	//deg(a)>=deg(b)+deg(c) でないと破綻する
	F middle(F&b){
		F res;
		res.init_from_product(*this,b,true);
		return res;
	}
	//middle product の情報が buf に入っているとする
	//サイズ s に縮める
	//[n-s,n) が重要で，それ以外のパートにはゴミが入っているかもしれない
	//Osijek Day6 A
	void middle_shrink(int s){
		if(empty())return;
		assert(s<=n);
		if(s<n){
			assume_have();
			rep(i,s)rw[i]=rw[n-s+i];
			rw.resize(n=s);
			buf.clear();
		}
	}
	//転置原理+分割統治と組み合わせるといい感じ
	//ありがちなパターン
	//for i=0,1,...: ans[i]=[x^(n-1)]v; v=m_i*v
	//m_i が（高々）一次なら，dfs(l,r) では v[k].middle_shrink(r-l)
	//m_i の要素の次数はバラバラでも大丈夫
	//空の prod で空が返るようにしてあるので m_i に空を入れてもいい
	//行列とかで戻していくとき，middle 側は空or全部同じ次数という形にしておく
};
//sum a[i]/(1-b[i]x)
//ABC278H
//ARC163F
vc<mint> suminv(vc<mint> a,vc<mint> b,int s){
	int n=si(a);assert(si(b)==n);
	if(n==0){return vc<mint>(s);}
	auto dfs=[&](auto self,int l,int r)->pair<F,F>{
		if(r-l==1){
			return mp(F{a[l]},F{1,-b[l]});
		}else{
			int m=(l+r)/2;
			auto [x,y]=self(self,l,m);
			auto [z,w]=self(self,m,r);
			return mp(x*w+z*y,y*w);
		}
	};
	auto [num,den]=dfs(dfs,0,n);
	vc<mint> res=(num*Poly<mint>(den.getrw()).inv(s)).getrw();
	res.resize(s);
	return res;
}

vc<mint> all_product(const vvc<mint>&ls){
	if(ls.empty())return {1};
	vc<F> fs(all(ls));
	const auto cmp=[&](const F&a,const F&b)->bool{
		return a.n>b.n;
	};
	make_heap(all(fs),cmp);
	int n=si(fs);
	while(n>=2){
		pop_heap(fs.bg,fs.bg+n,cmp);
		pop_heap(fs.bg,fs.bg+n-1,cmp);
		fs[n-2]*=fs[n-1];
		push_heap(fs.bg,fs.bg+n-1,cmp);
		fs.pop_back();
		n--;
	}
	return fs[0].getrw();
}
//a の subset sum の数え上げ
//TRCO31G
vc<mint> subset_sum(vi a){
	auto f=to_freq(a);
	vvc<mint> buf;
	for(auto [s,n]:f){
		vc<mint> x(s*n+1);
		rep(i,n+1)x[i*s]+=choose(n,i);
		buf.pb(move(x));
	}
	return all_product(buf);
}
