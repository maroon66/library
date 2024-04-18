//min がもらえる
//0,1,...N-1 でクエリする前提
//verify は変な関数でやってたから線分はどうなるかわからん
//流石に大丈夫だと思うが
//あと何箇所か ull->int, L->inf といった変更をしたが，ちゃんと変更できてるかどうかは未知数
//Rollback できる
//Petrozavodsk 2020S Day3 H
struct LiChao{
	int s;
	vc<Func> f;
	vc<pair<int,Func>> buf;
	LiChao(int n){
		s=1;while(s<n)s*=2;
		f.resize(s*2,{0,inf});
	}
	int ask(int x){
		int i=x+s;
		int res=inf;
		while(i){
			chmin(res,f[i].eval(x));
			i>>=1;
		}
		return res;
	}
	int ss(){
		return si(buf);
	}
	void rollback(int z){
		while(z<si(buf)){
			auto w=buf.back();buf.pop_back();
			f[w.a]=w.b;
		}
	}
	void save(int i){
		buf.eb(i,f[i]);
	}
	void add(Func v){
		int i=1,l=0,r=s;
		while(i<2*s){
			int m=(l+r)/2;
			bool a=f[i].eval(l)>=v.eval(l);
			bool b=f[i].eval(r-1)>=v.eval(r-1);
			bool c=f[i].eval(m)>=v.eval(m);
			if(a){
				if(b){
					save(i);
					f[i]=v;
					return;
				}else{
					if(c){
						save(i);
						swap(f[i],v);
						i=i*2+1;
						l=m;
					}else{
						i=i*2;
						r=m;
					}
				}
			}else{
				if(b){
					if(c){
						save(i);
						swap(f[i],v);
						i=i*2;
						r=m;
					}else{
						i=i*2+1;
						l=m;
					}
				}else{
					return;
				}
			}
		}
	}
};
