struct Allocator {
	Allocator *parent;

    Allocator(Allocator *_parent = NULL) : parent(_parent) {}
    virtual void deinit() {}
    virtual void* alloc(u64 n) = 0;
    virtual void free(void *p) = 0;
};

VSTD_DEF Allocator *allocator;

VSTD_DEF void push_allocator(Allocator *a);
VSTD_DEF void pop_allocator();
VSTD_DEF void* xalloc(u64 n, Allocator *a = NULL);
VSTD_DEF void xfree(void *p, Allocator *a = NULL);


struct _vstd_new_wrapper{};
inline void* operator new(size_t, _vstd_new_wrapper, void* ptr) { return ptr; }
inline void operator delete(void*, _vstd_new_wrapper, void*) {}
inline void* operator new[](size_t, _vstd_new_wrapper, void *ptr) { return ptr; }
inline void operator delete[](void*, _vstd_new_wrapper, void*) {}
#define pnew(t, p, ...) (new(_vstd_new_wrapper(), p) t(__VA_ARGS__))
#define xanew(t, a, ...) pnew(t, xalloc(sizeof(t), a), __VA_ARGS__)
#define xnew(t, ...) xanew(t, allocator, __VA_ARGS__)