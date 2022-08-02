#ifndef TEMPORARY_STORAGE_SIZE
#define TEMPORARY_STORAGE_SIZE KIBIBYTES(64)
#endif

struct Temporary_Storage : public Allocator {
	u64 used;
    u64 high_water_mark;
    u8 data[TEMPORARY_STORAGE_SIZE];

    void* alloc(u64 n) override {
        assert(used + n < TEMPORARY_STORAGE_SIZE);
        void *p = &data[used];
        used += n;
        if(used > high_water_mark) high_water_mark = used;
        return p;
    }

    void free(void *p) override {}
};

VSTD_DEF Allocator *temp_allocator;
VSTD_DEF Temporary_Storage temporary_storage;

VSTD_DEF void* talloc(u64 n);
VSTD_DEF void treset();