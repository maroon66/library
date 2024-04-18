vi invunit(int m,const vi&p){
	vi q(m,-1);
	rep(i,si(p))if(p[i]!=-1)q[p[i]]=i;
	return q;
}
//A: (N+1)*(K+1) subunit monge matrix. dif を取ると (i,a[i]) だけ 1
//B: (K+1)*(M+1) subunit monge matrix. dif を取ると (i,b[i]) だけ 1
//dif というのは
//-1 +1
//+1 -1
//左下の要素の総和を取る感じ
//min を計算している
vi subunit_monge_multiplication(int n,int m,int k,vi a,vi b){
	assert(si(a)==n);
	assert(si(b)==k);
	
	vi ai=invunit(k,a);swap(a,ai);
	vi bi=invunit(m,b);
	vi p(n,m),q(m,-1),tmp(k);
	vc<pi> buf;
	
	auto rec=[&](auto self,int l,int r)->pi{
		if(r-l==1){
			int as=a[l]!=-1,bs=b[l]!=-1;
			if(as&&bs){
				p[a[l]]=b[l];
				q[b[l]]=a[l];
			}
			return pi(as,bs);
		}else{
			int h=(l+r)/2;
			auto [als,bls]=self(self,l,h);
			auto [ahs,bhs]=self(self,h,r);
			int as=als+ahs,bs=bls+bhs;
			
			merge(a.bg+l,a.bg+l+als,a.bg+h,a.bg+h+ahs,tmp.bg);
			rep(i,as)a[l+i]=tmp[i];
			
			merge(b.bg+l,b.bg+l+bls,b.bg+h,b.bg+h+bhs,tmp.bg);
			rep(i,bs)b[l+i]=tmp[i];
			
			auto row=[&](int i){return i==as?n:a[l+i];};
			auto col=[&](int j){return j==bs?m:b[l+j];};
			
			buf.clear();
			int I=as,J=0,x=0,y=0,z=0;
			while(I>0&&J<bs){
				int i=row(I-1),i2=row(I);
				int j=col(J),j2=col(J+1);
				
				int u=y;
				if(bi[j]<h&&q[j]>=i)u++;
				if(h<=bi[j])u++;
				
				int v=z;
				if(h<=ai[i]&&p[i]<j2)v++;
				if(ai[i]<h)v++;
				
				if(min(u,v)+x-y-z>0)buf.eb(i,j);
				
				if(u<=v){//go right
					if(bi[j]<h&&inc(0,q[j],i-1))buf.eb(q[j],j);
					if(++J<bs){
						y=u;x=z;j=col(J);
						if(h<=bi[j]&&q[j]>=i2)z++;
					}
				}else{//go up
					if(h<=ai[i]&&inc(j2,p[i],m-1))buf.eb(i,p[i]);
					if(--I>0){
						z=v;x=y;i=row(I-1);
						if(ai[i]<h&&p[i]<j)y++;
					}
				}
			}
			
			rep(i,as)p[a[l+i]]=m;
			rep(i,bs)q[b[l+i]]=-1;
			for(auto [i,j]:buf){
				p[i]=j;
				q[j]=i;
			}
			return pi(as,bs);
		}
	};
	rec(rec,0,k);
	
	rep(i,n)if(p[i]==m)p[i]=-1;
	return p;
}

//LIS[l,r) = (r-l)-#{j:l<=j,0<=x[j]<r}
vi lis_info(vi p){
	int n=si(p);
	vi q(n);
	rep(i,n)q[p[i]]=i;
	vi tmp(n),x(n,-1);
	auto rec=[&](auto self,int l,int r)->void{
		int s=r-l;
		if(s>1){
			int h=(l+r)/2;
			self(self,l,h);
			self(self,h,r);
			
			merge(q.bg+l,q.bg+h,q.bg+h,q.bg+r,tmp.bg);
			rep(i,s)q[l+i]=tmp[i];
			rep(i,s)tmp[q[l+i]]=i;
			
			vi a(s);iota(all(a),0);
			vi b=a;
			rep(k,s){
				int i=q[l+k];
				if(p[i]<h){
					if(x[i]==-1)a[k]=-1;
					else a[k]=tmp[x[i]];
				}else{
					if(x[i]==-1)b[k]=-1;
					else b[k]=tmp[x[i]];
				}
			}
			vi y=subunit_monge_multiplication(s,s,s,a,b);
			rep(k,s){
				int i=q[l+k];
				if(y[k]==-1)x[i]=-1;
				else x[i]=q[l+y[k]];
			}
		}
	};
	rec(rec,0,n);
	return x;
}
