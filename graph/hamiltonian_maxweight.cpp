//Hamiltonian Cycle of Max Weight
//WAIPC-Qual D
template<int nmax=16>
struct Hamiltonian{
	int n;
	const vvc<int>&g;
	vc<array<int,nmax>> buf;
	vi val;
	Hamiltonian(const vvc<int>&gg):n(si(gg)),g(gg),buf(1<<n),val(1<<n,-inf){
		rep(bit,1<<n)rep(i,n)buf[bit][i]=-inf;
		rep(r,n){
			buf[1<<r][r]=0;
			rng(bit,1<<r,1<<(r+1)){
				rep(i,r+1)if((bit&1<<i)&&buf[bit][i]>-inf){
					rep(j,r)if(!(bit&1<<j)){
						chmax(buf[bit|1<<j][j],buf[bit][i]+g[i][j]);
					}
					chmax(val[bit],buf[bit][i]+g[i][r]);
				}
			}
		}
	}
	vi getsol(int bit){
		assert(val[bit]>-inf);
		int r=topbit(bit);
		int last=-1;
		rep(i,r+1)if((bit&1<<i)&&buf[bit][i]>-inf){
			if(val[bit]==buf[bit][i]+g[i][r]){
				last=i;
			}
		}
		vi res;
		while(bit!=(1<<r)){
			assert(last!=-1);
			assert(bit&1<<last);
			res.pb(last);
			bit^=1<<last;
			int nx=-1;
			rep(i,r+1)if((bit&1<<i)&&buf[bit][i]>-inf){
				if(buf[bit|1<<last][last]==buf[bit][i]+g[i][last]){
					nx=i;
				}
			}
			last=nx;
		}
		assert(last==r);
		res.pb(r);
		rein(res);
		return res;
	}
};