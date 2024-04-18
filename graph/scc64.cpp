//CF609D
const int Nmax=2005;
using B=bitset<Nmax>;
B g[Nmax];
 
int grp[Nmax],head;
B ava,cur,low[Nmax];
vi s;
void dfs(int v){
	assert(ava[v]);
	ava[v]=false;
	B tmp=cur;
	cur[v]=1;
	s.pb(v);
	while(1){
		int c=(ava&g[v])._Find_first();
		if(c<Nmax){
			dfs(c);
			low[v]|=low[c];
		}else
			break;
	}
	if((tmp&low[v]).none()){
		while(1){
			int t=s.back();s.pop_back();
			grp[t]=head;
			cur[t]=0;
			if(t==v)break;
		}
		head++;
	}
}
void Calc(int n){
	one(grp);
	head=0;
	ava.set();
	cur.reset();
	rep(i,n)
		low[i]=g[i];
	s.clear();
	rep(i,n)if(ava[i])
		dfs(i);
}
