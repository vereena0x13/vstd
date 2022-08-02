Allocator *temp_allocator;
Temporary_Storage temporary_storage;

static_init void temp_alloc_init() {
    memset(&temporary_storage, 0, sizeof(Temporary_Storage));
    temp_allocator = &temporary_storage;
}


void* talloc(u64 n) {
	return temporary_storage.alloc(n);
}

void treset() {
	temporary_storage.used = 0;
}