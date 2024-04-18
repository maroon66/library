//f(x) が ] -1 | 0 | 1 [ みたいな形をしているとする
//f(0)<=0 を仮定
//f(inf)=1 を仮定
//f(x)=0 なる x を見つけてくる．
//分母最小，その中で分子最小を見つける
//f(x)=0 なる call の中で最後のやつが答えになっている
//stress-tested
template<class F>
pi frac_bisect(F func){
	pi lw(0,1),up(1,0);
	int f;
	auto load=[&](pi v){
		f=func(v);
		assert(inc(-1,f,1));
	};
	load(lw);
	assert(f<=0);
	if(f==0)return lw;
	auto mid=[&](int a,int b){
		return pi(lw.a*a+up.a*b,lw.b*a+up.b*b);
	};
	load(mid(1,1));
	while(1){
		//func(lw)==-1
		//func(up)==1
		if(f==0)return mid(1,1);
		else if(f==-1){
			int w=1;
			while(1){
				w*=2;
				load(mid(1,w));
				if(f>-1)break;
			}
			int l=w/2,r=w,rf=f;
			while(r-l>1){
				const int v=(l+r)/2;
				load(mid(1,v));
				if(f==-1)l=v;
				else{
					r=v;
					rf=f;
				}
			}
			lw=mid(1,l);
			f=rf;
		}else if(f==1){
			int w=1;
			while(1){
				w*=2;
				load(mid(w,1));
				if(f<1)break;
			}
			int l=w/2,r=w,rf=f;
			while(r-l>1){
				const int v=(l+r)/2;
				load(mid(v,1));
				if(f==1)l=v;
				else{
					r=v;
					rf=f;
				}
			}
			up=mid(l,1);
			f=rf;
		}else assert(0);
	}
}

//有理数二分探索を並列化するためのライブラリ
//stress-tested
//EC Final 2021 K
struct Frac_Bisector{
	pi lw=pi(0,1),up=pi(1,0),ans;
	pi mid(int a,int b){
		return pi(lw.a*a+up.a*b,lw.b*a+up.b*b);
	}
	int phase=0,w,l,r,rf;
	//解がすでに求まっているかどうか返す
	//true: v に答えを入れる
	//false: v に次にクエリすべき有理数を入れる
	//phase=-1; すでに解は決定
	//phase 0;
	bool determined(pi&v){
		if(phase==-1){
			v=ans;
			return true;
		}else if(phase==0){
			//check if (0,1) is ok
			v=lw;
			return false;
		}else if(phase==1){
			//check mid(lw,up);
			v=mid(1,1);
			return false;
		}else if(phase==2){
			//f(mid(1,w))==-1
			//find w s.t. f(mid(1,w))>-1
			w*=2;
			v=mid(1,w);
			return false;
		}else if(phase==3){
			//f(mid(w,1))==1
			//find w s.t. f(mid(w,1))<1
			w*=2;
			v=mid(w,1);
			return false;
		}else if(phase==4){
			//f(mid(1,l))==-1
			//f(mid(1,r))>-1
			//find max t s.t f(mif(1,t))==-1
			//r の結果を保存して微小に高速化している
			if(r-l==1){
				if(rf==0){
					v=mid(1,r);
					return true;
				}else if(rf==1){
					lw=mid(1,l);
					w=2;
					v=mid(w,1);
					phase=3;
					return false;
				}else assert(false);
			}else{
				v=mid(1,(l+r)/2);
				return false;
			}
		}else if(phase==5){
			//f(mid(l,1))==1
			//f(mid(r,1))<1
			//find max t s.t f(mif(t,1))==1
			//r の結果を保存して微小に高速化している
			if(r-l==1){
				if(rf==0){
					v=mid(r,1);
					return true;
				}else if(rf==-1){
					up=mid(l,1);
					w=2;
					v=mid(1,w);
					phase=2;
					return false;
				}else assert(false);
			}else{
				v=mid((l+r)/2,1);
				return false;
			}
		}else{
			assert(false);
		}
	}
	void tell(int f){
		assert(phase!=-1);
		assert(inc(-1,f,1));
		if(phase==0){
			if(f==-1){
				phase=1;
			}else if(f==0){
				phase=-1;
				ans=lw;
			}else assert(false);
		}else if(phase==1){
			if(f==-1){
				phase=2;
				w=1;
			}else if(f==0){
				phase=-1;
				ans=mid(1,1);
			}else if(f==1){
				phase=3;
				w=1;
			}
		}else if(phase==2){
			if(f>-1){
				l=w/2;
				r=w;
				rf=f;
				phase=4;
			}
		}else if(phase==3){
			if(f<1){
				l=w/2;
				r=w;
				rf=f;
				phase=5;
			}
		}else if(phase==4){
			if(f==-1)l=(l+r)/2;
			else{
				r=(l+r)/2;
				rf=f;
			}
		}else if(phase==5){
			if(f==1)l=(l+r)/2;
			else{
				r=(l+r)/2;
				rf=f;
			}
		}else assert(false);
	}
};
