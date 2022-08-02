struct Sys_Allocator : public Allocator {
    void* alloc(u64 n) override {
        void *p = malloc(n);
        memset(p, 0, n);
        return p;
    }

    void free(void *p) override {
        ::free(p);
    }
};


Sys_Allocator sys_allocator;
Allocator *allocator = &sys_allocator;


void push_allocator(Allocator *a) {
    todo();
}

void pop_allocator() {
    todo();
}

void* xalloc(u64 n, Allocator *a) {
	auto a2 = a ? a : allocator;
	return a2->alloc(n);
}

void xfree(void *p, Allocator *a) {
	auto a2 = a ? a : allocator;
	a2->free(p);
}