//Multiuni2023-8 C
//f(lw)=false,...,f(n-1)=false,f(n)=true,...,f(up)=true,
//のときに n を返す
template<class F>
int find_min_true(int lw,int up,F f){
	while(up-lw>1){
		const int mid=(lw+up)/2;
		if(f(mid))up=mid;
		else lw=mid;
	}
	return up;
}
//f(lw)=true,f(up)=false
template<class F>
int find_max_true(int lw,int up,F f){
	while(up-lw>1){
		const int mid=(lw+up)/2;
		if(f(mid))lw=mid;
		else up=mid;
	}
	return lw;
}

//NERC 2023-2 I
template<int L,class F>
ld find_max_true_ld(ld lw,ld up,F f){
	rep(_,L){
		const ld mid=(lw+up)/2;
		if(f(mid))lw=mid;
		else up=mid;
	}
	return lw;
}
//ABC314H
template<int L,class F>
ld find_min_true(ld lw,ld up,F f){
	rep(_,L){
		const ld mid=(lw+up)/2;
		if(f(mid))up=mid;
		else lw=mid;
	}
	return up;
}
//true,false が全部逆
template<int L,class F>
ld find_min_false(ld lw,ld up,F f){
	rep(_,L){
		const ld mid=(lw+up)/2;
		if(!f(mid))up=mid;
		else lw=mid;
	}
	return up;
}

//誤差をもとにした2分探索
//CF930F
template<class F>
ld find_max_true_ld(ld lw,ld up,ld allow,F f){
	assert(lw<=up);
	while(up-lw>allow){
		const ld mid=(lw+up)/2;
		if(f(mid))lw=mid;
		else up=mid;
	}
	return lw;
}

//verify してません
//f(0)=false,...,f(n-1)=false,f(n)=true,true,true,
//のときに n を返す
template<class F>
int find_min_true(F f){
	for(int x=0,y=-1;;y=x,x=x*2+1){
		if(f(x)){
			while(x-y>1){
				int z=(x+y)/2;
				if(z)x=z;
				else y=z;
			}
			return x;
		}
	}
}


