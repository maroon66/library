ll cdiv(ll a, ll b) { // ceiled division
	return a / b + ((a ^ b) > 0 && a % b); }

int fcmp(int a,int b,int c,int d){
	int z=a*d-b*c;
	return z<0?-1:z==0?0:1;
}

//Farey 数列
//(la/lb,ra/rb) の形でデータをもつ
//default だと 1/2 に対応
//(0,1),(1,1) などがクエリで来ると困りがちともっぱらの噂
struct Farey{
	int la,lb,ra,rb;
	Farey():la(0),lb(1),ra(1),rb(1){}
	pi get_current(){
		return pi(la+ra,lb+rb);
	}
	pi get_right(int step){
		assert(step>=-1);
		return pi(la+ra*(step+1),lb+rb*(step+1));
	}
	pi get_left(int step){
		assert(step>=-1);
		return pi(la*(step+1)+ra,lb*(step+1)+rb);
	}
	void move_right(int step){
		assert(step>=0);
		tie(la,lb)=get_right(step-1);
	}
	void move_left(int step){
		assert(step>=0);
		tie(ra,rb)=get_left(step-1);
	}
	//右に進んでいって,
	//abok=true: a/b 以上になったら終了
	//false: a/b より真に大きくなったら終了
	//何回進むか返す
	int need_right(int a,int b,bool abok){
		int w=ra*b-rb*a;
		if(w<=0)return inf;
		return cdiv(max((lb+rb)*a-(la+ra)*b+(abok?0:1),int(0)),w);
	}
	//左に進んでいって,
	//abok=true: a/b 以下になったら終了
	//false: a/b より真に小さくなったら終了
	//何回進むか返す
	int need_left(int a,int b,bool abok){
		int w=a*lb-la*b;
		if(w<=0)return inf;
		return cdiv(max((la+ra)*b-(lb+rb)*a+(abok?0:1),int(0)),w);
	}
	//la/lb<=xa/xb && ya/yb<=ra/rb を仮定
	//(xa/xb,ya/yb) 内の LCA へ移動する
	void move_lca(int xa,int xb,int ya,int yb){
		assert(fcmp(la,lb,xa,xb)<=0);
		assert(fcmp(xa,xb,ya,yb)<0);
		assert(fcmp(ya,yb,ra,rb)<=0);
		while(1){
			bool upd=false;
			{
				int t=need_right(xa,xb,false);
				assert(t<inf);
				upd|=t;
				move_right(t);
			}
			{
				int t=need_left(ya,yb,false);
				assert(t<inf);
				upd|=t;
				move_left(t);
			}
			if(!upd)break;
		}
	}
};
