template<typename T>
constexpr T square(T x) noexcept { return x * x; }

template<typename T>
constexpr T sign(T x) noexcept { return x < 0 ? -1 : 1; }

template<typename T>
constexpr T clamp(T x, T min, T max) noexcept {
	if(x < min) return min;
	if(x > max) return max;
	return x;
}

template<typename T>
constexpr T min(T a, T b) noexcept {
	if(a < b) return a;
	return b;
}

template<typename T>
constexpr T max(T a, T b) noexcept {
	if(a > b) return a;
	return b;
}

template<typename T>
constexpr T abs(T x) noexcept {
	if(x < 0) return -x;
	return x;
}

template<typename T>
constexpr T lerp(T a, T b, T t) noexcept {
	return (1 - t) * a + t * b;
}

template<typename T>
constexpr T unlerp(T min, T max, T value) noexcept {
    return (value - min) / (max - min);
}

template<typename T>
constexpr T relerp(T in_min, T in_max, T value, T out_min, T out_max) noexcept {
    return lerp(out_min, out_max, unlerp(in_min, in_max, value));
}

template<typename T>
constexpr T move_towards(T value, T target, T rate) noexcept {
	if(abs(value - target) < 0.5) return target;
	return lerp(value, target, rate);
}

// NOTE: I did this after trying to write each of these
// as a single function template taking type `t`. But yknow,
// sometimes C++ just doesn't feel like working so meh.
#define DEFROT(t)                             \
    constexpr t rotl(t x, t s) noexcept {     \
        t mask = sizeof(t) * 8 - 1;           \
        s &= mask;                            \
        return (x << s) | (x >> (-s & mask)); \
    }                                         \
    constexpr t rotr(t x, t s) noexcept {     \
        t mask = sizeof(t) * 8 - 1;           \
        s &= mask;                            \
        return (x >> s) | (x << (-s & mask)); \
    }
DEFROT(u8)
DEFROT(u16)
DEFROT(u32)
DEFROT(u64)
#undef DEFROT

// NOTE: And despite the NOTE above about the rotl/rotr functions,
// so far, is_pow2 has caused to trouble. WTF C++? Either work or don't!
template<typename T>
constexpr bool is_pow2(T x) noexcept { return x > 0 && (x & (x - 1)) == 0; }

constexpr u8 next_pow2(u8 y) noexcept {
    u8 x = y;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x++;
    return x;
}

constexpr u16 next_pow2(u16 y) noexcept {
    u16 x = y;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x++;
    return x;
}

constexpr u32 next_pow2(u32 y) noexcept {
    u32 x = y;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

constexpr u64 next_pow2(u64 y) noexcept {
    u64 x = y;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    x++;
    return x;
}

template<typename t>
constexpr t pow(t base, t exp) noexcept {
    if(base == 0 && exp == 0) {
        return 1;
    } else if(base == 0) {
        return 0;
    } else if(exp == 0) {
        return 1;
    }
    t result = 1;
    while(exp) {
        if(exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}


#define BYTE_UNIT_MULTIPLES(X)  \
    X(1,  KILO,   KIBI)         \
    X(2,  MEGA,   MEBI)         \
    X(3,  GIGA,   GIBI)         \
    X(4,  TERA,   TEBI)         \
    X(5,  PETA,   PEBI)         \
    X(6,  EXA,    EXI )

#define X(i, dec, bin)                           \
    constexpr u64 dec##BYTE = pow<u64>(1000, i); \
    constexpr u64 bin##BYTE = pow<u64>(1024, i); \
    constexpr u64 dec##BYTES(u64 n) {            \
        return n * dec##BYTE;                    \
    }                                            \
    constexpr u64 bin##BYTES(u64 n) {            \
        return n * bin##BYTE;                    \
    }
BYTE_UNIT_MULTIPLES(X)
#undef X
