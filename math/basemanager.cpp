#ifdef LOCAL
const int S=10;
#else
const int S=230;
#endif
using B=bitset<S>;

//SRM790 Hard
//2 進数ベクトルを追加する
//その際 key も同時に追加
//circuit ができたら，その curcuit のなかで key 最小を削除して返してくれる
//O(S^2/64)
struct BaseManager{
	B val[S],idx[S];
	int num,ks[S],fs[S];
	BaseManager(){
		num=0;
	}
	int add(B cur,int key){
		B z;
		rep(i,num){
			if(cur[fs[i]]){
				cur^=val[i];
				z^=idx[i];
			}
		}
		int f=cur._Find_first();
		if(f<S){
			z[num]=1;
			ks[num]=key;
			val[num]=cur;
			idx[num]=z;
			fs[num]=f;
			num++;
			return -1;
		}else{
			int pos=-1,mn=key;
			rep(i,num)if(z[i]&&ks[i]<mn){
				pos=i;
				mn=ks[i];
			}
			if(pos==-1)return key;
			rep(i,num)if(idx[i][pos]){
				idx[i]^=z;
				idx[i][pos]=1;
			}
			ks[pos]=key;
			return mn;
		}
	}
};
