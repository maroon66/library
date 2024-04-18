//UTPC 2020 I
struct TreeCutManager{
	int n,m;
	vi head,to,nx,pr,par,work[2];
	TreeCutManager(int nn):n(nn),m(0),head(n,-1),par(n){}
	void ae(int a,int b){
		rep(_,2){
			to.pb(b);
			nx.pb(head[a]);
			pr.pb(a-n);
			if(head[a]!=-1)pr[head[a]]=si(to)-1;
			head[a]=si(to)-1;
			swap(a,b);
		}
		m++;
	}
	void del(int idx){
		rep(k,2){
			int a=pr[idx*2+k],b=nx[idx*2+k];
			if(a>=0)nx[a]=b;
			else head[a+n]=b;
			if(b!=-1)pr[b]=a;
		}
	}
	vi cut_get_smaller(int idx){
		del(idx);
		auto reach=[&](int k,int v,int p){
			work[k].pb(v);
			par[v]=p;
		};
		pi cur[2];
		rep(k,2){
			work[k].clear();
			reach(k,to[idx*2+k],-1);
			cur[k]=pi(-1,-1);
		}
		while(1){
			int upd=0;
			rep(k,2){
				while(1){
					if(cur[k].b==-1){
						cur[k].a++;
						if(cur[k].a==si(work[k]))break;
						cur[k].b=head[work[k][cur[k].a]];
					}
					if(cur[k].b==-1)continue;
					int v=work[k][cur[k].a];
					int w=to[cur[k].b];
					if(par[v]==w){
						cur[k].b=nx[cur[k].b];
						continue;
					}
					reach(k,w,v);
					cur[k].b=nx[cur[k].b];
					upd++;
					break;
				}
			}
			if(upd<2)break;
		}
		if(si(work[0])<si(work[1]))return work[0];
		else return work[1];
	}
};
