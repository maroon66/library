//TCO 2013 R3B Hard
// 1/4 の円の凸包を返す（sqrt(d),0）から (0,sqrt(d)) まで
vc<cm> circle_lattice_convex(int d){
	int e=sqrtl(d);
	cm cur(e,0);
	vc<cm> conv;
	while(cur.x>=0){
		while(si(conv)>=2){
			int s=si(conv);
			if(ccw(conv[s-2],conv[s-1],cur)<=0)conv.pop_back();
			else break;
		}
		conv.pb(cur);
		if(sq(cur.x)+sq(cur.y+1)<=d){
			cur=cm(cur.x,cur.y+1);
		}else{
			cur=cm(cur.x-1,cur.y);
		}
	}
	return conv;
}
