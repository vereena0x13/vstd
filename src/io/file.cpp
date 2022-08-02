str read_entire_file(str path, Allocator *a) {
	FILE *fh = fopen(path, "rb");
	if(!fh) return NULL;

	fseek(fh, 0L, SEEK_END);
	u64 size = ftell(fh);
	rewind(fh);

	_str *r = cast(_str*, xalloc(sizeof(_str) + size + 1, a));
	r->a = allocator;
	r->size = size;

	assert(fread(r->data, sizeof(char), size, fh) == size);
	r->data[size] = 0;

	fclose(fh);

	return cast(str, r->data);
}

bool write_entire_file(str path, str data) {
	FILE *fh = fopen(path, "wb");
	if(!fh) return false;

	u64 n = strsz(data);
	assert(fwrite(data, sizeof(char), n, fh) == n);

	fflush(fh);
	fclose(fh);

	return true;
}