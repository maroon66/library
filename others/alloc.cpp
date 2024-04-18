//The 1st Universal Cup. Stage 15: Hangzhou J
namespace pool{
char buf[5*ten(7)];
int head;
int snapshot(){return head;}
void rollback(int to){head=to;}
void*allocate(int v){
	void*res=buf+head;
	head+=v;
	return res;
}
template<class T>
struct alloc{
	typedef T value_type;
	alloc()=default;
	template<class U>
	constexpr alloc(const alloc<U>&)noexcept{}
	T* allocate(size_t n){return (T*)pool::allocate(sizeof(T)*n);}
	void deallocate(T*,size_t)noexcept{}
	template<class U>bool operator==(const alloc<U>&)const{return true;}
	template<class U>bool operator!=(const alloc<U>&)const{return false;}
};
}
