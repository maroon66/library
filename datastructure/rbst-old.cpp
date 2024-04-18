inline int xorshift(){
	static int w=1234567890;
	w=w^(w<<17);
	w=w^(w>>13);
	w=w^(w<<5);
	return w;
}
struct Node{
	using np=Node*;
	np l,r;
	int v,n,s,num,wei;
	void upd(){
		s=1;
		num=n;
		wei=n*v;
		for(np c:{l,r}){
			if(c){
				s+=c->s;
				num+=c->num;
				wei+=c->wei;
			}
		}
	}
} buf[100010];
using np=Node*;
//Not Verified
int bufUsed;
np newnode(int v,int n){
	np ptr=buf+(bufUsed++);
	ptr->l=0;
	ptr->r=0;
	ptr->v=v;
	ptr->n=n;
	ptr->s=1;
	ptr->num=n;
	ptr->wei=n*v;
	return ptr;
}
np merge(np a,np b){
	if(!a)return b;
	if(!b)return a;
	int s=a->s+b->s,x=xorshift()%s;
	if(x<a->s){
		a->r=merge(a->r,b);
		a->upd();
		return a;
	}else{
		b->l=merge(a,b->l);
		b->upd();
		return b;
	}
}
using pn=pair<np,np>;
pn split(np x,int t){
	if(!x)return pn(0,0);
	if(t<=x->v){
		pn c=split(x->l,t);
		x->l=c.b;
		x->upd();
		return pn(c.a,x);
	}else{
		pn c=split(x->r,t);
		x->r=c.a;
		x->upd();
		return pn(x,c.b);
	}
}
void meld(np x,np&tar){
	if(!x)return;
	meld(x->l,tar);
	meld(x->r,tar);
	x->l=0;
	x->r=0;
	x->upd();
	auto s=split(tar,x->v);
	tar=merge(s.a,merge(x,s.b));
}
