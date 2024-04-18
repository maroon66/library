//lw <= hx mod w <=up
//return hx mod w 
int waf(int h,int w,int lw,int up){
	assert(0<=lw);
	assert(lw<=up);
	assert(up<=w);
	if(lw==0)return 0;
	if(h==0)return -1;
	int need=(lw+h-1)/h;
	int p=need*h;
	if(inc(lw,p,up)){
		return p;
	}
	int res=waf(w%h,h,p-up,p-lw);
	if(res==-1)return -1;
	return p-res;
}
