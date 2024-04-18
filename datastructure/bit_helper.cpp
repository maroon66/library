/*
mint c2(int i){return i*(i+1)/2;}
//[l,r) に 1v,2v,3v,... を足し込んでいると思われる
//Verify はされてない
struct AddSum{
	const int n;
	BIT<mint> a,b,c;
	AddSum(int nn):n(nn),a(n+1),b(n+1),c(n+1){}
	void add(int l,int r,mint v){
		//[0,l) 無
		//[l,r)
		//(i-l)*(i-l+1)/2*v
		//(i*(i+1)/2-l*(2i+1)/2+l^2/2)*v
		//i*(i+1)/2 - l*i + l*(l-1)/2
		a.add_range(l,r,v*(l*(l-1)/2));
		b.add_range(l,r,v*(-l));
		c.add_range(l,r,v);
		
		//[r,n+1)
		//c2(r-l)*v
		a.add_range(r,n+1,v*c2(r-l));
	}
	mint eval(int i){
		return a.get(i)+b.get(i)*i+c.get(i)*c2(i);
	}
	mint sum(int l,int r){
		return eval(r)-eval(l);
	}
};
*/

//UCUP 2-17-F
//区間 add, 区間 sum
struct AddSum{
	const int n;
	BIT<mint> a,b;
	AddSum(int nn):n(nn),a(n+1),b(n+1){}
	void add(int l,int r,mint v){
		//[0,l) 無
		//[l,r)
		//(i-l)*(i-l+1)/2*v
		//(i*(i+1)/2-l*(2i+1)/2+l^2/2)*v
		//i*(i+1)/2 - l*i + l*(l-1)/2
		a.add_range(l,r,v*(-l));
		b.add_range(l,r,v);
		
		//[r,n+1)
		//c2(r-l)*v
		a.add_range(r,n+1,v*(r-l));
	}
	mint eval(int i){
		return a.get(i)+b.get(i)*i;
	}
	mint sum(int l,int r){
		return eval(r)-eval(l);
	}
};

//Verify してません!
//バカか?
template<class t>
struct BIT_helper{
	vc<t> val;
	BIT<t> bit;
	BIT_helper(int n):val(n),bit(n){}
	BIT_helper(const vc<t>&ini):val(ini),bit(ini){}
	void add(int i,t v){
		val[i]+=v;
		bit.add(i,v);
	}
	void set(int i,t v){
		bit.add(i,v-val[i]);
		val[i]=v;
	}
	t get(int i){
		return bit.get(i);
	}
	t sum(int l,int r){
		return bit.sum(l,r);
	}
};
