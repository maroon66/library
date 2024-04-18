
mint invs[vmax];
void initinvs(){
	invs[1]=1;
	rng(v,2,vmax)invs[v]=-(mint::mod/v)*invs[mint::mod%v];
}
