str mkstr(cstr s, u64 n, Allocator *a) {
	auto a2 = a ? a : allocator;
	_str *r = cast(_str*, xalloc(sizeof(_str) + n + 1, a2));
	r->a = a2;
	r->size = n;
	if(s) memcpy(r->data, s, n);
	r->data[n] = 0;
	return r->data;
}

str mkstr(cstr s, Allocator *a) {
	return mkstr(s, strlen(cast(rstr, s)), a);
}

void freestr(str s) {
	auto s2 = strhdr(s);
	s2->a->free(s2);
}

u64 strsz(str s) {
	return strhdr(s)->size;
}

str strcopy(str s, Allocator *a) {
	auto s2 = strhdr(s);
	auto a2 = a ? a : s2->a;
	_str *r = cast(_str*, a2->alloc(sizeof(_str) + s2->size + 1));
	r->a = a2;
	r->size = s2->size;
	memcpy(r->data, s2->data, r->size + 1);
	return r->data;
}

bool streq(str a, str b) {
	if(strsz(a) != strsz(b)) return false;
	return strcmp(a, b) == 0;
}

str substr(str s, u64 b, u64 e) {
	auto s2 = strhdr(s);
	assert(e >= b);
	assert(e < s2->size);
	u64 n = e - b + 1;
	return mkstr(s2->data + b, n);
}

str tvsprintf(rstr fmt, va_list args) {
	va_list args2;
	va_copy(args2, args);
	u64 n = vsnprintf(NULL, 0, fmt, args2);
	va_end(args2);
	auto r = cast(_str*, xalloc(sizeof(_str) + n + 1, temp_allocator));
	vsprintf(r->data, fmt, args);
	r->data[n] = 0;
    r->size = n;
	return r->data;
}

str tsprintf(rstr fmt, ...) {
	va_list args;
	va_start(args, fmt);
	return tvsprintf(fmt, args);
}

void tfprintf(FILE *fh, rstr fmt, ...) {
	va_list args;
	va_start(args, fmt);
	str s = tvsprintf(fmt, args);
	va_end(args);
	fprintf(fh, "%s", s);
}