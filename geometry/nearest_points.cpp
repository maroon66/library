//UCUP 2023-2 I (TLE)

const int nmax=250010;
pi xy[nmax];
int nx[nmax];
ll nl[nmax];

ll val(int i,int j){
	return sq<ll>(xy[i].a-xy[j].a)+sq<ll>(xy[i].b-xy[j].b);
}

hash_table<pi,int> table;
//unordered_map<pi,int> table;
ll curmin,D;
vi has;
void clear(){
	curmin=infLL;
	D=sqrtl(curmin);
	table.clear();
	has.clear();
}

int qnum=0;
ll query(int q){
	qnum++;
	ll res=curmin;
	if(curmin>0&&nl[q]<curmin){
		int xmid=xy[q].a/D,ymid=xy[q].b/D;
		rng(i,xmid-1,xmid+2)rng(j,ymid-1,ymid+2){
			auto itr=table.find(pi(i,j));
			if(itr!=table.ed){
				for(int x=itr->b;x!=-1;x=nx[x]){
					chmin(res,val(q,x));
				}
			}
		}
	}
	//dmp2(q,res);
	return res;
}

int itrnum=0;

void add(int q){
	has.pb(q);
	if(chmin(curmin,query(q))&&curmin>0){
		D=sqrtl(curmin);
		itrnum+=si(has);
		table.clear();
		for(auto x:has){
			int i=xy[x].a/D,j=xy[x].b/D;
			auto itr=table.find(pi(i,j));
			if(itr==table.ed){
				nx[x]=-1;
				table[pi(i,j)]=x;
			}else{
				nx[x]=itr->b;
				itr->b=x;
			}
		}
	}else if(curmin>0){
		int i=xy[q].a/D,j=xy[q].b/D;
		auto itr=table.find(pi(i,j));
		if(itr==table.ed){
			nx[q]=-1;
			table[pi(i,j)]=q;
		}else{
			nx[q]=itr->b;
			itr->b=q;
		}
	}
}
