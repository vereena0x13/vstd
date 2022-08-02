u32 murmur3(void const *input, s32 len, u32 seed) {
	constexpr u32 C1 = 0xCC9E2D51;
	constexpr u32 C2 = 0x1B873593;

	u8 const *data = (u8 const*) input;
	s32 n_blocks = len / 4;

	u32 h = seed;

	u32 const *blocks = (u32 const*) (data + n_blocks * 4);
	for(s32 i = -n_blocks; i; i++) {
		u32 k = blocks[i];

		k *= C1;
		k = rotl(k, 15);
		k *= C2;

		h ^= k;
		h = rotl(h, 13);
		h = h * 5 + 0xE6546B64;
	}

	u8 const *tail = (u8 const*) (data + n_blocks * 4);
	u32 k = 0;
	switch(len & 3) {
		case 3: k ^= tail[2] << 16; [[fallthrough]];
		case 2: k ^= tail[1] << 8;  [[fallthrough]];
		case 1: k ^= tail[0];
				k *= C1;
				k = rotl(k, 15);
				k *= C2;
				h ^= k;
	}

	h ^= len;

	h ^= h >> 16;
	h *= 0x85EBCA6B;
	h ^= h >> 13;
	h *= 0xC2B2AE35;
	h ^= h >> 16;

	return h;
}


u32 fnv1a(void const* input, u64 len) {
	u32 x = 0x811C9DC5;
	for(u64 i = 0; i < len; i++) {
		x ^= cast(u8*, input)[i];
		x *= 0x01000193;
	}
	return x;
}