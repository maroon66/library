//CF850F
// sum u>=0 binom(b-u,c+u) の和を求める
// b,c がいい感じの順序に並んでいることが前提
// x,y の増減をちゃんと全部 verify できているかわかっていない
struct Z{
	mint cur=1;
	int x=0,y=0;
	mint calc(int b,int c){
		if(c<0){
			b+=c;
			c=0;
		}
		if(b<0)return 0;
		while(x<b){
			cur*=2;
			if(y>0){
				cur+=binom(x+1,y-1);
			}
			x++;
		}
		while(x>b){
			x--;
			if(y>0){
				cur-=binom(x+1,y-1);
			}
			cur*=invs[2];
		}
		while(y<c){
			cur*=2;
			cur-=binom(x,y);
			y++;
		}
		while(y>c){
			y--;
			cur+=binom(x,y);
			cur*=invs[2];
		}
		return cur;
	}
};
