const int nmax=2010;
const int smax=4010;

struct E{
	int to,cost;
};

E g[smax][smax];
int deg[smax];

void initg(int n){
	rep(i,n)deg[i]=0;
}

void ae(int s,int t,int c){
	g[s][deg[s]++]=E{t,c};
}

int dist[smax];
int qbuf[ten(5)];
bool inq[smax];
void sp(int n,int s){
	rep(i,n){
		dist[i]=inf;
		inq[i]=false;
	}
	int head=0,tail=0;
	auto push=[&](int v,int d){
		if(chmin(dist[v],d)){
			if(!inq[v]){
				qbuf[tail++]=v;
				inq[v]=true;
			}
		}
	};
	push(s,0);
	while(head<tail){
		int v=qbuf[head++];
		inq[v]=false;
		rep(i,deg[v])push(g[v][i].to,dist[v]+g[v][i].cost);
	}
}



struct E{
	int to,cost;
};

const int S=ten(7); 
int qbuf[S];
vi sp(const vvc<E>&g,int s){
	int n=si(g);
	vi dist(n,inf);
	vc<bool> inq(n,false);
	int head=0,tail=0,cnt=0;
	auto push=[&](int v,int d){
		if(chmin(dist[v],d)){
			if(!inq[v]){
				cnt++;
				qbuf[tail++]=v;
				inq[v]=true;
				if(tail==S){
					rng(i,head,tail)qbuf[i-head]=qbuf[i];
					tail-=head;
					head=0;
				}
			}
		}
	};
	push(s,0);
	while(head<tail){
		int v=qbuf[head++];
		inq[v]=false;
		for(auto e:g[v])push(e.to,dist[v]+e.cost);
	}
	return dist;
}
