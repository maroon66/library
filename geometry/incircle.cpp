//UCUP 3-29-M
bool incircle(pt a,pt b,pt c,pt p){
	//if(ccw(a,b,c)<0)swap(b,c);
	assert(ccw(a,b,c)>0);
	__int128 a2=norm(a-=p),b2=norm(b-=p),c2=norm(c-=p);
	return a2*crs(b,c)+b2*crs(c,a)+c2*crs(a,b)>=0;
}//座標の 4 乗オーダーの値が出てくると話題に(sgn 禁止)
bool oncircle(pt a,pt b,pt c,pt p){
	//if(ccw(a,b,c)<0)swap(b,c);
	assert(ccw(a,b,c)>0);
	__int128 a2=norm(a-=p),b2=norm(b-=p),c2=norm(c-=p);
	return a2*crs(b,c)+b2*crs(c,a)+c2*crs(a,b)==0;
}//座標の 4 乗オーダーの値が出てくると話題に(sgn 禁止)
