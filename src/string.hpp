//
// NOTE: This is how our _str type works:
//
//    +-----------------+--------+------+
//    |  header (_str)  |  data  |  \0  |
//    +-----------------+--------+------+
//						|
//						+-> returned pointer
//

using str  = char*;       // str
using istr = char*;       // interned string
using cstr = char*;       // char*
using rstr = char const*; // char const*

struct _str {
	Allocator *a;
	u64 size;
	char data[];
};

#define strhdr(s) cast(_str*,(s)-sizeof(_str))

// TODO: use a hash table or something lmao
// TODO: allocate the _istrs linearly
struct _istr {
	_istr *prev;
	_str val;
};

#define istrhdr(s) cast(_istr*,(s)-sizeof(_istr))


VSTD_DEF str mkstr(cstr s, u64 n, Allocator *a = NULL);
VSTD_DEF str mkstr(cstr s, Allocator *a = NULL);
VSTD_DEF void freestr(str s);
VSTD_DEF u64 strsz(str s);
VSTD_DEF str strcopy(str s, Allocator *a = NULL);
VSTD_DEF bool streq(str a, str b);
VSTD_DEF str substr(str s, u64 b, u64 e);
VSTD_DEF str tvsprintf(rstr fmt, va_list args);
VSTD_DEF str tsprintf(rstr fmt, ...);
VSTD_DEF void tfprintf(FILE *fh, rstr fmt, ...);