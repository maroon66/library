namespace Nimber64{
	const vi fs{3,5,17,257,641,65537,6700417};
	const ull s=-1;
	
	ull multiply_powers_of_2(ull,ull);

	ull multiply(ull a,ull b) {
	   ull ans = 0;
	   rep(p1,64)if((a>>p1)&1)
		   rep(p2,64)if((b>>p2)&1)
			  ans=ans^multiply_powers_of_2(ull(1)<<p1,ull(1)<<p2);
	   return ans;
	}
	ull multiply_powers_of_2(ull a, ull b) {
		if (a == 1 or b == 1) return a * b;
		int n=__lg(__lg(max(a,b)));
		ull power = ull(1)<<(1<<n);
		if (a >= power and b >= power) {
			return multiply(power * 3 / 2, multiply_powers_of_2(a / power, b / power));
		} else if (a >= power) {
			return multiply_powers_of_2(a / power, b) * power;
		} else {
			return multiply_powers_of_2(a, b / power) * power;
		}
	}
	
	struct precalc{
		ull tmp[64][64];
		unsigned char x[256][256];
		ull y[8][8][256];
		template<int len>
		ull multfast(ull a,ull b){
			ull c=0;
			rep(i,len)if((a>>i)&1)rep(j,len)if((b>>j)&1)c^=tmp[i][j];
			return c;
		}
		precalc():tmp(),x(),y(){
			rep(i,64)rep(j,64)tmp[i][j]=multiply_powers_of_2(1ULL<<i,1ULL<<j);
			rep(i,256)rep(j,256)x[i][j]=multfast<8>(i,j);
			rep(i,8)rep(j,i+1)rep(k,256)y[i][j][k]=multfast<64>(tmp[i*8][j*8],k);
		}
		ull multfast2(ull a,ull b)const{
			ull res=0;
			rep(i,8){
				rep(j,i){
					res^=y[i][j][
					x[(a>>(i*8))&255][(b>>(j*8))&255]^
					x[(a>>(j*8))&255][(b>>(i*8))&255]];
				}
				res^=y[i][i][x[(a>>(i*8))&255][(b>>(i*8))&255]];
			}
			return res;
		}
	};
	const precalc buf;
	
	struct nim{
		ull v;
		nim(ull vv=0):v(vv){}
		nim& operator+=(const nim&rhs){v^=rhs.v;return *this;}
		nim& operator*=(const nim&rhs){v=buf.multfast2(v,rhs.v);return *this;}
		nim& operator/=(const nim&rhs){return (*this)*=rhs.inv();}
		nim operator+(const nim&rhs)const{return nim(*this)+=rhs;}
		nim operator*(const nim&rhs)const{return nim(*this)*=rhs;}
		nim operator/(const nim&rhs)const{return nim(*this)/=rhs;}
		nim pow(ull n)const{
			nim res=1,x=*this;
			while(n){
				if(n&1)res*=x;
				x*=x;
				n>>=1;
			}
			return res;
		}
		nim inv()const{
			return pow(ull(-2));
		}
	};
	istream& operator>>(istream&is,nim&x){
		return is>>x.v;
	}
	ostream& operator<<(ostream&os,const nim&x){
		return os<<x.v;
	}
	
	pair<ull,ull> garnerull(vc<pi> z){
		ull ans=0,w=1;
		rep(i,si(z)){
			int x=z[i].a;
			ans=(ans+w*x);
			rng(j,i+1,si(z))
				z[j].a=(z[j].a+z[j].b-x%z[j].b)*modinv(z[i].b,z[j].b).b%z[j].b;
			w=(w*z[i].b);
		}
		return {ans,w};
	}
	
	//returns minimum x s.t. a^x=b
	//and its period
	//returns (0,0) if infeasible
	pair<ull,ull> discrete_log(const nim a,const nim b){
		vc<pi> z;
		for(auto f:fs){
			nim x=a.pow(s/f);
			nim y=b.pow(s/f);
			if(x.v==1){
				if(y.v==1){
					continue;
				}else{
					return {0,0};
				}
			}
			
			const int L=ceil(sqrt(f-1));
			map<ull,int> v;
			//hash_table<ull,int> v;
			{
				nim cur=1;
				rep(i,L){
					auto itr=v.find(cur.v);
					if(itr==v.ed)v[cur.v]=i;
					cur*=x;
				}
			}
			int ans=-1;
			{
				nim cur=1;
				nim w=x.pow((f-1)*L);
				for(int i=0;i<f;i+=L){
					nim tar=y*cur;
					auto itr=v.find(tar.v);
					if(itr!=v.ed){
						ans=i+itr->b;
						break;
					}
					cur*=w;
				}
			}
			if(ans==-1)return {0,0};
			z.eb(ans,f);
		}
		
		return garnerull(z);
	}
	
	bool checkprimitive(const nim a){
		for(auto f:fs)if((a.pow(s/f)).v==1)return false;
		return true;
	}
}
using Nimber64::nim;
//原子根 ull(1)<<32|6
