using P=array<int,3>;

template<class t>
t vol(const array<array<t,3>,3>&a){
	t res=0;
	rep(i,3){
		t w=1;
		rep(j,3)w*=a[j][(i+j)%3];
		res+=w;
	}
	rep(i,3){
		t w=1;
		rep(j,3)w*=a[j][(i+3-j)%3];
		res-=w;
	}
	//return abs(res);
	return res;
}


using ld=long double;

ld heron(ld a,ld b,ld c){
	ld s=(a+b+c)/2;
	return sqrt(s*(s-a)*(s-b)*(s-c));
}

//三角形 x を単位球面に射影して原点との距離（は？）
//多分引数を y に差し替えたら普通に 3 点のなす平面と原点の距離が出てくるはずなんだけど
//試してないや
ld dist(array<P,3> x){
	array<array<ld,3>,3> y;
	rep(i,3){
		ld s=0;
		rep(j,3)s+=sq(x[i][j]);
		s=sqrt(s);
		rep(j,3)y[i][j]=x[i][j]/s;
	}
	ld v=vol(y);
	ld len[3];
	rep(i,3){
		ld s=0;
		rep(j,3)s+=sq(y[i][j]-y[(i+1)%3][j]);
		s=sqrt(s);
		len[i]=s;
	}
	ld a=heron(len[0],len[1],len[2]);
	return v/2/a;
}

//verify してない HELP
using P=array<ld,3>;
P cross(P a,P b){
	P res{};
	rep(i,3)res[i]=a[(i+1)%3]*b[(i+2)%3]-a[(i+2)%3]*b[(i+1)%3];
	return res;
}
ld length(P a){
	ld v=0;
	rep(i,3)v+=sq(a[i]);
	return sqrtl(v);
}
//w[0]->w[1]->w[2] を単位球面に射影し，そのあいだの角度を求める
//答えが [0,PI] に収まらないとどうなるかはしらない
ld arg_on_sphere(const array<P,3>&w){
	ld v=vol<ld>(w);
	ld a=length(cross(w[0],w[1]));
	ld b=length(cross(w[1],w[2]));
	ld p=v/a;
	ld h=b/length(w[1]);
	return asinl(min(ld(1),max(ld(-1),p/h)));
}
