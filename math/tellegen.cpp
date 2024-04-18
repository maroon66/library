//機械的に転置する
//メモリ効率とかはもちろん悪い
//計算の手間を省くくらいの用途
//UCUP2-6-K
namespace Tellegen{
	using D=int;
	struct N{
		int pre[2];
		D wei[2];
		N():pre{-1,-1},wei{}{}
	};
	vc<N> buf;
	vc<D> val;
	void clear(){
		buf.clear();
		val.clear();
	}
	
	struct T{
		int i;
		T(){
			i=si(buf);
			buf.eb();
			val.pb(0);
		}
		N*operator->(){return &buf[i];}
		T operator+(T rhs){
			T res;
			res->pre[0]=i;
			res->wei[0]=1;
			res->pre[1]=rhs.i;
			res->wei[1]=1;
			return res;
		}
		T operator+=(T rhs){
			return *this=(*this)+rhs;
		}
		T operator-(T rhs){
			T res;
			res->pre[0]=i;
			res->wei[0]=1;
			res->pre[1]=rhs.i;
			res->wei[1]=-1;
			return res;
		}
		void addweight(D d){
			val[i]+=d;
		}
		D getweight(){
			return val[i];
		}
	};
	
	vc<T> presum(const vc<T>&a){
		vc<T> res(1);
		for(const auto&v:a)res.pb(res.back()+v);
		return res;
	}
	
	void calc(){
		per(i,si(buf))rep(k,2){
			int j=buf[i].pre[k];
			if(j!=-1)val[j]+=val[i]*buf[i].wei[k];
		}
	}
}
