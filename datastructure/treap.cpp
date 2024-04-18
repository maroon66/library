//multiuni2021 Day6 E
//update 周りは試してない，大変怪しい
//各点に label を持ち，これが昇順になるように管理している
//おそらく O(NlogN) であると噂されている
const int S=ten(6)+10;

mt19937_64 gen;
struct node{
	using np=node*;
	ull key;
	node *p,*l,*r;
	void init(){
		key=gen();
		p=l=r=nullptr;
	}
	void setl(np x){
		l=x;
		if(x)x->p=this;
	}
	void setr(np x){
		r=x;
		if(x)x->p=this;
	}
	int pos(){
		if(p&&p->l==this)return -1;
		if(p&&p->r==this)return 1;
		return 0;
	}
	void rotate(){
		assert(p);
		np q=p,c;
		if(pos()==1){
			c=l;
			l=p;
			p->r=c;
		}else{
			c=r;
			r=p;
			p->l=c;
		}
		if(c)c->p=p;
		p=p->p;
		q->p=this;
		if(p&&p->l==q)p->l=this;
		if(p&&p->r==q)p->r=this;
		//q->update();
	}
	void rectify(){
		while(p&&p->key<key)rotate();
		//update();
	}
	np left(){
		if(l)return l->left();
		else return this;
	}
	np right(){
		if(r)return r->right();
		else return this;
	}
	ll label;
	void relabel(){
		if(p){
			label=p->label+((p->label&-p->label)>>1)*pos();
		}else{
			label=1LL<<62;
		}
		if(l)l->relabel();
		if(r)r->relabel();
	}
} buf[S];
using np=node*;

int head;
void init(){
	head=0;
}

np newnode(){
	buf[head].init();
	buf[head].relabel();
	return buf+head++;
}

//x の一つあとに newnode 作って返す
np grow(np x){
	np y=newnode();
	if(x->r){
		x->r->left()->setl(y);
	}else{
		x->setr(y);
	}
	y->rectify();
	y->relabel();
	return y;
}
