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