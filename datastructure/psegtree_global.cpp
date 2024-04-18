//1st Unicup Stage15
//数列を持つ，hash で管理

#ifdef LOCAL
const int Lmax=10;
#else
const int Lmax=20;
#endif
const int nmax=(1<<Lmax)+10;

struct N{
	mint h;
	int a,b;
} w[nmax*(Lmax+5)];

mint powbuf[nmax]; initして

//lv は根が 0 で hei まである
int hei,used;
void leaf(int i,mint k){
	w[i].h=k;
	w[i].a=w[i].b=-1;
}
int newleaf(mint k){
	leaf(used,k);
	return used++;
}
void internal(int i,int lv,int a,int b){
	w[i].h=w[a].h+w[b].h*powbuf[1<<(hei-lv-1)];
	w[i].a=a;
	w[i].b=b;
}
int newinternal(int lv,int a,int b){
	internal(used,lv,a,b);
	return used++;
}
int init(int n){
	int s=minp2(n);
	hei=topbit(s);
	rng(i,s,s*2)leaf(i,0);
	gnr(i,1,s)internal(i,topbit(i),i*2,i*2+1);
	used=s*2;
	return 1;
}
int add(int x,int lv,int i){
	if(lv==hei)return newleaf(w[x].h+1);
	if(i&1<<(hei-lv-1))return newinternal(lv,w[x].a,add(w[x].b,lv+1,i));
	else return newinternal(lv,add(w[x].a,lv+1,i),w[x].b);
}
int add(int x,int i){return add(x,0,i);}
void dfs(int x,int lv,int val,vi&dst){
	if(lv==hei){
		int num=w[x].h.val();
		rep(_,num)dst.pb(val);
	}else{
		dfs(w[x].b,lv+1,val|1<<(hei-lv-1),dst);
		dfs(w[x].a,lv+1,val,dst);
	}
}
