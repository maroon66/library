//(0,h),(w,0)から伸びる直線で inclusive に囲まれる領域内を進み，x,y に至る方法)
//(x,y) が領域からはみ出ていた場合はもちろん 0
mint catalan_super(int h,int w,int x,int y){
	if(inc(-w,y-x,h)){
		int s=x+y,m=h+w+2;
		mint res=0;
		for(int z=y%m;z<=s;z+=m)res+=choose(s,z);
		for(int z=(x+(h+1))%m;z<=s;z+=m)res-=choose(s,z);
		return res;
	}else{
		return 0;
	}
}
