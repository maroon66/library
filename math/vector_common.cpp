const int L=64;
//2 つのベクトル空間の共通部分を返す
//addA,addB に基底を渡していく(基底じゃないと壊れます)
//共通部分が広がるたびに新しい基底が返ってくる
//Universal Cup 2023 Stage 1 M
//https://en.wikipedia.org/wiki/Zassenhaus_algorithm
int cur=0;
ull buf[L],key[L];
void clear(){cur=0;}
ull add(ull a,ull b){
	rep(i,cur)if(chmin(a,a^buf[i]))b^=key[i];
	if(a==0)return b;
	else{
		buf[cur]=a;
		key[cur++]=b;
		return 0;
	}
}
ull addA(ull a){return add(a,0);}
ull addB(ull a){return add(a,a);}
