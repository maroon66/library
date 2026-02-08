struct Rect{
	int xl,xr,yl,yr;
	bool valid()const{
		return xl<=xr&&yl<=yr;
	}
	bool empty()const{
		return xl==xr||yl==yr;
	}
	int wid()const{
		return xr-xl;
	}
	int hei()const{
		return yr-yl;
	}
	//FHC 2021 Final F
	Rect translated(int x,int y){
		return {xl+x,xr+x,yl+y,yr+y};
	}
	//FHC 2021 Final F
	Rect shifted(int dir){
		static const int dxy[]{-1,0,1,0,-1};
		//LURectD for 2d-axis
		int x=wid()*dxy[dir],y=hei()*dxy[dir+1];
		return translated(x,y);
	}
	//UCUP4-9-I
	void rot90(){
		tie(xl,xr,yl,yr)=mt(-yr,-yl,xl,xr);
	}
};
ostream& operator<<(ostream&os,Rect v){
	return os<<"["<<v.xl<<","<<v.xr<<")*["<<v.yl<<","<<v.yr<<")";
}
bool intersect(Rect a,Rect b){
	return max(a.xl,b.xl)<min(a.xr,b.xr)
		&& max(a.yl,b.yl)<min(a.yr,b.yr);
}
Rect bounding(Rect a,Rect b){
	return Rect{
		min(a.xl,b.xl),
		max(a.xr,b.xr),
		min(a.yl,b.yl),
		max(a.yr,b.yr),
	};
}
